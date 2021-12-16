#ifndef CZUJNIKANALOGOWYUSTAWIENIAOKNO_H
#define CZUJNIKANALOGOWYUSTAWIENIAOKNO_H

#include <QDialog>

namespace Ui {
class CzujnikAnalogowyUstawieniaOkno;
}

class CzujnikAnalogowyUstawieniaWidget;
class CzujnikAnalogowyUstawieniaOkno;

class CzujnikAnalogowyUstawieniaOkno : public QDialog
{
    Q_OBJECT

public:
    explicit CzujnikAnalogowyUstawieniaOkno(QWidget *parent = 0);
    ~CzujnikAnalogowyUstawieniaOkno();

    void setData(int i, const QString & name, const QString & unit, const double & ratio);
    void getData(CzujnikAnalogowyUstawieniaOkno * dlg);
private slots:
    void updateCzujnik();
private:
    Ui::CzujnikAnalogowyUstawieniaOkno *ui;
    CzujnikAnalogowyUstawieniaWidget * ustawienia[6];
};

#endif // CZUJNIKANALOGOWYUSTAWIENIAOKNO_H
