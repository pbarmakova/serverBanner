import socket
import json

# Параметры подключения (замени на нужные)
HOST = '127.0.0.1'  # или твой IP
PORT = 1234        # твой порт

# Формируем JSON-команду
request = {
    "command": "GET_FILE",
    "filename": "cat.jpg"   # или другой файл из /assets/
}

# Подключаемся к серверу
sock = socket.create_connection((HOST, PORT))

# Отправляем JSON-запрос
sock.sendall(json.dumps(request).encode('utf-8'))

# Принимаем ответ (файл) и сохраняем
with open("received_cat.jpg", "wb") as f:
    while True:
        data = sock.recv(4096)
        if not data:
            break
        f.write(data)

sock.close()
print("Файл получен!")
