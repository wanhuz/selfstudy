import socket

HOST = '127.0.0.1'
PORT = 64444

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.connect((HOST, PORT))
    s.sendall(b'Hellowww')
    data = s.recv(1024)

print('Received, today\'s date: ', repr(data))