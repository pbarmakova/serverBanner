#include "server.h"         // подключаем заголовок, где объявлены методы
#include <QDebug>           // для вывода сообщений в консоль (аналог printf)

Server::Server(QObject *parent) : QTcpServer(parent) {
    // Конструктор сервера
    // Пока ничего не настраиваем, просто передаём родителя
    // Qt автоматически освободит память, если будет parent
}
void Server::startServer(quint16 port) {
    // Пытаемся начать прослушивание порта
    if (listen(QHostAddress::LocalHost, port)) {
        // Успешно
        qDebug() << "✅ Server started on port" << port;
    } else {
        // Ошибка
        qDebug() << "❌ Failed to start server";
    }
}
void Server::incomingConnection(qintptr socketDescriptor) {
    // Создаём сокет для клиента
    auto socket = new QTcpSocket(this);

    // Привязываем сокет к уже установленному соединению
    socket->setSocketDescriptor(socketDescriptor);

    // Добавляем его в список подключённых клиентов
    clients.append(socket);
    qDebug() << "📡 New client connected:" << socket->peerAddress().toString();
    qDebug() << "👥 Total clients connected:" << clients.count();


    // Обработка входящих данных от клиента
    connect(socket, &QTcpSocket::readyRead, [socket]() {
        QByteArray data = socket->readAll();               // читаем всё, что прислал клиент
        QString message = QString::fromUtf8(data).trimmed();
        qDebug() << "📨 Received:" << data;

        if (message.compare("PING", Qt::CaseInsensitive) == 0) {
            socket->write("PONG\n");
        } else if (message.compare("STATUS", Qt::CaseInsensitive) == 0) {
            socket->write("👥 Clients connected: " + QByteArray::number(clients.size()) + "\n");
        } else if (message.compare("EXIT", Qt::CaseInsensitive) == 0) {
            socket->write("👋 Bye!\n");
            socket->disconnectFromHost();
        } else {
            socket->write("❓ Unknown command\n");
        }
    });

    // Обработка отключения клиента
    connect(socket, &QTcpSocket::disconnected, [=]() {
        qDebug() << "❌ Client disconnected:" << socket->peerAddress().toString();
        clients.removeOne(socket);                         // удаляем из списка
        socket->deleteLater();                             // безопасно удаляем сокет
    });
}
