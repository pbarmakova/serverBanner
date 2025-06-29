#ifndef SERVER_H    //защита от двойного подключения классов, чтобы консоль не выдавала ошибок
#define SERVER_H

#include <QTcpServer>  // класс для TCP-сервера
#include <QTcpSocket>  // класс для клиентского соединения
#include <QList>       // для хранения списка клиентов
#include <QObject>

// Класс Server наследуется от QTcpServer

/*
 * @class Server
 * @brief TCP-сервер, принимающий подключения и обрабатывающий команды клиентов.
 *
 * Управляет списком клиентов, парсит JSON-команды, вызывает обработчики.
 */
class Server : public QTcpServer {
    Q_OBJECT // макрос Qt для поддержки сигналов и слотов

public:
    /*
     * @brief Конструктор Server
     * @param parent Родительский QObject
     */
    explicit Server(QObject *parent = nullptr);      // конструктор
    /*
     * @brief Запустить сервер на указанном порту
     * @param port Номер порта для прослушивания
     */
    void startServer(quint16 port);                  // метод запуска сервера на заданном порту

signals:
    /*
     * @brief Сигнал о подключении нового клиента
     * @param socket Сокет нового клиента
     */
    void clientConnected(QTcpSocket* socket);
    /*
     * @brief Сигнал об отключении клиента
     * @param socket Сокет отключённого клиента
     */
    void clientDisconnected(QTcpSocket* socket);

protected:
    /*
     * @brief Обработка нового входящего соединения
     * @param socketDescriptor Дескриптор сокета
     */
    void incomingConnection(qintptr socketDescriptor) override; // вызывается автоматически, когда приходит новое подключение

private:
    QList<QTcpSocket*> clients;  // список подключённых клиентов
    void log(const QString &message);

};

#endif // SERVER_H
