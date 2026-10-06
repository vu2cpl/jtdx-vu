#!/usr/bin/env python3
"""JTDX-VU: play scripted FT2 periods into Christo's TCI_HV echo server, as a
TCI client.  SUPERSEDED on 2026-10-07 by tools/ft2_tci_sim.py (the harness's
own TCI server): TCI_HV paces its stream on a Qt timer that ran at 47-87 %
of real time here, so what this client plays comes out time-compressed and
nothing decodes.  Kept for the websocket client and the resampler, which
ft2_tci_sim.py imports (load_signals).

Original description: play scripted FT2 periods into the echo server, as a
TCI client that answers the server's TxChrono frames with transmit audio -
no radio, no MSHV head, no hands.  The server adds its own Gaussian noise
and every other client (an MSHV head, a JTDX-VU -r tcitest) hears the mix,
so two decoders are scored on the same audio.

usage: tci_player.py --sig DIR --levels g1,g2,... [--repeats N] [--port 50002]
                     [--layout frames|flat] [--log schedule.txt]

DIR holds p1.wav .. p6.wav from ft2sim (12 kHz, one 3.75 s period each, made
with snr 99 = clean, full scale).  One sequence = 8 FT2 periods = 30 s: the six
messages of a QSO (CQ, reply, report, R-report, RR73, 73) then two silent
periods, started on a 15 s UTC boundary so the even/odd period bookkeeping of
the decoders' AP7 sees a real QSO.  Each level is the signal amplitude g
(1.0 = full scale) and is repeated N times; with the server at -n 100 (noise
1.0) the SNR in 2500 Hz is about 20*log10(g) + 32 dB.  The schedule
(UTC start, level) goes to --log for the scorer.
"""
import argparse, array, base64, math, os, socket, struct, sys, time, wave

FS_IN, FS_OUT = 12000, 48000
PERIOD = 3.75

# ---------------------------------------------------------------- websocket
class WS:
    def __init__(self, host, port):
        self.s = socket.create_connection((host, port), timeout=10)
        key = base64.b64encode(os.urandom(16)).decode()
        self.s.sendall((f"GET / HTTP/1.1\r\nHost: {host}:{port}\r\nUpgrade: websocket\r\n"
                        f"Connection: Upgrade\r\nSec-WebSocket-Key: {key}\r\n"
                        "Sec-WebSocket-Version: 13\r\n\r\n").encode())
        buf = b""
        while b"\r\n\r\n" not in buf:
            d = self.s.recv(4096)
            if not d: raise RuntimeError("handshake failed")
            buf += d
        self.buf = buf.split(b"\r\n\r\n", 1)[1]
        self.s.settimeout(None)
    def send(self, op, payload):
        mask = b"\0\0\0\0"      # a legal masking key; XOR with 0 is the identity, so no per-byte loop
        n = len(payload)
        hdr = bytes([0x80 | op])
        if n < 126: hdr += bytes([0x80 | n])
        elif n < 65536: hdr += bytes([0x80 | 126]) + struct.pack(">H", n)
        else: hdr += bytes([0x80 | 127]) + struct.pack(">Q", n)
        self.s.sendall(hdr + mask + payload)
    def _need(self, n):
        while len(self.buf) < n:
            d = self.s.recv(65536)
            if not d: raise RuntimeError("server closed")
            self.buf += d
    def recv(self):
        """-> (opcode, payload) of one complete message (continuations joined)."""
        payload = b""; op0 = None
        while True:
            self._need(2)
            b0, b1 = self.buf[0], self.buf[1]
            fin, op = b0 & 0x80, b0 & 0x0f
            n, i = b1 & 0x7f, 2
            if n == 126: self._need(4); n = struct.unpack(">H", self.buf[2:4])[0]; i = 4
            elif n == 127: self._need(10); n = struct.unpack(">Q", self.buf[2:10])[0]; i = 10
            self._need(i + n)
            payload += self.buf[i:i + n]; self.buf = self.buf[i + n:]
            if op0 is None: op0 = op
            if op == 9: self.send(10, payload); payload = b""; op0 = None; continue   # ping
            if fin: return op0, payload

def _mask_fast(payload, mask):
    a = array.array("I", payload[:len(payload) & ~3])
    m = struct.unpack("<I", mask)[0]
    for i in range(len(a)): a[i] ^= m
    tail = bytes(b ^ mask[i & 3] for i, b in enumerate(payload[len(payload) & ~3:], start=len(payload) & ~3))
    return a.tobytes() + tail

# ---------------------------------------------------------------- signals
def upsample4(x):
    """12 kHz -> 48 kHz, windowed-sinc lowpass (cutoff 5 kHz), pure Python."""
    L, half = 4, 16
    taps = []
    for k in range(-half * L, half * L + 1):
        t = k / L
        s = 1.0 if k == 0 else math.sin(math.pi * t * 0.8333) / (math.pi * t * 0.8333)   # 5 kHz / 6 kHz Nyquist of 12 k
        w = 0.5 - 0.5 * math.cos(2 * math.pi * (k + half * L) / (2 * half * L))
        taps.append(s * w)
    g = L / sum(taps)
    taps = [t * g for t in taps]
    # polyphase: output sample n = sum_m x[m] * taps[n - 4m + half*L]
    N = len(x); out = array.array("f", bytes(4 * N * L))
    xs = list(x)
    for n in range(N * L):
        acc = 0.0
        base = n + half * L
        m0 = max(0, (base - 2 * half * L + 3) // L); m1 = min(N - 1, base // L)
        for m in range(m0, m1 + 1):
            acc += xs[m] * taps[base - L * m]
        out[n] = acc
    return out

def load_signals(d):
    sigs = {}
    for i in range(1, 7):
        cache = os.path.join(d, f"p{i}.f32")
        if os.path.exists(cache):
            a = array.array("f"); a.frombytes(open(cache, "rb").read()); sigs[i] = a; continue
        w = wave.open(os.path.join(d, f"p{i}.wav")); assert w.getframerate() == FS_IN and w.getnchannels() == 1
        raw = array.array("h"); raw.frombytes(w.readframes(w.getnframes()))
        x = [v / 32768.0 for v in raw]
        a = upsample4(x); open(cache, "wb").write(a.tobytes()); sigs[i] = a
        print(f"resampled p{i}: {len(a)} samples", flush=True)
    return sigs

# ---------------------------------------------------------------- main
def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--sig", required=True); ap.add_argument("--levels", required=True)
    ap.add_argument("--repeats", type=int, default=4); ap.add_argument("--port", type=int, default=50002)
    ap.add_argument("--host", default="127.0.0.1"); ap.add_argument("--layout", default="flat")
    ap.add_argument("--record", default="", help="write the receive stream (ch 0, 12 kHz int16 wav) here")
    ap.add_argument("--pace", default="rx", help="rx: send one transmit block per receive block (the server builds exactly one per tick, so this follows its consumption); chrono: answer TxChrono frames")
    ap.add_argument("--setup", action="store_true", help="configure the server's audio stream (48 kHz float32 stereo, 2048) as an MSHV head does - before the decoders connect")
    ap.add_argument("--wait", type=float, default=5.0, help="seconds before the first sequence (time for the decoders to start)")
    ap.add_argument("--log", default="schedule.txt"); ap.add_argument("--lead", type=float, default=0.0,
                    help="seconds to send audio early (latency compensation)")
    a = ap.parse_args()
    levels = [float(v) for v in a.levels.split(",")]
    sigs = load_signals(a.sig)
    rendered = {}            # (k, g) -> interleaved stereo float32 bytes, 8 bytes per frame
    for g in levels:
        for k, sig in sigs.items():
            st = array.array("f", bytes(8 * len(sig)))
            st[0::2] = array.array("f", [v * g for v in sig]); st[1::2] = st[0::2]
            rendered[(k, g)] = st.tobytes()
    log = open(a.log, "a")

    ws = WS(a.host, a.port)
    if a.setup:
        for cmd in ("audio_samplerate:48000;", "audio_stream_sample_type:float32;", "audio_stream_channels:2;",
                    "audio_stream_samples:2048;", "tx_stream_audio_buffering:50;"):
            ws.send(1, cmd.encode())
    # schedule: list of (abs start time, period file, g)
    t = math.ceil((time.time() + a.wait) / 15) * 15   # first sequence on a 15 s boundary, >= --wait s away
    plan = []
    for g in levels:
        for r in range(a.repeats):
            log.write(f"{t:.3f} {time.strftime('%Y%m%d_%H%M%S', time.gmtime(t))} level {g} rep {r}\n"); log.flush()
            for k in range(6): plan.append((t + k * PERIOD, k + 1, g))
            t += 8 * PERIOD
    t_end = t
    print(f"{len(plan)} periods, {len(levels)} levels x {a.repeats}, ends {time.strftime('%H:%M:%S', time.gmtime(t_end))} UTC", flush=True)

    # No stream set-up commands: JTDX-VU (started first) has already put the
    # server into float32 / 2048-sample blocks, and re-sending them restarts the
    # server's stream under the other clients ("partial loss of data" in JTDX-VU,
    # then no decodes at all).  The TxChrono header says what the server wants.
    rec = array.array("h")
    if a.record or a.pace == "rx": ws.send(1, b"audio_start:0;")
    hdr_fmt = "<16I"
    n_sent = 0; t0 = None
    # The server paces its blocks on its own clock (measured 5-7 % slower than
    # wall time), so the audio must be continuous from block to block - a
    # period's signal starts at the first block after its wall-clock start and
    # then runs sample by sample, whatever the block cadence.
    active = None            # [rendered bytes, byte position]
    counts = {}
    silence = bytes(8 * 8192)
    started = set()
    while time.time() < t_end + 2:
        op, msg = ws.recv()
        if op != 2 or len(msg) < 64: continue
        h = struct.unpack(hdr_fmt, msg[:64])
        receiver, srate, fmt, codec, crc, length, typ, chans = h[:8]
        counts[typ] = counts.get(typ, 0) + 1
        if typ == 1 and receiver == 0 and a.record:     # RxAudioStream: keep channel 0, every 4th frame
            if not rec: log.write(f"{time.time():.3f} record_start\n"); log.flush()
            fl = array.array("f"); fl.frombytes(msg[64:64 + 4 * length])
            rec.extend(int(max(-32768, min(32767, v * 32767))) for v in fl[0::8])
        if receiver != 0: continue
        if a.pace == "rx" and typ != 1: continue        # one TX block per RX block (= per server tick)
        if a.pace == "chrono" and typ != 3: continue    # or one per TxChrono
        now = time.time()
        if t0 is None: t0 = now; print(f"first {'RX block' if typ == 1 else 'TxChrono'}: length {length} chans {chans} srate {srate}", flush=True)
        nfloats = length * 2 if a.layout == "frames" else length
        nframes = nfloats // 2
        # a due period pre-empts whatever is still playing (only trailing silence
        # by then: a file's signal ends 2.97 s in, the slow stream stretches it to ~3.4 s)
        for idx, (ts, k, g) in enumerate(plan):
            if idx not in started and ts - 0.02 <= now + a.lead < ts + 1.0:
                started.add(idx); active = [rendered[(k, g)], 0]
                log.write(f"{now:.3f} start p{k} g {g} late {now - ts:.3f}\n"); log.flush(); break
        nbytes = 8 * nframes
        if active is None: block = silence[:nbytes] if len(silence) >= nbytes else bytes(nbytes)
        else:
            buf, pos = active
            block = buf[pos:pos + nbytes]
            if len(block) < nbytes: block += bytes(nbytes - len(block)); active = None
            else: active[1] = pos + nbytes
        out = struct.pack(hdr_fmt, 0, FS_OUT, 3, 0, 0, length, 2, 2, *([0] * 8)) + block
        ws.send(2, out)
        n_sent += 1
    if a.record:
        ws.send(1, b"audio_stop:0;")
        w = wave.open(a.record, "wb"); w.setnchannels(1); w.setsampwidth(2); w.setframerate(FS_IN)
        w.writeframes(rec.tobytes()); w.close(); print(f"recorded {len(rec) / FS_IN:.1f} s to {a.record}", flush=True)
    print(f"frames received by type (1=RX audio, 3=TxChrono): {counts}", flush=True)
    print(f"done: {n_sent} blocks in {time.time() - t0:.1f} s = {n_sent * 2048 / max(1e-9, time.time() - t0) / 1000:.1f} kframes/s", flush=True)

if __name__ == "__main__":
    main()
