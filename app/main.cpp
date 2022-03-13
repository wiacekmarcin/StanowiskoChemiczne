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
    qDebug("%s %d",__FILE__,__LINE__);
    GlowneOkno w;
    qDebug("%s %d",__FILE__,__LINE__);
    w.show();

    return a.exec();

}

