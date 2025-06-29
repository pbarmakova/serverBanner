//Класс обработки JSON-команд: хранит обработчики команд (PING, EXIT, STATUS, GET_FILE), вызывает нужные действия.
/*
Описание основных команд:

PING — Проверка соединения (ответ: PONG)
EXIT — Отключить клиента
STATUS — Вернуть количество подключенных клиентов
GET_FILE — Передать файл по TCP (см. реализацию)
*/

#include "commandprocessor.h"
#include "logger.h"
#include <QJsonObject>
#include <QTcpSocket>
#include <QCoreApplication>
#include <QFile>

constexpr qint64 CHUNK_SIZE = 64 * 1024; // 64 KB, можно менять на 128*1024 или др.

/*
 * @brief Конструктор CommandProcessor.
 * @param clients Список всех подключённых клиентов
 */
CommandProcessor::CommandProcessor(const QList<QTcpSocket*>& clients)
    : clients(clients) {}

/*
 * @brief Возвращает хэш-таблицу обработчиков команд по имени команды.
 * @return QHash с функциями-обработчиками
 */
QHash<QString, std::function<void(QTcpSocket*, const QJsonObject&)>> CommandProcessor::getHandlers() const{
    QHash<QString, std::function<void(QTcpSocket*, const QJsonObject&)>> handlers;

    /*
     * @brief Обработчик команды PING.
     * Отправляет клиенту строку "PONG" и логирует событие.
     */
    handlers["PING"] = [](QTcpSocket* socket, const QJsonObject&) {
        socket->write("📤 PONG\n");
        Logger::instance().log("📤 Sent: PONG");
    };

    /*
     * @brief Обработчик команды EXIT.
     * Завершает соединение с клиентом и логирует событие.
     */
    handlers["EXIT"] = [](QTcpSocket* socket, const QJsonObject&) {
        socket->write("👋 Bye\n");
        socket->flush();
        Logger::instance().log("📤 Sent: Bye");
        socket->disconnectFromHost();
    };

    /*
     * @brief Обработчик команды STATUS.
     * Отправляет количество подключённых клиентов.
     */
    handlers["STATUS"] = [this](QTcpSocket* socket, const QJsonObject&) {
        QString status = "👥 Clients connected: " + QString::number(this->clients.size()) + "\n";
        socket->write(status.toUtf8());
        Logger::instance().log("📤 Sent: " + status.trimmed());
    };

    /*
     * @brief Обработчик команды GET_FILE.
     * Отправляет файл из папки assets клиенту по TCP (в чанках).
     * В запросе должен быть параметр filename.
     */
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
        Logger::instance().log("Путь поиска файла: " + path);
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
