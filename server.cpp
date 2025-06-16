#include "server.h"
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include "logger.h"
#include "commandprocessor.h"

Server::Server(QObject *parent) : QTcpServer(parent) {}

void Server::startServer(quint16 port) {
    if (listen(QHostAddress::LocalHost, port)) {
        Logger::instance().log("✅ Server started on port " + QString::number(port));
    } else {
        Logger::instance().log("❌ Failed to start server");
    }
}

void Server::incomingConnection(qintptr socketDescriptor) {
    auto socket = new QTcpSocket(this);
    socket->setSocketDescriptor(socketDescriptor);

    clients.append(socket);
    Logger::instance().log("📡 New client connected: " + socket->peerAddress().toString());
    Logger::instance().log("👥 Clients connected: " + QByteArray::number(clients.size()));

    connect(socket, &QTcpSocket::bytesWritten, this, [socket]() {
        if (socket->bytesToWrite() == 0) {
            socket->disconnectFromHost();
        }
    });

    connect(socket, &QTcpSocket::readyRead, [socket, this]() {
        QByteArray data = socket->readAll();
        QString message = QString::fromUtf8(data).trimmed();
        Logger::instance().log("📨 Received: " + message);

        QJsonParseError parseError;
        QJsonDocument jsonDoc = QJsonDocument::fromJson(data, &parseError);

        if (!jsonDoc.isNull() && jsonDoc.isObject()) {
            QJsonObject obj = jsonDoc.object();
            QString command = obj.value("command").toString().toUpper();

            CommandProcessor processor(clients);
            auto handlers = processor.getHandlers();

            if (handlers.contains(command)) {
                handlers[command](socket, obj); // передаём и сокет, и сам JSON!
            } else {
                QString error = "❓ Unknown command: " + command + "\n";
                socket->write(error.toUtf8());
                Logger::instance().log("📤 Sent: " + error.trimmed());
            }
        } else {
            QString error = "❌ Invalid JSON: " + parseError.errorString() + "\n";
            socket->write(error.toUtf8());
            Logger::instance().log("📤 Sent: " + error.trimmed());
        }
    });

    connect(socket, &QTcpSocket::disconnected, [=]() {
        Logger::instance().log("❌ Client disconnected: " + socket->peerAddress().toString());
        clients.removeOne(socket);
        socket->deleteLater();
    });
}
