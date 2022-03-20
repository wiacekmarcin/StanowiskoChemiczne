#include "glowneokno.h"
#include <QApplication>
//#include <QWindowsStyle>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QCoreApplication::setOrganizationName("Kolodziejczyk");
    //QCoreApplication::setOrganizationDomain("mysoft.com");
    QCoreApplication::setApplicationName("StanowiskoChemiczne");
    //a.setStyle("Windows");
    //QApplication::setStyle(QWindowsStyle());
    GlowneOkno w;
    w.show();

    return a.exec();

}

