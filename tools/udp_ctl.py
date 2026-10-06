# minimal WSJT-X/JTDX UDP controller: learn the client id from its heartbeat,
# then send SetTxDeltaFreq / TriggerCQ / HaltTx.  usage: udp_ctl.py port cmd...
import socket, struct, sys, time
port = int(sys.argv[1]); cmds = sys.argv[2:]
s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM); s.bind(("127.0.0.1", port)); s.settimeout(30)
data, addr = s.recvfrom(65536)
magic, schema, typ, n = struct.unpack(">IIII", data[:16]); cid = data[16:16+n]
print("client", addr, "schema", schema, "type", typ, "id", cid)
def msg(t, payload=b""):
    return struct.pack(">III", 0xadbccbda, schema, t) + struct.pack(">I", len(cid)) + cid + payload
def qba(b): return struct.pack(">I", len(b)) + b
for c in cmds:
    if c.startswith("df="): s.sendto(msg(50, struct.pack(">I", int(c[3:]))), addr)
    elif c == "cq":        s.sendto(msg(51, qba(b"") + b"\x01"), addr)
    elif c == "halt":      s.sendto(msg(8, b"\x00"), addr)
    elif c.startswith("sleep="): time.sleep(float(c[6:]))
    print("sent", c, flush=True)
