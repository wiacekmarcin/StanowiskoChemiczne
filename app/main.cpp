#include "glowneokno.h"
#include <QApplication>
//#include <QWindowsStyle>


#include "ustawienia.h"
#include "urzadzenia.h"


#ifdef DEBUG_DEVICE
#include "urzadzenia_debug.h"
#endif
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

#if GUI_DEBUG
    UrzadzeniaDebug * dlg = new UrzadzeniaDebug;
    dlg->setLabels(u);
    dlg->setUrzadzenie(&urzadz);
    dlg->show();
#endif
    GlowneOkno w(u, &urzadz);
    w.showMaximized();

    return a.exec();

}

