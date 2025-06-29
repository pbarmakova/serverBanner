//Точка входа приложения, запускает Qt-приложение и главное окно (MainWindow).

#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
