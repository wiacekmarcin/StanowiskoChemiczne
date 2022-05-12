#ifndef SYGNALYCYFROWEUSTAWIENIA_H
#define SYGNALYCYFROWEUSTAWIENIA_H

#include "ustawienia.h"

#include <QFrame>
#include <QMap>

class StanWyjscia;

namespace Ui {
class SygnalyCyfroweUstawienia;
}

class SygnalyCyfroweUstawienia : public QFrame
{
    Q_OBJECT

public:
    explicit SygnalyCyfroweUstawienia(QWidget *parent = nullptr);
    ~SygnalyCyfroweUstawienia();
    void setLabels(const Ustawienia & ust, const UserPrivilige &user);
    void setOnOff(uint16_t mask);

signals:
    void writeValue(digitalOut mask, uint8_t value);

private:
    Ui::SygnalyCyfroweUstawienia *ui;
    QMap<uint16_t, StanWyjscia*> wyjscia;
};

#endif // SYGNALYCYFROWEUSTAWIENIA_H
