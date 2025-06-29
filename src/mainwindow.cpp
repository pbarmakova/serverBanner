//Определяет и реализует главное окно графического интерфейса, связывает интерфейс и сервер, показывает клиентов.

#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "server.h"
#include <QDateTime>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Инициализация и запуск сервера
    server = new Server(this);
    server->startServer(1234); // или другой порт
    statusBar()->showMessage("Сервер запущен на порту 1234");


    connect(server, &Server::clientConnected, this, &MainWindow::onClientConnected);
    connect(server, &Server::clientDisconnected, this, &MainWindow::onClientDisconnected);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onClientConnected(QTcpSocket* socket)
{
    int row = ui->clientsTableWidget->rowCount();
    ui->clientsTableWidget->insertRow(row);
    ui->clientsTableWidget->setItem(row, 0, new QTableWidgetItem(socket->peerAddress().toString()));
    ui->clientsTableWidget->setItem(row, 1, new QTableWidgetItem("Connected"));
    ui->clientsTableWidget->setItem(row, 2, new QTableWidgetItem(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")));

    socketToRow[socket] = row;  // Привязываем сокет к строке

}

void MainWindow::onClientDisconnected(QTcpSocket* socket)
{
    if (socketToRow.contains(socket)) {
        int row = socketToRow[socket];
        ui->clientsTableWidget->setItem(row, 1, new QTableWidgetItem("Disconnected"));
        // Можно: socketToRow.remove(socket); — если не нужно держать в памяти
    }
}


