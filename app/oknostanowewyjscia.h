#ifndef OKNOSTANOWEWYJSCIA_H
#define OKNOSTANOWEWYJSCIA_H

#include <QWidget>
#include <QMap>
#include "ustawienia.h"
namespace Ui {
class OknoStanoweWyjscia;
}

class StanWyjscia;

class OknoStanoweWyjscia : public QWidget
{
    Q_OBJECT

public:
    explicit OknoStanoweWyjscia(QWidget *parent = nullptr);
    ~OknoStanoweWyjscia();
    void setLabels(const Ustawienia & ust);
    void setOnOff(uint16_t mask);

signals:
    void writeValue(uint16_t mask, uint8_t value);
private:
    Ui::OknoStanoweWyjscia *ui;
    QMap<uint16_t, StanWyjscia*> wyjscia;
};

#endif // OKNOSTANOWEWYJSCIA_H
