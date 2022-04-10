#ifndef CZUJNIKANALOGOWYUSTAWIENIAOKNO_H
#define CZUJNIKANALOGOWYUSTAWIENIAOKNO_H

#include "ustawienia.h"

#include <QDialog>
#include <QTimer>

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
    CzujnikAnalogowyUstawieniaWidget * czujniki[Ustawienia::maxCzujekAnal];
    QDialogButtonBox *buttonBox;
    QGridLayout * gridLayout;
    QTimer t;
};

#endif // CZUJNIKANALOGOWYUSTAWIENIAOKNO_H
