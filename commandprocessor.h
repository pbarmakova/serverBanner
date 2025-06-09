#ifndef COMMANDPROCESSOR_H
#define COMMANDPROCESSOR_H

#include <QTcpSocket>
#include <QString>
#include <QList>

class CommandProcessor {
public:
    CommandProcessor(const QList<QTcpSocket*>& clients);
    void process(const QString& event, QTcpSocket* socket);
    QHash<QString, std::function<void(QTcpSocket*)>> getHandlers() const;

private:
    const QList<QTcpSocket*>& clients;
};

#endif // COMMANDPROCESSOR_H
