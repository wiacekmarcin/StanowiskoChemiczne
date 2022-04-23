#include "mainwindow.h"

#include <QApplication>
#include "testdata.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    TestData t;
    w.show();
    return a.exec();
}
