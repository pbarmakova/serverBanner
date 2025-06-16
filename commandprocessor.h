#ifndef COMMANDPROCESSOR_H
#define COMMANDPROCESSOR_H

#include <QTcpSocket>
#include <QString>
#include <QList>

class CommandProcessor {
public:
    CommandProcessor(const QList<QTcpSocket*>& clients);
    QHash<QString, std::function<void(QTcpSocket*, const QJsonObject&)>> getHandlers() const;

private:
    const QList<QTcpSocket*>& clients;
};

#endif // COMMANDPROCESSOR_H
