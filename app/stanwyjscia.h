#ifndef STANWYJSCIA_H
#define STANWYJSCIA_H

#include <QWidget>
#include <QFrame>

namespace Ui {
class StanWyjscia;
}

class StanWyjscia : public QFrame
{
    Q_OBJECT

public:
    explicit StanWyjscia(QWidget *parent = nullptr);
    ~StanWyjscia();
    void setMaskId(uint16_t mask, bool tBVis);
    void setLabel(const QString &name);

    void digitalWrite(uint16_t value);

signals:
    void writeValue(uint16_t mask, uint8_t value);


private slots:
    void TbOnclicked();
    void TbOffclicked();

protected:
    void setOnOff(bool on);
private:
    Ui::StanWyjscia *ui;
    bool m_on;
    uint16_t maskId;
};

#endif // STANWYJSCIA_H
