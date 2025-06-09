#include <QCoreApplication>
#include "server.h"  // подключаем наш класс сервера

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);  // создаём консольное приложение

    Server server;          // создаём сервер
    server.startServer(1234); // запускаем его на порту 1234

    return a.exec();        // запускаем цикл событий Qt
}
