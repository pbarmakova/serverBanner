#include "logger.h"
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QDebug>

Logger& Logger::instance() {
    static Logger instance;
    return instance;
}

void Logger::log(const QString& message) {
    qDebug() << message;
    QFile file("log.txt");
    if (file.open(QIODevice::Append | QIODevice::Text)) {
        QTextStream out(&file);
        out << QDateTime::currentDateTime().toString("[yyyy-MM-dd hh:mm:ss] ") << message << "\n";
    }
}
