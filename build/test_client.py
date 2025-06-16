import socket, json

HOST = '127.0.0.1'
PORT = 1234

request = {"command": "GET_FILE", "filename": "video.mp4"}

sock = socket.create_connection((HOST, PORT))
sock.sendall((json.dumps(request) + "\n").encode('utf-8'))

# Получаем meta
meta_bytes = b''
while not meta_bytes.endswith(b'\n'):
    chunk = sock.recv(1)
    if not chunk:
        print('Сервер разорвал соединение до передачи мета')
        exit(1)
    meta_bytes += chunk

meta_end = meta_bytes.index(b'\n') + 1
meta_part = meta_bytes[:meta_end]
file_part = meta_bytes[meta_end:]

meta = json.loads(meta_part.decode().strip())
print("meta:", meta)
print("file_part len:", len(file_part))

received = len(file_part)
with open("received_" + meta["filename"], "wb") as f:
    if file_part:
        f.write(file_part)
    while received < meta["size"]:
        data = sock.recv(min(65536, meta["size"] - received))
        if not data:
            print('recv дал пустоту')
            break
        f.write(data)
        received += len(data)
        print(f"{received}/{meta['size']} bytes received")

print("Done, received:", received)
