#ifndef CZUJNIKANALOGOWYUSTAWIENIAOKNO_H
#define CZUJNIKANALOGOWYUSTAWIENIAOKNO_H

#include "ustawienia.h"

#include <QDialog>

namespace Ui {
class CzujnikAnalogowyUstawieniaOkno;
}

class QDialogButtonBox;
class CzujnikAnalogowyUstawieniaWidget;
class CzujnikAnalogowyUstawieniaOkno;
class QGridLayout;

class CzujnikAnalogowyUstawieniaOkno : public QDialog
{
    Q_OBJECT

public:
    explicit CzujnikAnalogowyUstawieniaOkno(const Ustawienia & sett, QWidget *parent = 0);
    ~CzujnikAnalogowyUstawieniaOkno();

    void saveData(Ustawienia & ust);

private slots:
    void updateCzujnik();

protected:
    void createOneElement(int id, const QString &titleBox)    ;
private:
    Ui::CzujnikAnalogowyUstawieniaOkno *ui;
    CzujnikAnalogowyUstawieniaWidget * czujniki[Ustawienia::maxCzujek];
    QDialogButtonBox *buttonBox;
    QGridLayout * gridLayout;
};

#endif // CZUJNIKANALOGOWYUSTAWIENIAOKNO_H
