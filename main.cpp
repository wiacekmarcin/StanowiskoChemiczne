#include "glowneokno.h"
#include <QApplication>
//#include <QWindowsStyle>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QCoreApplication::setOrganizationName("MySoft");
    QCoreApplication::setOrganizationDomain("mysoft.com");
    QCoreApplication::setApplicationName("Star Runner");
    //a.setStyle("Windows");
    //QApplication::setStyle(QWindowsStyle());
    GlowneOkno w;
    w.show();

    return a.exec();
}
