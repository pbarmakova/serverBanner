#ifndef COMMANDPROCESSOR_H
#define COMMANDPROCESSOR_H

#include <QTcpSocket>
#include <QString>
#include <QList>

/*
 * @class CommandProcessor
 * @brief Обработка команд клиентов. Маршрутизирует команды на соответствующие обработчики.
 */

class CommandProcessor {
public:
    /*
     * @brief Конструктор CommandProcessor
     * @param clients Список всех подключённых клиентов
     */
    CommandProcessor(const QList<QTcpSocket*>& clients);
    /*
     * @brief Получить таблицу обработчиков команд (по имени команды)
     * @return QHash с обработчиками команд
     */
    QHash<QString, std::function<void(QTcpSocket*, const QJsonObject&)>> getHandlers() const;
private:
    const QList<QTcpSocket*>& clients; //< Список клиентов (для команд, требующих доступа к списку)

};

#endif // COMMANDPROCESSOR_H
