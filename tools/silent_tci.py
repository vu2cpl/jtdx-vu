# Accepts a websocket handshake on 127.0.0.1:50099 and then never says a word.
import socket, hashlib, base64, threading, time, sys
G=b"258EAFA5-E914-47DA-95CA-C5AB0DC85B11"
log=open(sys.argv[1],'a',buffering=1)
def serve(c):
    req=b""
    while b"\r\n\r\n" not in req:
        d=c.recv(4096)
        if not d: return
        req+=d
    key=[l.split(b":",1)[1].strip() for l in req.split(b"\r\n") if l.lower().startswith(b"sec-websocket-key")][0]
    acc=base64.b64encode(hashlib.sha1(key+G).digest())
    c.sendall(b"HTTP/1.1 101 Switching Protocols\r\nUpgrade: websocket\r\nConnection: Upgrade\r\nSec-WebSocket-Accept: "+acc+b"\r\n\r\n")
    log.write("%.3f connected\n"%time.time())
    while c.recv(4096): pass
    log.write("%.3f closed\n"%time.time())
s=socket.socket(); s.setsockopt(socket.SOL_SOCKET,socket.SO_REUSEADDR,1); s.bind(("127.0.0.1",50099)); s.listen(5)
while True:
    c,_=s.accept(); threading.Thread(target=serve,args=(c,),daemon=True).start()
