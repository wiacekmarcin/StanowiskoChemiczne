#include "glowneokno.h"
#include <QApplication>
//#include <QWindowsStyle>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //a.setStyle("Windows");
    //QApplication::setStyle(QWindowsStyle());
    GlowneOkno w;
    w.show();

    return a.exec();
}
