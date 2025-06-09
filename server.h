#ifndef SERVER_H    //защита от двойного подключения классов, чтобы консоль не выдавала ошибок
#define SERVER_H

#include <QTcpServer>  // класс для TCP-сервера
#include <QTcpSocket>  // класс для клиентского соединения
#include <QList>       // для хранения списка клиентов
#include <QObject>

// Класс Server наследуется от QTcpServer
class Server : public QTcpServer {
    Q_OBJECT // макрос Qt для поддержки сигналов и слотов

public:
    explicit Server(QObject *parent = nullptr);      // конструктор
    void startServer(quint16 port);                  // метод запуска сервера на заданном порту

protected:
    void incomingConnection(qintptr socketDescriptor) override;
    // вызывается автоматически, когда приходит новое подключение

private:
    QList<QTcpSocket*> clients;  // список подключённых клиентов
    void log(const QString &message);

};

#endif // SERVER_H
