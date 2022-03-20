#include "dozowniksettings.h"
#include "ui_dozowniksettings.h"

#include <QRadioButton>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QDebug>

#include "serialmessage.h"

#define CONNECT_RB(N) connect(ui->rbL_##N, &QRadioButton::clicked, this, &DozownikSettings::on_rbL_##N##_clicked); \
                      connect(ui->rbP_##N, &QRadioButton::clicked, this, &DozownikSettings::on_rbP_##N##_clicked);

#define CONNECT_VALUE_CHANGE(N) connect(ui->sb_steps_##N, qOverload<int>(&QSpinBox::valueChanged), this, &DozownikSettings::on_sb_steps_##N##_valueChanged );\
                                connect(ui->sb_ml_##N, qOverload<double>(&QDoubleSpinBox::valueChanged), this, &DozownikSettings::on_sb_ml_##N##_valueChanged );

#define INITIAL_FORM(N) \
    ui->sb_ml_##N->setMinimum(0);\
    ui->sb_ml_##N->setMaximum(10);\
    ui->sb_ml_##N->setDecimals(2);\
    ui->sb_ml_##N->setSingleStep(0.01);\
    ui->sb_steps_##N->setMaximum(10000);\
    ui->sb_steps_##N->setMinimum(0);\
    ui->sb_steps_##N->setSingleStep(1); \
    ui->rbP_##N->setChecked(true);\
    nomodifysteps[N-1] = 2; \
    praweObr[N-1] = true; \
    steps[N-1] = 0;


DozownikSettings::DozownikSettings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DozownikSettings),
    ratio_steps2ml(50.0),
    NrDozownik(0),
    setHome(false)    

{
    ui->setupUi(this);

    INITIAL_FORM(1)
    INITIAL_FORM(2)
    INITIAL_FORM(3)
    INITIAL_FORM(4)
    INITIAL_FORM(5)

    CONNECT_RB(1)
    CONNECT_RB(2)
    CONNECT_RB(3)
    CONNECT_RB(4)
    CONNECT_RB(5)

    CONNECT_VALUE_CHANGE(1)
    CONNECT_VALUE_CHANGE(2)
    CONNECT_VALUE_CHANGE(3)
    CONNECT_VALUE_CHANGE(4)
    CONNECT_VALUE_CHANGE(5)


}

DozownikSettings::~DozownikSettings()
{
    delete ui;
}

#define FUN_ON_RP_CLICKED(N) void DozownikSettings::on_rbP_##N##_clicked() { praweObr[N-1] = true; srlmsg->setReverse##N(true); } \
                             void DozownikSettings::on_rbL_##N##_clicked() { praweObr[N-1] = false; srlmsg->setReverse##N(false); } \


FUN_ON_RP_CLICKED(1)
FUN_ON_RP_CLICKED(2)
FUN_ON_RP_CLICKED(3)
FUN_ON_RP_CLICKED(4)
FUN_ON_RP_CLICKED(5)

#define FUN_SB_VALUE_CHANGED(N) \
void DozownikSettings::on_sb_steps_##N##_valueChanged(int arg) \
{\
    if (nomodifysteps[N-1] > 0) { \
        nomodifysteps[N-1] -= 1; \
    }\
    if (nomodifysteps[N-1] == 0) {\
        return;\
    }\
    steps[N-1] = arg;\
    ui->sb_ml_##N->setValue(1.0*arg/ratio_steps2ml);\
    nomodifysteps[N-1] = 2;\
}\
void DozownikSettings::on_sb_ml_##N##_valueChanged(double arg)\
{\
    if (nomodifysteps[N-1] > 0) { \
        nomodifysteps[N-1] -= 1; \
    }\
    if (nomodifysteps[N-1] == 0) {\
        return;\
    }\
    steps[N-1] = round(arg*ratio_steps2ml);\
    ui->sb_steps_##N->setValue(steps[N-1]);\
    nomodifysteps[N-1] = 2;\
}

FUN_SB_VALUE_CHANGED(1)
FUN_SB_VALUE_CHANGED(2)
FUN_SB_VALUE_CHANGED(3)
FUN_SB_VALUE_CHANGED(4)
FUN_SB_VALUE_CHANGED(5)

#define FUN_ON_PB_CLICKED(N) \
void DozownikSettings::on_pb_home_##N##_clicked()\
{\
    setHome = true;\
    NrDozownik = N-1;\
    srlmsg->setReset();\
}\
void DozownikSettings::on_pb_set_##N##_clicked()\
{\
    setHome = false;\
    NrDozownik = N-1;\
    srlmsg->setReset();\
}

FUN_ON_PB_CLICKED(1)
FUN_ON_PB_CLICKED(2)
FUN_ON_PB_CLICKED(3)
FUN_ON_PB_CLICKED(4)
FUN_ON_PB_CLICKED(5)


void DozownikSettings::setSmg(SerialMessage * msg_)
{
    srlmsg = msg_;
    connect(srlmsg, &SerialMessage::resetDone, this, &DozownikSettings::resetDone);
    connect(srlmsg, &SerialMessage::setParamsDone, this, &DozownikSettings::setParamsDone);
    connect(srlmsg, &SerialMessage::dozownik, this, &DozownikSettings::dozownik);
    connect(srlmsg, &SerialMessage::errorSerial, this, &DozownikSettings::errorSerial);
    connect(srlmsg, &SerialMessage::debug, this, &DozownikSettings::debug);
    connect(srlmsg, &SerialMessage::donePositionHome, this, &DozownikSettings::donePositionHome);
    connect(srlmsg, &SerialMessage::donePosition, this, &DozownikSettings::donePosition);
    connect(srlmsg, &SerialMessage::successOpenDevice, this, &DozownikSettings::successOpenDevice);
}

void DozownikSettings::resetDone()
{
    srlmsg->setParameters();
}

void DozownikSettings::setParamsDone()
{
    if (setHome)
        srlmsg->setPositionHome(NrDozownik);
    else
        srlmsg->setPosition(NrDozownik, steps[NrDozownik]);
}

void DozownikSettings::dozownik(bool conn)
{
     debug(QString("Open Dozownik %1").arg(conn));
}

void DozownikSettings::errorSerial(QString  err)
{
    ui->errorTxt->appendPlainText(err);
}

void DozownikSettings::debug(QString dbg)
{
    ui->debugText->appendPlainText(dbg);
}

void DozownikSettings::donePositionHome(bool ok)
{
    ui->gb_1->setDisabled(false);
    ui->gb_2->setDisabled(false);
    ui->gb_3->setDisabled(false);
    ui->gb_4->setDisabled(false);
    ui->gb_5->setDisabled(false);
}

void DozownikSettings::donePosition()
{
    ui->gb_1->setDisabled(false);
    ui->gb_2->setDisabled(false);
    ui->gb_3->setDisabled(false);
    ui->gb_4->setDisabled(false);
    ui->gb_5->setDisabled(false);
}

void DozownikSettings::successOpenDevice(bool success)
{
    ui->gb_1->setDisabled(!success);
    ui->gb_2->setDisabled(!success);
    ui->gb_3->setDisabled(!success);
    ui->gb_4->setDisabled(!success);
    ui->gb_5->setDisabled(!success);
}

