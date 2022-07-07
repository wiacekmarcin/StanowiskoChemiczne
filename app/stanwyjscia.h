#ifndef STANWYJSCIA_H
#define STANWYJSCIA_H

#include <QWidget>
#include <QFrame>
#include <QTimer>
#include "ustawienia.h"
namespace Ui {
class StanWyjscia;
}

class StanWyjscia : public QFrame
{
    Q_OBJECT

public:
    explicit StanWyjscia(bool oneBtn = false, QWidget *parent = nullptr);
    ~StanWyjscia();
    void setMaskId(digitalOut mask, bool tBVis);
    void setLabel(const QString &name);

    void digitalWrite(uint16_t value);

signals:
    void writeValue(digitalOut mask, uint8_t value);


private slots:
    void TbOnclicked();
    void TbOffclicked();

protected:
    void setOnOff(bool on);
private:
    Ui::StanWyjscia *ui;
    bool m_on;
    digitalOut maskId;
    bool oneBtn;
    bool stateOn;
};

#endif // STANWYJSCIA_H
