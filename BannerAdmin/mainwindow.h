#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "server.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
class QTcpSocket;
}
QT_END_NAMESPACE

/*
 * @class MainWindow
 * @brief Главное окно графического интерфейса для отображения подключённых клиентов.
 *
 * Отображает список клиентов, подключённых к серверу, их статус и время подключения.
 * Запускает и управляет экземпляром сервера.
 */

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /*
     * @brief Конструктор MainWindow
     * @param parent Родительский виджет
     */
    MainWindow(QWidget *parent = nullptr);
    /*
     * @brief Деструктор MainWindow
     */
    ~MainWindow();

public slots:
    /*
     * @brief Слот вызывается при подключении клиента
     * @param socket Указатель на сокет клиента
     */
    void onClientConnected(QTcpSocket* socket);
    /*
     * @brief Слот вызывается при отключении клиента
     * @param socket Указатель на сокет клиента
     */
    void onClientDisconnected(QTcpSocket* socket);

private:
    Ui::MainWindow *ui; //< Автоматически сгенерированный интерфейс
    Server* server; // < Экземпляр TCP-сервера
    QHash<QTcpSocket*, int> socketToRow; // Сопоставление сокет → строка таблицы
};

#endif // MAINWINDOW_H
