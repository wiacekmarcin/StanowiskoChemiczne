#ifndef SYGNALANALOGOWYUSTAWIENIA_H
#define SYGNALANALOGOWYUSTAWIENIA_H

#include <QWidget>
#include "ustawienia.h"

namespace Ui {
class SygnalAnalogowyUstawienia;
}

class SygnalAnalogowyUstawienia : public QWidget
{
    Q_OBJECT

public:
    explicit SygnalAnalogowyUstawienia(QWidget *parent = 0);
    ~SygnalAnalogowyUstawienia();
    bool valid();

    void setData(const UserPrivilige &user, const Ustawienia::CzujnikAnalogowy &czA);

    Ustawienia::CzujnikAnalogowy getUstawienia() const;
private slots:
    void textChange();
signals:
    void updateCzujnik();

private:
    Ui::SygnalAnalogowyUstawienia *ui;
    double rr;
};

#endif // SYGNALANALOGOWYUSTAWIENIA_H
