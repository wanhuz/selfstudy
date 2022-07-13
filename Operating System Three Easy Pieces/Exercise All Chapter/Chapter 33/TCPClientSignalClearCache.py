import selectors
import socket
import types
import random

HOST = '127.0.0.1'
PORT = 64444

sel = selectors.DefaultSelector()
messages = [b'clear']
random.seed(10)

def start_connections(host, port, num_conns):
    server_addr = (HOST, PORT)
    connid = random.randint(1, 100)
    print('starting connection ', connid, 'to ', server_addr)
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.setblocking(False)
    sock.connect_ex(server_addr)
    events = selectors.EVENT_READ | selectors.EVENT_WRITE
    data = types.SimpleNamespace(connid=connid,
                                msg_total=sum(len(m) for m in messages),
                                recv_total=0,
                                messages=list(messages),
                                outb=b'')
    sel.register(sock, events, data=data)
        

def service_connection(key, mask):
    sock = key.fileobj
    data = key.data
    if mask & selectors.EVENT_READ:
        recv_data = sock.recv(1024)
        if recv_data:
            print('received', repr(recv_data), 'from connection ', data.connid)
            data.recv_total += len(recv_data)
        if not recv_data or data.recv_total == data.msg_total:
            print('closing connection to', data.connid)
            sel.unregister(sock)
            sock.close()
    if mask & selectors.EVENT_WRITE:
        if not data.outb and data.messages:
            data.outb = data.messages.pop(0)
        if data.outb:
            sent = sock.send(data.outb)
            data.outb = data.outb[sent:]

start_connections(HOST, PORT, 1)

while True:
    events = sel.select(timeout=1)
    if events:
        for key, mask in events:
            service_connection(key, mask)
    if not sel.get_map():
        break

sel.close()