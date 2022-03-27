#include "glowneokno.h"
#include <QApplication>
//#include <QWindowsStyle>

#include "ustawienia.h"
#include "urzadzenia.h"

int main(int argc, char *argv[])
{
    qRegisterMetaType<uint16_t>("uint16_t");
    qRegisterMetaType<uint32_t>("uint32_t");
    QApplication a(argc, argv);
    QCoreApplication::setOrganizationName("Kolodziejczyk");
    //QCoreApplication::setOrganizationDomain("mysoft.com");
    QCoreApplication::setApplicationName("StanowiskoChemiczne");
    //a.setStyle("Windows");
    //QApplication::setStyle(QWindowsStyle());
    Ustawienia u;
    Urzadzenia urzadz(u);

    GlowneOkno w(u, &urzadz);
    w.show();

    return a.exec();

}

