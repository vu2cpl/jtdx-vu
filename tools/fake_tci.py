# A minimal TCI server for testing JTDX-VU's TCI client without a radio.
# usage: fake_tci.py <log file> [port, default 50099]
# Sends the start-up burst (protocol ... start; ready;), echoes every "set"
# command back as the new state and answers queries from that state, the way
# an SDR program does. After audio_start it streams silent receive audio
# (48 kHz float32 stereo frames) - unless the file <log file>.mute exists,
# which imitates an SDR program that acknowledges audio_start but sends
# nothing (AetherSDR 26.9.5 after its DAX release). No IQ. Kill it to
# simulate the SDR program closing; start it again to see the client reconnect.
import socket, hashlib, base64, threading, time, sys, struct
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

def reply(cmd):
    name, _, a = cmd.partition(":")
    args = a.split(",") if a else []
    k = KEYS.get(name)
    if k is None:
        return cmd + ";"                 # audio_start:0 etc. - acknowledge
    key = name + (":" + ",".join(args[:k]) if k else "")
    if len(args) > k:
        state[key] = ",".join(args[k:])
    return key + ("," if k else ":") + state.get(key, "0") + ";"

import os
MUTE = sys.argv[1] + ".mute"
def audio_frame():
    n = 2048                               # float32 stereo samples per frame
    hdr = struct.pack("<16I", 0, 48000, 3, 0, 0, n * 2, 1, 2, *([0] * 8))
    return hdr + bytes(n * 2 * 4)

def stream_audio(c, alive, lock):
    f = audio_frame(); n = len(f)
    ws = (bytes([0x82, 126]) + struct.pack(">H", n) if n < 65536 else bytes([0x82, 127]) + struct.pack(">Q", n)) + f
    while alive[0]:
        if not os.path.exists(MUTE):
            try:
                with lock: c.sendall(ws)
            except OSError: return
        time.sleep(2048 / 48000)

PUSH = sys.argv[1] + ".push"
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
    c.sendall(frame("protocol:fake,1.5;device:FakeSDR;receive_only:false;trx_count:2;channels_count:2;"
                    "vfo:0,0,%s;modulation:0,%s;split_enable:0,false;drive:50;start;ready;"
                    % (state["vfo:0,0"], state["modulation:0"])))
    alive = [False, True]
    lock = threading.Lock()
    threading.Thread(target=pusher, args=(c, alive, lock), daemon=True).start()
    try:
        for msg in frames(c):
            for cmd in filter(None, msg.split(";")):
                r = reply(cmd)
                if cmd.startswith("audio_start") and not alive[0]:
                    alive[0] = True
                    threading.Thread(target=stream_audio, args=(c, alive, lock), daemon=True).start()
                elif cmd.startswith("audio_stop"):
                    alive[0] = False
                if not cmd.startswith("rx_smeter"): log.write("%.3f < %s  > %s\n" % (time.time(), cmd, r))
                with lock: c.sendall(frame(r))
    except OSError:
        pass
    alive[0] = alive[1] = False
    log.write("%.3f closed\n" % time.time())

s = socket.socket(); s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
s.bind(("127.0.0.1", port)); s.listen(5)
while True:
    c, _ = s.accept(); threading.Thread(target=serve, args=(c,), daemon=True).start()
