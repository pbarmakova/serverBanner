#include "commandprocessor.h"
#include "logger.h"
#include <QJsonObject>
#include <QTcpSocket>

CommandProcessor::CommandProcessor(const QList<QTcpSocket*>& clients)
    : clients(clients) {}

QHash<QString, std::function<void(QTcpSocket*)>> CommandProcessor::getHandlers() const{
    QHash<QString, std::function<void(QTcpSocket*)>> handlers;

    handlers["PING"] = [](QTcpSocket* socket) {
        socket->write("📤 PONG\n");
        Logger::instance().log("📤 Sent: PONG");
    };

    handlers["EXIT"] = [](QTcpSocket* socket) {
        socket->write("👋 Bye\n");
        Logger::instance().log("📤 Sent: Bye");
        QObject::connect(socket, &QTcpSocket::bytesWritten, socket, [socket]() {
            socket->disconnectFromHost();
        });
    };

    handlers["STATUS"] = [this](QTcpSocket* socket) {
        QString status = "👥 Clients connected: " + QString::number(this->clients.size()) + "\n";
        socket->write(status.toUtf8());
        Logger::instance().log("📤 Sent: " + status.trimmed());
    };

    return handlers;
}
