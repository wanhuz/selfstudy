import socket
from datetime import datetime

HOST = '127.0.0.1'
PORT = 64444

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.bind((HOST, PORT))
    s.listen()
    conn, addr = s.accept()
    with conn:
        print('Connected by', addr)
        while True:
            data = conn.recv(1024)
            if not data:
                break
            now = datetime.now()
            today = now.strftime("%d/%m/%Y %H:%M:%S").encode()
            conn.sendall(today)
            