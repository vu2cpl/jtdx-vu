# Ask a TCI server for receive audio and count what arrives - tells "the SDR
# program isn't sending audio" apart from a JTDX-VU problem. Changes nothing
# on the radio and sends no audio_stop. usage: tci_audio_probe.py (port 50001 on
# localhost, receiver 0; edit below for others). Used 2026-10-03 to show
# AetherSDR 26.9.5 answering audio_start with no audio (aethersdr/AetherSDR#6006).
import socket, base64, os, struct, time
s=socket.create_connection(("127.0.0.1",50001),timeout=5)
k=base64.b64encode(os.urandom(16))
s.sendall(b"GET / HTTP/1.1\r\nHost: 127.0.0.1:50001\r\nUpgrade: websocket\r\nConnection: Upgrade\r\nSec-WebSocket-Key: "+k+b"\r\nSec-WebSocket-Version: 13\r\n\r\n")
buf=b""
while b"\r\n\r\n" not in buf: buf+=s.recv(4096)
hdr,_,buf=buf.partition(b"\r\n\r\n"); print(hdr.split(b"\r\n")[0])
def send(t):
    b=t.encode(); m=os.urandom(4)
    s.sendall(bytes([0x81,0x80|len(b)])+m+bytes(c^m[i%4] for i,c in enumerate(b)))
def pump(secs):
    global buf
    end=time.time()+secs; txt=[]; nbin=0; s.settimeout(0.2)
    while time.time()<end:
        try:
            d=s.recv(65536)
            if not d: break
            buf+=d
        except socket.timeout: pass
        while len(buf)>=2:
            op=buf[0]&15; n=buf[1]&127; i=2
            if n==126:
                if len(buf)<4: break
                n=struct.unpack(">H",buf[2:4])[0]; i=4
            elif n==127:
                if len(buf)<10: break
                n=struct.unpack(">Q",buf[2:10])[0]; i=10
            if len(buf)<i+n: break
            p=buf[i:i+n]; buf=buf[i+n:]
            if op==1: txt.append(p.decode(errors="replace"))
            elif op==2: nbin+=n
    return txt,nbin
t,b=pump(2); print("burst:", " ".join(t)[:900]); print("binary in first 2 s:",b)
send("audio_samplerate:48000;"); send("audio_start:0;")
t,b=pump(3); print("after audio_start:", " ".join(x for x in t if not x.startswith("rx_smeter"))[:400]); print("binary bytes in 3 s:",b)
s.close()
