#!/usr/bin/env python3
"""JTDX-VU: a TCI server that *is* the band - Gaussian noise plus a scripted FT2
QSO at chosen SNRs, streamed at an exact 48 kHz on the wall clock to every
client that asks for audio (a JTDX-VU -r tcitest, an MSHV head), so two
decoders are scored on identical audio with nothing to click.  Built on
tools/fake_tci.py (handshake, start-up burst, state echo).

usage: ft2_tci_sim.py --sig DIR --snr -10,-14,-16 [--repeats 4] [--port 50002]
                      [--log schedule.txt] [--wait 25] [--noise 0.05]

DIR: p1.wav..p6.wav from ft2sim (12 kHz, clean, full scale).  One sequence =
8 FT2 periods (30 s): CQ, reply, report, R-report, RR73, 73, two silent, from
a 15 s UTC boundary.  SNR is in 2500 Hz: with noise RMS sigma per sample the
signal amplitude is g = sigma * 10^((snr - 6.8)/20).  The schedule (start, g,
snr) goes to --log; tools/ft2_score.py reads it.  Exits when the plan is done.
"""
import argparse, array, base64, hashlib, math, operator, os, random, socket, struct, sys, threading, time
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from tci_player import load_signals, FS_OUT, PERIOD

G = b"258EAFA5-E914-47DA-95CA-C5AB0DC85B11"
BLOCK = 2048                       # frames per audio frame
KEYS = {"vfo": 2, "modulation": 1, "split_enable": 1, "trx": 1, "rx_enable": 1,
        "rx_smeter": 2, "drive": 0, "tune": 1, "rx_channel_enable": 2}
state = {"vfo:0,0": "14074000", "vfo:0,1": "14074000", "modulation:0": "usb",
         "split_enable:0": "false", "trx:0": "false", "rx_enable:1": "false",
         "rx_smeter:0,0": "-80", "drive": "50"}
clients = []                       # [socket, lock, audio_on]
clients_lock = threading.Lock()
log = None

def frame(text):
    b = text.encode(); n = len(b)
    return (bytes([0x81, n]) if n < 126 else bytes([0x81, 126]) + struct.pack(">H", n)) + b

def frames(c):
    buf = b""
    while True:
        while len(buf) < 2:
            d = c.recv(4096)
            if not d: return
            buf += d
        op, n = buf[0] & 0x0f, buf[1] & 0x7f
        i, need = 2, 2
        if n == 126: need = 4
        elif n == 127: need = 10
        while len(buf) < need + 4:
            d = c.recv(4096)
            if not d: return
            buf += d
        if n == 126: n = struct.unpack(">H", buf[2:4])[0]; i = 4
        elif n == 127: n = struct.unpack(">Q", buf[2:10])[0]; i = 10
        mask = buf[i:i + 4]; i += 4
        while len(buf) < i + n:
            d = c.recv(65536)
            if not d: return
            buf += d
        data = bytes(b ^ mask[k & 3] for k, b in enumerate(buf[i:i + n])) if n < 1024 else b""
        buf = buf[i + n:]
        if op == 8: return
        if op == 9: c.sendall(bytes([0x8a, 0]))
        if op == 1 and data: yield data.decode(errors="replace")

def reply(cmd):
    name, _, a = cmd.partition(":")
    args = a.split(",") if a else []
    k = KEYS.get(name)
    if k is None: return cmd + ";"
    key = name + (":" + ",".join(args[:k]) if k else "")
    if len(args) > k: state[key] = ",".join(args[k:])
    return key + ("," if k else ":") + state.get(key, "0") + ";"

def serve(c):
    req = b""
    while b"\r\n\r\n" not in req:
        d = c.recv(4096)
        if not d: return
        req += d
    key = [l.split(b":", 1)[1].strip() for l in req.split(b"\r\n") if l.lower().startswith(b"sec-websocket-key")][0]
    acc = base64.b64encode(hashlib.sha1(key + G).digest())
    c.sendall(b"HTTP/1.1 101 Switching Protocols\r\nUpgrade: websocket\r\nConnection: Upgrade\r\nSec-WebSocket-Accept: " + acc + b"\r\n\r\n")
    me = [c, threading.Lock(), False]
    with clients_lock: clients.append(me)
    log.write("%.3f client connected (%d)\n" % (time.time(), len(clients))); log.flush()
    c.sendall(frame("protocol:ExpertSDR3,1.9;device:FT2sim;receive_only:false;trx_count:2;channels_count:2;"
                    "audio_samplerate:48000;vfo:0,0,%s;vfo:0,1,%s;modulation:0,%s;split_enable:0,false;drive:50;"
                    "tx_enable:0,true;rx_enable:0,true;start;ready;"
                    % (state["vfo:0,0"], state["vfo:0,1"], state["modulation:0"])))
    try:
        for msg in frames(c):
            for cmd in filter(None, msg.split(";")):
                r = reply(cmd)
                if cmd.startswith("audio_start"): me[2] = True
                elif cmd.startswith("audio_stop"): me[2] = False
                if not cmd.startswith(("rx_smeter", "vfo:0,0;", "modulation:0;")):
                    log.write("%.3f < %s  > %s\n" % (time.time(), cmd, r)); log.flush()
                with me[1]: c.sendall(frame(r))
    except OSError:
        pass
    with clients_lock:
        if me in clients: clients.remove(me)
    log.write("%.3f client closed\n" % time.time()); log.flush()

def main():
    global log
    ap = argparse.ArgumentParser()
    ap.add_argument("--sig", required=True); ap.add_argument("--snr", default=""); ap.add_argument("--levels", default="")
    ap.add_argument("--repeats", type=int, default=4); ap.add_argument("--port", type=int, default=50002)
    ap.add_argument("--log", default="schedule.txt"); ap.add_argument("--wait", type=float, default=25.0)
    ap.add_argument("--noise", type=float, default=0.05, help="noise RMS per sample, full scale = 1")
    a = ap.parse_args()
    log = open(a.log, "a", buffering=1)
    sigma = a.noise
    plan_levels = []           # (g, label)
    for v in filter(None, a.snr.split(",")): plan_levels.append((sigma * 10 ** ((float(v) - 6.8) / 20), float(v)))
    for v in filter(None, a.levels.split(",")): plan_levels.append((float(v), 20 * math.log10(float(v) / sigma) + 6.8))
    if not plan_levels: sys.exit("give --snr or --levels")
    sigs = load_signals(a.sig)
    rendered = {}
    for g, _ in plan_levels:
        for k, sig in sigs.items():
            st = array.array("f", bytes(8 * len(sig)))
            st[0::2] = array.array("f", [v * g for v in sig]); st[1::2] = st[0::2]
            rendered[(k, g)] = st
    # 12 s of stereo Gaussian noise, used from random offsets
    rnd = random.Random(1)
    noise = array.array("f", [rnd.gauss(0.0, sigma) for _ in range(FS_OUT * 12 * 2)])
    NB = len(noise) // 2 - BLOCK - 1
    t = math.ceil((time.time() + a.wait) / 15) * 15
    plan = []
    for g, snr in plan_levels:
        for r in range(a.repeats):
            log.write(f"{t:.3f} {time.strftime('%Y%m%d_%H%M%S', time.gmtime(t))} level {g:.6f} rep {r} snr {snr:.1f}\n")
            for k in range(6): plan.append((t + k * PERIOD, k + 1, g))
            t += 8 * PERIOD
    t_end = t + 5
    print(f"{len(plan)} periods, {len(plan_levels)} levels x {a.repeats}, first at "
          f"{time.strftime('%H:%M:%S', time.gmtime(plan[0][0]))} UTC, ends {time.strftime('%H:%M:%S', time.gmtime(t_end))}", flush=True)

    s = socket.socket(); s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    s.bind(("127.0.0.1", a.port)); s.listen(5)
    def acceptor():
        while True:
            c, _ = s.accept(); threading.Thread(target=serve, args=(c,), daemon=True).start()
    threading.Thread(target=acceptor, daemon=True).start()

    hdr = struct.pack("<16I", 0, FS_OUT, 3, 0, 0, BLOCK * 2, 1, 2, *([0] * 8))
    n = len(hdr) + BLOCK * 8                           # minimal length encoding, or Qt drops the link
    wshdr = bytes([0x82, 126]) + struct.pack(">H", n) if n < 65536 else bytes([0x82, 127]) + struct.pack(">Q", n)
    dt = BLOCK / FS_OUT
    t0 = time.time(); n = 0; sent = 0; late_max = 0.0
    while True:
        bt = t0 + n * dt                                   # wall time of this block's first sample
        if bt >= t_end: break
        delay = bt - time.time()
        if delay > 0: time.sleep(delay)
        else: late_max = max(late_max, -delay)
        off = rnd.randrange(0, NB) * 2
        block = noise[off:off + BLOCK * 2]
        for (ts, k, g) in plan:
            if ts + PERIOD < bt or ts > bt + dt: continue
            sig = rendered[(k, g)]
            so = int(round((bt - ts) * FS_OUT)) * 2        # stereo index of the signal at block start
            lo, hi = max(0, -so), min(BLOCK * 2, len(sig) - so)
            if hi <= lo: continue
            mixed = array.array("f", block)
            mixed[lo:hi] = array.array("f", map(operator.add, mixed[lo:hi], sig[so + lo:so + hi]))
            block = mixed
        payload = wshdr + hdr + block.tobytes()
        with clients_lock: cl = list(clients)
        for me in cl:
            if not me[2]: continue
            try:
                with me[1]: me[0].sendall(payload)
                sent += 1
            except OSError:
                me[2] = False
        n += 1
    print(f"done: {n} blocks, {sent} sent, worst lateness {late_max * 1000:.0f} ms", flush=True)

if __name__ == "__main__":
    main()
