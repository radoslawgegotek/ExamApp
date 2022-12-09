#include "mainwindow.h"
#include "app.h"
#include "exam.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    App mainApp;

    MainWindow w(nullptr, &mainApp);
    w.show();

    return a.exec();
}
