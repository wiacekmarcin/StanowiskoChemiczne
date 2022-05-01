#ifndef CZUJNIKANALOGOWYUSTAWIENIAWIDGET_H
#define CZUJNIKANALOGOWYUSTAWIENIAWIDGET_H

#include <QWidget>
#include "ustawienia.h"

namespace Ui {
class CzujnikAnalogowyUstawieniaWidget;
}

class CzujnikAnalogowyUstawieniaWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CzujnikAnalogowyUstawieniaWidget(QWidget *parent = 0);
    ~CzujnikAnalogowyUstawieniaWidget();
    bool valid();

    void setData(const UserPrivilige &user, const Ustawienia::CzujnikAnalogowy &czA);

    Ustawienia::CzujnikAnalogowy getUstawienia() const;
private slots:
    void textChange();
signals:
    void updateCzujnik();

private:
    Ui::CzujnikAnalogowyUstawieniaWidget *ui;
    double rr;
};

#endif // CZUJNIKANALOGOWYUSTAWIENIAWIDGET_H
