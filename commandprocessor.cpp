#include "commandprocessor.h"
#include "logger.h"
#include <QJsonObject>
#include <QTcpSocket>
#include <QCoreApplication>
#include <QFile>

constexpr qint64 CHUNK_SIZE = 64 * 1024; // 64 KB, можно менять на 128*1024 или др.


CommandProcessor::CommandProcessor(const QList<QTcpSocket*>& clients)
    : clients(clients) {}

QHash<QString, std::function<void(QTcpSocket*, const QJsonObject&)>> CommandProcessor::getHandlers() const{
    QHash<QString, std::function<void(QTcpSocket*, const QJsonObject&)>> handlers;

    handlers["PING"] = [](QTcpSocket* socket, const QJsonObject&) {
        socket->write("📤 PONG\n");
        Logger::instance().log("📤 Sent: PONG");
    };

    handlers["EXIT"] = [](QTcpSocket* socket, const QJsonObject&) {
        socket->write("👋 Bye\n");
        socket->flush();
        Logger::instance().log("📤 Sent: Bye");
        socket->disconnectFromHost();
    };

    handlers["STATUS"] = [this](QTcpSocket* socket, const QJsonObject&) {
        QString status = "👥 Clients connected: " + QString::number(this->clients.size()) + "\n";
        socket->write(status.toUtf8());
        Logger::instance().log("📤 Sent: " + status.trimmed());
    };

    // Новый обработчик GET_FILE
    handlers["GET_FILE"] = [](QTcpSocket* socket, const QJsonObject& json) {

        QString filename = json.value("filename").toString();
        Logger::instance().log("🔥 Получен запрос GET_FILE на " + filename);
        if (filename.isEmpty()) {
            socket->write("ERROR: Missing filename\n");
            Logger::instance().log("❌ Missing filename in GET_FILE");
            socket->flush();
            socket->disconnectFromHost();
            return;
        }
        QString path = QCoreApplication::applicationDirPath() + "/assets/" + filename;
        QFile file(path);

        if (!file.exists()) {
            QString msg = "ERROR: File not found: " + filename + "\n";
            socket->write(msg.toUtf8());
            Logger::instance().log(msg.trimmed());
            socket->flush();
            socket->disconnectFromHost();
            return;
        }

        if (!file.open(QIODevice::ReadOnly)) {
            QString msg = "ERROR: Cannot open file: " + filename + "\n";
            socket->write(msg.toUtf8());
            Logger::instance().log(msg.trimmed());
            socket->flush();
            socket->disconnectFromHost();
            return;
        }

        // Сначала отправим метаинформацию (например, размер и имя файла)
        QJsonObject meta;
        meta["filename"] = filename;
        meta["size"] = file.size();
        QJsonDocument doc(meta);
        QByteArray metaJson = doc.toJson(QJsonDocument::Compact) + "\n"; // \n — разделитель

        socket->write(metaJson);
        socket->flush();

        // Теперь отправим файл по чанкам
        constexpr qint64 CHUNK_SIZE = 64 * 1024; // 64 KB
        while (!file.atEnd()) {
            QByteArray chunk = file.read(CHUNK_SIZE);
            if (chunk.isEmpty())
                break;
            socket->write(chunk);
            socket->flush();

            // waitForBytesWritten по необходимости, как раньше
            if (socket->bytesToWrite() > 2 * CHUNK_SIZE)
                socket->waitForBytesWritten(-1);
        }
        file.close();
        socket->flush();

        Logger::instance().log("📤 File sent: " + filename + " (" + QString::number(meta["size"].toInt()) + " bytes)");
        socket->disconnectFromHost();

    };

    return handlers;
}
