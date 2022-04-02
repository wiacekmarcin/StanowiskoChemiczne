#include "stanwyjscia.h"
#include "ui_stanwyjscia.h"
#include <QToolButton>
StanWyjscia::StanWyjscia(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::StanWyjscia)
{
    ui->setupUi(this);
    m_on = false;
    setOnOff(false);
    connect(ui->tb_on, &QToolButton::clicked, this, &StanWyjscia::TbOnclicked);
    connect(ui->tb_off, &QToolButton::clicked, this, &StanWyjscia::TbOffclicked);
}

StanWyjscia::~StanWyjscia()
{
    delete ui;
}

void StanWyjscia::setMaskId(uint16_t mask, bool tBVis)
{

    maskId = mask;
    ui->tb_on->setVisible(tBVis);
    ui->tb_off->setVisible(tBVis);
}

void StanWyjscia::setLabel(const QString &name)
{
    ui->label->setText(name);
}

void StanWyjscia::digitalWrite(uint16_t value)
{
    bool setVal = (value & maskId) != 0;
    if (setVal != m_on) {
        setOnOff(setVal);
    }
}

void StanWyjscia::TbOnclicked()
{
    setOnOff(true);
    emit writeValue(maskId, 1);
}

void StanWyjscia::TbOffclicked()
{
    setOnOff(false);
    emit writeValue(maskId, 0);
}

void StanWyjscia::setOnOff(bool on)
{
    m_on = on;
    ui->tb_on->setEnabled(!on);
    ui->tb_off->setEnabled(on);

    ui->pic_on->setVisible(on);
    ui->pic_off->setVisible(!on);


}

