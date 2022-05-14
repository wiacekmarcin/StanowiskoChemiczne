#ifndef SYGNALYCYFROWEUSTAWIENIA_H
#define SYGNALYCYFROWEUSTAWIENIA_H

#include "ustawienia.h"

#include <QFrame>
#include <QMap>
#include <QAbstractButton>


class StanWyjscia;
class QLineEdit;

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

    void restoreDefaults();

    void save(Ustawienia & ust);
signals:
    void writeValue(digitalOut mask, uint8_t value);
protected:
    void setWejscie(digitalIn id, const QString & name);
    QString wejscie(digitalIn id) const;
    void setWyjscie(digitalOut id, const QString & name);
    QString wyjscie(digitalOut id) const;


private:
    Ui::SygnalyCyfroweUstawienia *ui;
    QMap<uint16_t, StanWyjscia*> wyjscia;
    QMap<digitalIn, QLineEdit *> posIn;
    QMap<digitalOut, QLineEdit *> posOut;
};

#endif // SYGNALYCYFROWEUSTAWIENIA_H
