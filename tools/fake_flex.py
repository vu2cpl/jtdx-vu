#!/usr/bin/env python3
"""JTDX-VU: a fake FlexRadio for the VITA-49 rig type, no radio needed.

Speaks enough of the SmartSDR API (TCP, default 4992) for FlexTransceiver's
start-up - V/H greeting, C<seq>| commands answered R<seq>|0|, slice status
lines, "client gui", "client udpport", "slice create/tune/set", "dax audio
set", "stream create type=dax_rx/dax_tx", "xmit" - and then plays the band
as tools/ft2_tci_sim.py does: Gaussian noise plus a scripted FT2 QSO at the
SNRs asked for, as DAX RX VITA-49 packets (type 3, class 0x534C03E3, 24 kHz
stereo float32 BE, 128 frames = 1052 bytes) to the port the client named.
DAX TX packets (type 1, class 0x534C0123, 128 mono int16 BE) arriving on UDP
4991 are counted and kept as 12 kHz wavs, one per transmission, so ft2dec can
check what JTDX-VU sent.

FAKE_GUI_SLICE=1 hands a new GUI client a slice at 14.100 USB, as the 6600 does.
FAKE_NO_ECHO=1 answers "slice tune" / "slice set" OK but never reports the
change in the slice status (seen on the FLEX-6600, 2026-10-07).
FAKE_TUNE_DELAY=0.8 delays the status echo of "slice tune", as a real radio
does on a band change (the 2026-10-07 hang needed it).
usage: fake_flex.py --sig DIR --snr -10,-14 [--repeats 2] [--port 4992] [--log fake_flex.txt]
                    [--wait 25] [--noise 0.05] [--txdir DIR] [--freq 14074000]
"""
import argparse, array, math, operator, os, random, socket, struct, sys, threading, time, wave
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from tci_player import load_signals, PERIOD

RATE = 24000
FPP = 128
HANDLE = 0x5A4B3C2D
OUI = 0x00001C2D
RX_CLASS, TX_CLASS = 0x534C03E3, 0x534C0123
log = None
lock = threading.Lock()
state = {"udpport": 0, "slices": {}, "rx_stream": 0, "tx_stream": 0, "next_slice": 0, "xmit": 0, "peer": None}

def logw(s):
    log.write("%.3f %s\n" % (time.time(), s)); log.flush()

def status(sock, text):
    sock.sendall(("S%08X|%s\n" % (HANDLE, text)).encode())

def slice_line(n):
    s = state["slices"][n]
    return ("slice %d in_use=1 sample_rate=24000 RF_frequency=%.6f client_handle=0x%08X index_letter=%s rxant=ANT1 "
            "mode=%s wide=0 txant=ANT1 dax=%d tx=%d active=1 ant_list=ANT1,ANT2 mode_list=LSB,USB,DIGU,DIGL,CW"
            % (n, s["freq"] / 1e6, HANDLE, chr(ord('A') + n), s["mode"], s["dax"], s["tx"]))

def serve(c, a):
    state["peer"] = c.getpeername()[0]
    c.sendall(b"V1.4.0.0\n" + ("H%08X\n" % HANDLE).encode())
    logw("client connected")
    buf = b""
    try:
        while True:
            d = c.recv(4096)
            if not d: break
            buf += d
            while b"\n" in buf:
                line, buf = buf.split(b"\n", 1)
                line = line.decode(errors="replace").strip()
                if not line.startswith("C"): continue
                seq, _, cmd = line[1:].partition("|")
                body, extra = "", []
                p = cmd.split()
                with lock:
                    if p[0] == "client" and p[1] == "gui" and os.environ.get("FAKE_GUI_SLICE"):
                        # like the 6600: a new GUI client is handed a slice at the radio's default
                        n = state["next_slice"]; state["next_slice"] += 1
                        state["slices"][n] = {"freq": 14100000, "mode": "USB", "dax": 0, "tx": 1}
                        extra.append(slice_line(n))
                    elif p[0] == "client" and p[1] == "udpport": state["udpport"] = int(p[2])
                    elif p[0] == "sub" and p[1] == "slice":
                        for n in state["slices"]: extra.append(slice_line(n))
                    elif p[0] == "info": body = 'model="FLEX-6600",chassis_serial="FAKE",software_ver=3.8.19,atu_present=1'
                    elif p[0] == "slice" and p[1] == "create":
                        n = state["next_slice"]; state["next_slice"] += 1
                        f = a.freq
                        for kv in p[2:]:
                            if kv.startswith("freq="): f = int(round(float(kv[5:]) * 1e6))
                        state["slices"][n] = {"freq": f, "mode": "DIGU", "dax": 0, "tx": 1}
                        body = str(n); extra.append(slice_line(n))
                    elif p[0] == "slice" and p[1] == "tune":
                        n = int(p[2]); f_new = int(round(float(p[3]) * 1e6))
                        # FAKE_BAND_MODE=1: like a real Flex, a band change brings back that
                        # band's last mode - LSB below 10 MHz, USB above (a fresh radio)
                        if os.environ.get("FAKE_BAND_MODE") and (f_new < 10e6) != (state["slices"][n]["freq"] < 10e6):
                            state["slices"][n]["mode"] = "LSB" if f_new < 10e6 else "USB"
                        state["slices"][n]["freq"] = f_new
                        d = float(os.environ.get("FAKE_TUNE_DELAY", "0"))   # a real radio confirms a band change late
                        if os.environ.get("FAKE_NO_ECHO"): pass              # accept, never report it (what the 6600 seemed to do)
                        elif d > 0:
                            line_ = slice_line(n)
                            threading.Timer(d, lambda c=c, l=line_: status(c, l)).start()
                        else: extra.append(slice_line(n))
                    elif p[0] == "slice" and p[1] in ("set", "s"):
                        n = int(p[2])
                        for kv in p[3:]:
                            k, _, v = kv.partition("=")
                            if k == "mode": state["slices"][n]["mode"] = v.upper()
                            elif k == "tx": state["slices"][n]["tx"] = int(v)
                        if not os.environ.get("FAKE_NO_ECHO"): extra.append(slice_line(n))
                    elif p[0] == "slice" and p[1] == "remove":
                        n = int(p[2]); state["slices"].pop(n, None); extra.append("slice %d in_use=0" % n)
                    elif p[0] == "dax" and p[1] == "audio" and p[2] == "set":
                        for n in state["slices"]: state["slices"][n]["dax"] = int(p[3])
                    elif p[0] == "stream" and p[1] == "create":
                        if "type=dax_rx" in cmd:
                            state["rx_stream"] = 0x04000001; body = "0x%08X" % state["rx_stream"]
                            extra.append("stream 0x%08X type=dax_rx dax_channel=1 client_handle=0x%08X ip=%s port=%d"
                                         % (state["rx_stream"], HANDLE, state["peer"], state["udpport"]))
                        else:
                            state["tx_stream"] = 0x84000000; body = "0x%08X" % state["tx_stream"]
                            extra.append("stream 0x%08X type=dax_tx client_handle=0x%08X" % (state["tx_stream"], HANDLE))
                    elif p[0] == "stream" and p[1] == "remove":
                        sid = int(p[2], 16)
                        if sid == state["rx_stream"]: state["rx_stream"] = 0
                        if sid == state["tx_stream"]: state["tx_stream"] = 0
                    elif p[0] == "xmit":
                        state["xmit"] = int(p[1]); extra.append("interlock state=%s" % ("TRANSMITTING" if state["xmit"] else "READY"))
                logw("< %s" % cmd)
                c.sendall(("R%s|0|%s\n" % (seq, body)).encode())
                for e in extra: status(c, e)
    except OSError:
        pass
    with lock:
        state["rx_stream"] = state["tx_stream"] = 0; state["udpport"] = 0; state["xmit"] = 0
    logw("client closed")

def tx_listener(a):
    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM); s.bind(("0.0.0.0", 4991))
    cur = None; n = 0; last = 0; idx = 0
    while True:
        s.settimeout(1.0)
        try:
            d, _ = s.recvfrom(4096)
        except socket.timeout:
            if cur is not None and time.time() - last > 1.0:
                f = os.path.join(a.txdir, "tx%02d.wav" % idx); idx += 1
                w = wave.open(f, "wb"); w.setnchannels(1); w.setsampwidth(2); w.setframerate(12000)
                w.writeframes(cur.tobytes()); w.close()
                logw("TX transmission ended: %d packets, %.2f s -> %s" % (n, len(cur) / 12000.0, f))
                cur = None; n = 0
            continue
        if len(d) < 28 or struct.unpack(">I", d[12:16])[0] != TX_CLASS: continue
        if cur is None: cur = array.array("h"); n = 0; logw("TX transmission started")
        samples = struct.unpack(">%dh" % ((len(d) - 28) // 2), d[28:])
        for i in range(0, len(samples) - 1, 2): cur.append(int((samples[i] + samples[i + 1]) / 2))   # 24 -> 12 kHz
        n += 1; last = time.time()

def main():
    global log
    ap = argparse.ArgumentParser()
    ap.add_argument("--sig", required=True); ap.add_argument("--snr", default="-10"); ap.add_argument("--repeats", type=int, default=2)
    ap.add_argument("--port", type=int, default=4992); ap.add_argument("--log", default="fake_flex.txt")
    ap.add_argument("--wait", type=float, default=25.0); ap.add_argument("--noise", type=float, default=0.05)
    ap.add_argument("--txdir", default="."); ap.add_argument("--freq", type=int, default=14074000)
    ap.add_argument("--forever", action="store_true", help="keep streaming noise after the plan")
    a = ap.parse_args()
    log = open(a.log, "a", buffering=1)
    os.makedirs(a.txdir, exist_ok=True)
    sigma = a.noise
    # 24 kHz stream (12 kHz Nyquist): noise RMS sigma per sample is 10*log10(0.5*12000/2500) = 3.8 dB
    # under a full-scale tone in 2500 Hz - not the 6.8 of the 48 kHz TCI sim
    levels = [(sigma * 10 ** ((float(v) - 3.8) / 20), float(v)) for v in filter(None, a.snr.split(","))]
    sigs48 = load_signals(a.sig)
    rendered = {}
    for g, _ in levels:
        for k, sig in sigs48.items():
            rendered[(k, g)] = array.array("f", [sig[i] * g for i in range(0, len(sig), 2)])   # 24 kHz mono
    rnd = random.Random(2)
    noise = array.array("f", [rnd.gauss(0.0, sigma) for _ in range(RATE * 12)])
    NB = len(noise) - FPP - 1
    t = math.ceil((time.time() + a.wait) / 15) * 15
    plan = []
    for g, snr in levels:
        for r in range(a.repeats):
            log.write(f"{t:.3f} {time.strftime('%Y%m%d_%H%M%S', time.gmtime(t))} level {g:.6f} rep {r} snr {snr:.1f}\n")
            for k in range(6): plan.append((t + k * PERIOD, k + 1, g))
            t += 8 * PERIOD
    t_end = t + 5
    print(f"{len(plan)} periods, first at {time.strftime('%H:%M:%S', time.gmtime(plan[0][0]))} UTC, ends {time.strftime('%H:%M:%S', time.gmtime(t_end))}", flush=True)

    srv = socket.socket(); srv.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1); srv.bind(("0.0.0.0", a.port)); srv.listen(5)
    def acceptor():
        while True:
            c, _ = srv.accept(); threading.Thread(target=serve, args=(c, a), daemon=True).start()
    threading.Thread(target=acceptor, daemon=True).start()
    threading.Thread(target=tx_listener, args=(a,), daemon=True).start()

    udp = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    dt = FPP / RATE
    t0 = time.time(); n = 0; sent = 0; count = 0
    while a.forever or time.time() < t_end:
        bt = t0 + n * dt
        delay = bt - time.time()
        if delay > 0: time.sleep(delay)
        with lock: sid, port, peer = state["rx_stream"], state["udpport"], state["peer"]
        if sid and port:
            off = rnd.randrange(0, NB)
            block = noise[off:off + FPP]
            for (ts, k, g) in plan:
                if ts + PERIOD < bt or ts > bt + dt: continue
                sig = rendered[(k, g)]
                so = int(round((bt - ts) * RATE))
                lo, hi = max(0, -so), min(FPP, len(sig) - so)
                if hi <= lo: continue
                mixed = array.array("f", block)
                mixed[lo:hi] = array.array("f", map(operator.add, mixed[lo:hi], sig[so + lo:so + hi]))
                block = mixed
            # stereo L == R, big-endian floats
            st = array.array("f", bytes(8 * FPP)); st[0::2] = block; st[1::2] = block
            if sys.byteorder == "little": st.byteswap()
            hdr = struct.pack(">IIIIII", 0x38500107 | ((count & 0x0f) << 16), sid, OUI, RX_CLASS, int(bt), 0) + struct.pack(">I", 0)
            udp.sendto(hdr + st.tobytes(), (peer, port))
            count += 1; sent += 1
        n += 1
    print(f"done: {sent} RX packets", flush=True)

if __name__ == "__main__":
    main()
