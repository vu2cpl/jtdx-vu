# A minimal TCI server for testing JTDX-VU's TCI client without a radio.
# usage: fake_tci.py <log file> [port, default 50099]
# Sends the start-up burst (protocol ... start; ready;), echoes every "set"
# command back as the new state and answers queries from that state, the way
# an SDR program does. After audio_start it streams silent receive audio
# (48 kHz float32 stereo frames) - unless the file <log file>.mute exists,
# which imitates an SDR program that acknowledges audio_start but sends
# nothing (AetherSDR 26.9.5 after its DAX release). No IQ. Kill it to
# simulate the SDR program closing; start it again to see the client reconnect.
# FAKE_TRX_DELAY=0.35 delays the trx (PTT) echo as AetherSDR does - the Fake It
# split runaway needs it.
# FAKE_NO_VFO=1 leaves vfo:0,0 out of the start-up burst (AetherSDR, issue #1).
# FAKE_START_DELAY=6 says "stop;" first and "start;" 6 s later (SmartSDR for Mac).
# While trx is on it sends TxChrono and logs "TX over ... peak=" per over;
# a file <log file>.drop drops the client (it should reconnect).
import socket, hashlib, base64, threading, time, sys, struct, os
G = b"258EAFA5-E914-47DA-95CA-C5AB0DC85B11"
log = open(sys.argv[1], 'a', buffering=1)
port = int(sys.argv[2]) if len(sys.argv) > 2 else 50099
# number of leading index arguments per command (the rest is the value)
KEYS = {"vfo": 2, "modulation": 1, "split_enable": 1, "trx": 1, "rx_enable": 1,
        "rx_smeter": 2, "drive": 0, "tune": 1, "rx_channel_enable": 2}
state = {"vfo:0,0": "14074000", "vfo:0,1": "14074000", "modulation:0": "usb",
         "split_enable:0": "false", "trx:0": "false", "rx_enable:1": "false",
         "rx_smeter:0,0": "-80", "drive": "50"}

def frame(text):
    b = text.encode()
    n = len(b)
    hdr = bytes([0x81, n]) if n < 126 else bytes([0x81, 126]) + struct.pack(">H", n)
    return hdr + b

def frames(c):
    buf = b""
    while True:
        while len(buf) < 2:
            d = c.recv(4096)
            if not d: return
            buf += d
        op, n = buf[0] & 0x0f, buf[1] & 0x7f
        i = 2
        if n == 126: need = 4
        elif n == 127: need = 10
        else: need = 2
        while len(buf) < need + 4:
            d = c.recv(4096)
            if not d: return
            buf += d
        if n == 126: n = struct.unpack(">H", buf[2:4])[0]; i = 4
        elif n == 127: n = struct.unpack(">Q", buf[2:10])[0]; i = 10
        mask = buf[i:i+4]; i += 4
        while len(buf) < i + n:
            d = c.recv(4096)
            if not d: return
            buf += d
        data = bytes(b ^ mask[k % 4] for k, b in enumerate(buf[i:i+n]))
        buf = buf[i+n:]
        if op == 8: return
        if op == 1: yield data.decode()
        if op == 2: yield data              # binary: the client's TX audio

# FAKE_VFO_LOCK=1 refuses every frequency change and answers with the frequency
# it keeps, as AetherSDR does with an IC-705's dial lock on (2026-10-19).
VFO_LOCK = bool(os.environ.get("FAKE_VFO_LOCK"))
def reply(cmd):
    name, _, a = cmd.partition(":")
    args = a.split(",") if a else []
    k = KEYS.get(name)
    if k is None:
        return cmd + ";"                 # audio_start:0 etc. - acknowledge
    key = name + (":" + ",".join(args[:k]) if k else "")
    if len(args) > k and not (name == "vfo" and VFO_LOCK):
        state[key] = ",".join(args[k:])
    return key + ("," if k else ":") + state.get(key, "0") + ";"

import os
MUTE = sys.argv[1] + ".mute"
def audio_frame():
    n = 2048                               # float32 stereo samples per frame
    hdr = struct.pack("<16I", 0, 48000, 3, 0, 0, n * 2, 1, 2, *([0] * 8))
    return hdr + bytes(n * 2 * 4)

# FAKE_TCI_WAVS="a.wav b.wav ..." plays 15 s 12 kHz mono recordings (e.g. WSJT-X's
# save folder) instead of silence, one per UTC 15 s period in turn, so a client
# decodes real FT8 without a radio.
WAVS = []
def load_wavs():
    import wave, array
    for path in os.environ.get("FAKE_TCI_WAVS", "").split():
        w = wave.open(path)
        a = array.array("h", w.readframes(w.getnframes()))
        f = array.array("f", (x / 32768.0 for x in a for _ in range(4)))   # 12 -> 48 kHz
        WAVS.append(f)

def stream_wavs(c, alive, lock):
    import array
    n = 2048
    hdr = struct.pack("<16I", 0, 48000, 3, 0, 0, n * 2, 1, 2, *([0] * 8))
    t0 = time.time()
    sent = 0
    while alive[0]:
        t = t0 + sent / 48000.0
        w = WAVS[int(t // 15) % len(WAVS)]
        pos = int((t % 15) * 48000)
        chunk = w[pos:pos + n]
        out = array.array("f", bytes(n * 2 * 4))
        for i, x in enumerate(chunk):
            out[2 * i] = out[2 * i + 1] = x
        f = hdr + out.tobytes(); m = len(f)
        ws = bytes([0x82, 127]) + struct.pack(">Q", m) + f if m >= 65536 else bytes([0x82, 126]) + struct.pack(">H", m) + f
        try:
            with lock: c.sendall(ws)
        except OSError: return
        sent += n
        time.sleep(max(0.0, t0 + sent / 48000.0 - time.time()))

def stream_audio(c, alive, lock):
    if WAVS: return stream_wavs(c, alive, lock)
    f = audio_frame(); n = len(f)
    ws = (bytes([0x82, 126]) + struct.pack(">H", n) if n < 65536 else bytes([0x82, 127]) + struct.pack(">Q", n)) + f
    while alive[0]:
        if not os.path.exists(MUTE):
            try:
                with lock: c.sendall(ws)
            except OSError: return
        time.sleep(2048 / 48000)

PUSH = sys.argv[1] + ".push"
DROP = sys.argv[1] + ".drop"
# While trx is on, send TxChrono every 1024 frames (as AetherSDR) and measure
# the TX audio the client returns: "TX over ... peak=" per over in the log
# (peak 0 = the silent overs of 2026-10-18).
def chrono(c, alive, lock, tx):
    n = 1024                               # stereo frames per block; length counts floats
    hdr = struct.pack("<16I", 0, 48000, 3, 0, 0, n * 2, 3, 2, *([0] * 8))
    ws = bytes([0x82, len(hdr)]) + hdr      # < 126 bytes: 7-bit length, or Qt drops us
    while alive[1]:
        if state.get("trx:0", "").startswith("true"):
            if tx["t0"] is None: tx.update(t0=time.time(), peak=0.0, frames=0)
            try:
                with lock: c.sendall(ws)
            except OSError: return
        elif tx["t0"] is not None:
            log.write("%.3f TX over %.1f s frames=%d peak=%.4f\n"
                      % (time.time(), time.time() - tx["t0"], tx["frames"], tx["peak"]))
            tx["t0"] = None
        time.sleep(n / 48000.0)
    if tx["t0"] is not None:
        log.write("%.3f TX over cut by the drop %.1f s frames=%d peak=%.4f\n"
                  % (time.time(), time.time() - tx["t0"], tx["frames"], tx["peak"]))

def tx_audio(data, tx):
    import array
    if len(data) < 64 or struct.unpack("<I", data[24:28])[0] != 2 or tx["t0"] is None: return
    n = struct.unpack("<I", data[20:24])[0]          # floats; the rest is not filled
    a = array.array("f"); a.frombytes(data[64:64 + min(n, (len(data) - 64) // 4) * 4])
    tx["frames"] += n // 2
    if a: tx["peak"] = max(tx["peak"], max(abs(x) for x in a))

TRX_DELAY = float(os.environ.get("FAKE_TRX_DELAY", "0"))   # seconds before trx is echoed
def pusher(c, alive, lock):
    # a file <log file>.push is sent as one text message, as if the SDR program
    # changed something itself (e.g. "vfo:0,0,21074000;"), then deleted
    while alive[1]:
        if os.path.exists(PUSH):
            t = open(PUSH).read().strip(); os.remove(PUSH)
            for cmd in filter(None, t.split(";")): reply(cmd)   # keep state in step
            try:
                with lock: c.sendall(frame(t))
            except OSError: return
            log.write("%.3f pushed %s\n" % (time.time(), t))
        if os.path.exists(DROP):
            # <log file>.drop: drop this client (as AetherSDR's backlog limit
            # did on 2026-10-18), keep listening for its reconnect
            os.remove(DROP); log.write("%.3f dropping client\n" % time.time())
            try: c.shutdown(socket.SHUT_RDWR)
            except OSError: pass
            return
        time.sleep(0.2)

def serve(c):
    req = b""
    while b"\r\n\r\n" not in req:
        d = c.recv(4096)
        if not d: return
        req += d
    key = [l.split(b":", 1)[1].strip() for l in req.split(b"\r\n") if l.lower().startswith(b"sec-websocket-key")][0]
    acc = base64.b64encode(hashlib.sha1(key + G).digest())
    c.sendall(b"HTTP/1.1 101 Switching Protocols\r\nUpgrade: websocket\r\nConnection: Upgrade\r\nSec-WebSocket-Accept: " + acc + b"\r\n\r\n")
    log.write("%.3f connected\n" % time.time())
    # FAKE_NO_VFO=1 leaves our receiver's vfo out of the burst, as AetherSDR does
    # when its slice is on another receiver index (jtdx-vu issue #1)
    vfo = "" if os.environ.get("FAKE_NO_VFO") else "vfo:0,0,%s;" % state["vfo:0,0"]
    # FAKE_START_DELAY=6 greets with "stop;ready;" and no receiver state, then
    # says "start;" with the state that many seconds later - SmartSDR for Mac
    # just after it has connected to its radio
    late_start = float(os.environ.get("FAKE_START_DELAY", "0"))
    burst = "%smodulation:0,%s;split_enable:0,false;drive:50;" % (vfo, state["modulation:0"])
    head = "protocol:fake,1.5;device:FakeSDR;receive_only:false;trx_count:2;channels_count:2;"
    if late_start:
        c.sendall(frame(head + "stop;ready;"))
        def started():
            time.sleep(late_start)
            try:
                c.sendall(frame("start;" + burst))
                log.write("%.3f late start sent\n" % time.time())
            except OSError: pass
        threading.Thread(target=started, daemon=True).start()
    else:
        c.sendall(frame(head + burst + "start;ready;"))
    alive = [False, True]
    lock = threading.Lock()
    threading.Thread(target=pusher, args=(c, alive, lock), daemon=True).start()
    tx = {"t0": None}
    threading.Thread(target=chrono, args=(c, alive, lock, tx), daemon=True).start()
    try:
        for msg in frames(c):
            if isinstance(msg, bytes):
                tx_audio(msg, tx); continue
            for cmd in filter(None, msg.split(";")):
                r = reply(cmd)
                if cmd.startswith("audio_start") and not alive[0]:
                    alive[0] = True
                    threading.Thread(target=stream_audio, args=(c, alive, lock), daemon=True).start()
                elif cmd.startswith("audio_stop"):
                    alive[0] = False
                if not cmd.startswith("rx_smeter"): log.write("%.3f < %s  > %s\n" % (time.time(), cmd, r))
                if cmd.startswith("trx") and TRX_DELAY:
                    # like AetherSDR: Tx confirmed ~0.35 s later, after the vfo echo
                    def late(r=r):
                        try:
                            with lock: c.sendall(frame(r))
                        except OSError: pass
                    threading.Timer(TRX_DELAY, late).start()
                    continue
                with lock: c.sendall(frame(r))
    except OSError:
        pass
    alive[0] = alive[1] = False
    state["trx:0"] = "false"             # the SDR program unkeys a client that goes
    log.write("%.3f closed\n" % time.time())

load_wavs()
s = socket.socket(); s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
s.bind(("127.0.0.1", port)); s.listen(5)
while True:
    c, _ = s.accept(); threading.Thread(target=serve, args=(c,), daemon=True).start()
