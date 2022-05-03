#include "glowneokno.h"
#include <QApplication>
//#include <QWindowsStyle>


#include "ustawienia.h"
#include "urzadzenia.h"
#include "logowaniedlg.h"


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
    LogowanieDlg * dlg = new LogowanieDlg();
    if (dlg->exec() == QDialog::Rejected) {
        a.quit();
        return 0;
    }
    UserPrivilige user = dlg->getUser();
    delete dlg;

    Urzadzenia urzadz(u);
    GlowneOkno w(user, u, &urzadz);
    w.showMaximized();
    //w.showMinimized();

    return a.exec();

}

