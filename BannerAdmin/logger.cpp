//Singleton для логирования событий в файл и консоль.

#include "logger.h"
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QDebug>

/*
 * @brief Получить экземпляр Logger (singleton).
 * @return Ссылка на Logger
 */
Logger& Logger::instance() {
    static Logger instance;
    return instance;
}

/*
 * @brief Записать сообщение в лог-файл и вывести его в консоль.
 * @param message Сообщение для логирования
 */
void Logger::log(const QString& message) {
    qDebug() << message;
    QFile file("log.txt");
    if (file.open(QIODevice::Append | QIODevice::Text)) {
        QTextStream out(&file);
        out << QDateTime::currentDateTime().toString("[yyyy-MM-dd hh:mm:ss] ") << message << "\n";
    }
}
