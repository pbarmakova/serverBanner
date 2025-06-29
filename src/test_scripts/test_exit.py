import socket
import json

HOST = '127.0.0.1'
PORT = 1234

sock = socket.create_connection((HOST, PORT))
sock.sendall((json.dumps({"command": "EXIT"}) + "\n").encode("utf-8"))
response = sock.recv(4096)
print("Ответ:", response.decode(errors="ignore"))
sock.close()
