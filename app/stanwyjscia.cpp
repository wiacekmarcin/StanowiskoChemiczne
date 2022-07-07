#include "stanwyjscia.h"
#include "ui_stanwyjscia.h"
#include <QToolButton>
StanWyjscia::StanWyjscia(bool oneBtn_, QWidget *parent) :
    QFrame(parent),
    ui(new Ui::StanWyjscia),
    oneBtn(oneBtn_),
    stateOn(false)
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

void StanWyjscia::setMaskId(digitalOut mask, bool tBVis)
{

    maskId = mask;
    if (!oneBtn) {
        ui->tb_on->setVisible(tBVis);
        ui->tb_off->setVisible(tBVis);
    } else {
        ui->tb_on->setVisible(tBVis);
        ui->tb_off->setVisible(false);
    }
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
    if (oneBtn) {
        stateOn = !stateOn;
        setOnOff(stateOn);
        emit writeValue(maskId, stateOn ? 1 : 0);
    } else {
        setOnOff(true);
        emit writeValue(maskId, 1);
    }
}

void StanWyjscia::TbOffclicked()
{
    setOnOff(false);
    emit writeValue(maskId, 0);
}

void StanWyjscia::setOnOff(bool on)
{
    m_on = on;
    if (oneBtn) {
        ui->tb_on->setText(stateOn ? "OFF" : "ON");
    } else {
        ui->tb_on->setEnabled(!on);
        ui->tb_off->setEnabled(on);
    }
    ui->pic_on->setVisible(on);
    ui->pic_off->setVisible(!on);


}

