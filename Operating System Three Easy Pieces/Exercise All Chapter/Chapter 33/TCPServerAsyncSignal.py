from asyncio.tasks import sleep
import socket
from datetime import datetime
import selectors
import types
import aiofiles
import asyncio

cache = "" #Empty cache string

async def openUsingAsync(file_name):
    await asyncio.sleep(3) #Simulate file blocking since I can't find big enough file to block by IO
    async with aiofiles.open(file_name, mode='r') as f:
        contents = await f.read()
    return contents


def accept_wrapper(sock):
    conn, addr = sock.accept()
    print('accepted connection from ', addr)
    conn.setblocking(False)
    data = types.SimpleNamespace(addr=addr, inb=b'', outb=b'')
    events = selectors.EVENT_READ | selectors.EVENT_WRITE
    sel.register(conn, events, data=data)

async def service_connection(key, mask):
    global cache
    sock = key.fileobj
    data = key.data
    if mask & selectors.EVENT_READ:
        recv_data = sock.recv(1024)
        if recv_data:

            if recv_data == b'clear':
                print("Clearing cache!")
                cache = ""
                file_content = "Cache cleared"
            elif cache != "":
                print("Cache exist!")
                file_content = cache
            else:
                print("Nothing in Cache yet!")
                file_content = await openUsingAsync(recv_data)
                cache = file_content

            data.outb = file_content.encode()
        else:
            print('closing connection to', data.addr)
            sel.unregister(sock)
            sock.close()
    if mask & selectors.EVENT_WRITE:
        if data.outb:
            sent = sock.send(data.outb)
            data.outb = data.outb[sent:]


#Main function

HOST = '127.0.0.1'
PORT = 64444

sel = selectors.DefaultSelector()
lsock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
lsock.bind((HOST, PORT))
lsock.listen()
print('listening on', (HOST, PORT))
lsock.setblocking(False)
sel.register(lsock, selectors.EVENT_READ, data=None)

async def main():
    while True:
        events = sel.select(timeout=None)
        for key, mask in events:
            if key.data is None:
                accept_wrapper(key.fileobj)
            else:
                await service_connection(key, mask)

asyncio.run(main())

