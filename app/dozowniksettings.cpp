#include "dozowniksettings.h"
#include "ui_dozowniksettings.h"

#include <QRadioButton>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QDebug>
#include <math.h>

#include "serialdevice.h"

#define CONNECT_RB(N) connect(ui->rbL_##N, &QRadioButton::clicked, this, &DozownikSettings::on_rbL_##N##_clicked); \
                      connect(ui->rbP_##N, &QRadioButton::clicked, this, &DozownikSettings::on_rbP_##N##_clicked);

#define CONNECT_VALUE_CHANGE(N) connect(ui->sb_steps_##N, qOverload<int>(&QSpinBox::valueChanged), this, &DozownikSettings::on_sb_steps_##N##_valueChanged );\
                                connect(ui->sb_ml_##N, qOverload<double>(&QDoubleSpinBox::valueChanged), this, &DozownikSettings::on_sb_ml_##N##_valueChanged );

#define INITIAL_FORM(N) \
    ui->sb_ml_##N->setMinimum(0);\
    ui->sb_ml_##N->setMaximum(100);\
    ui->sb_ml_##N->setDecimals(2);\
    ui->sb_ml_##N->setSingleStep(0.01);\
    ui->sb_steps_##N->setMaximum(1000000);\
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
    setHome(false),
    onlySetParameters(false)

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

#define FUN_ON_RP_CLICKED(N) void DozownikSettings::on_rbP_##N##_clicked() { praweObr[N-1] = true;  /*srlmsg->setReverse##N(true);*/ } \
                             void DozownikSettings::on_rbL_##N##_clicked() { praweObr[N-1] = false; /*srlmsg->setReverse##N(false);*/ } \


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
    /*srlmsg->setReset(); */ setParamsDone();\
    setBigSteps(steps[N-1]);\
}\
void DozownikSettings::on_pb_set_##N##_clicked()\
{\
    setHome = false;\
    NrDozownik = N-1;\
    /*srlmsg->setReset(); */ setParamsDone();\
    setBigSteps(steps[N-1]);\
}

FUN_ON_PB_CLICKED(1)
FUN_ON_PB_CLICKED(2)
FUN_ON_PB_CLICKED(3)
FUN_ON_PB_CLICKED(4)
FUN_ON_PB_CLICKED(5)


void DozownikSettings::setSmg(SerialDevice * msg_)
{
    srlmsg = msg_;
    connect(srlmsg, &SerialDevice::resetDone, this, &DozownikSettings::resetDone);
    connect(srlmsg, &SerialDevice::setParamsDone, this, &DozownikSettings::setParamsDone);
    connect(srlmsg, &SerialDevice::dozownikConfigured, this, &DozownikSettings::dozownik);
    connect(srlmsg, &SerialDevice::errorSerial, this, &DozownikSettings::errorSerial);
    connect(srlmsg, &SerialDevice::debug, this, &DozownikSettings::debug);
    connect(srlmsg, &SerialDevice::setPositionHomeDone, this, &DozownikSettings::donePositionHome);
    connect(srlmsg, &SerialDevice::setPositionDone, this, &DozownikSettings::donePosition);
    connect(srlmsg, &SerialDevice::dozownikConfigured, this, &DozownikSettings::successOpenDevice);
}

void DozownikSettings::resetDone()
{
    srlmsg->setSettings(praweObr[0], praweObr[1], praweObr[2], praweObr[3], praweObr[4],
            ui->spMaxSteps->value(), ui->spImpTime->value());
}

void DozownikSettings::setParamsDone()
{
    if (onlySetParameters) {
        onlySetParameters = false;
        return;
    }
    if (runTest)
        return;

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
    //ui->errorTxt->appendPlainText(err);
}

void DozownikSettings::debug(QString dbg)
{
    ui->debugText->appendPlainText(dbg);
}

void DozownikSettings::donePositionHome(bool ok)
{
    debug(QString("Pozycjonowanie zakonczone %1").arg(ok ? "sukcesem" : "porażką"));
    ui->gb_1->setDisabled(false);
    ui->gb_2->setDisabled(false);
    ui->gb_3->setDisabled(false);
    ui->gb_4->setDisabled(false);
    ui->gb_5->setDisabled(false);
    if (runTest) {
        //int uiSteps = srlmsg->getRSteps();
        //ui->rsteps->setText(QString("%1").arg(ui->rsteps->text().toInt() + uiSteps));
        nastepnyCykl();
    }
}

void DozownikSettings::donePosition()
{
    ui->gb_1->setDisabled(false);
    ui->gb_2->setDisabled(false);
    ui->gb_3->setDisabled(false);
    ui->gb_4->setDisabled(false);
    ui->gb_5->setDisabled(false);
    if (runTest) {
        srlmsg->setPositionHome(NrDozownik);
    }
}

void DozownikSettings::successOpenDevice(bool open, bool conf)
{
    bool success = open && conf;
    ui->gb_1->setDisabled(!success);
    ui->gb_2->setDisabled(!success);
    ui->gb_3->setDisabled(!success);
    ui->gb_4->setDisabled(!success);
    ui->gb_5->setDisabled(!success);
}


void DozownikSettings::on_pbSetParameters_clicked()
{
    ratio_steps2ml = 1.0/ui->spmlonsteps->value();
    onlySetParameters = true;
    srlmsg->setMaxImp(ui->spMaxSteps->value());
    srlmsg->setImpTime(ui->spImpTime->value());
    //srlmsg->setSettings5(ui->rbP_1->isChecked(), ui->rbP_2->isChecked(), ui->rbP_3->isChecked(),
    //                      ui->rbP_4->isChecked(), ui->rbP_5->isChecked(), ui->spMaxSteps->value(),
    //                      ui->spImpTime->value());
    srlmsg->setSettings(false, false, false, false, true, ui->spMaxSteps->value(), ui->spImpTime->value());
}


void DozownikSettings::on_pushButton_clicked()
{
    nrCykl = 0;
    runTest = true;
    ui->rsteps->setText(0);
    srlmsg->setSettings(ui->rbP_1->isChecked(), ui->rbP_2->isChecked(), ui->rbP_3->isChecked(),
                          ui->rbP_4->isChecked(), ui->rbP_5->isChecked(), ui->spMaxSteps->value(),
                          ui->spImpTime->value());
    srlmsg->setPositionHome(0);
    //nastepnyCykl();

}

void DozownikSettings::setBigSteps(unsigned long steps) {
    runTest = true;
    restSteps = steps;
    srlmsg->setPositionHome(NrDozownik);
}

void DozownikSettings::nastepnyCykl()
{
    qDebug("%s:%d nastepny cykl %d", __FILE__, __LINE__, restSteps);

    if (restSteps > 0) {
        runTest = true;
        uint32_t st = restSteps > 50000 ? 50000 : restSteps;
        srlmsg->setPosition(NrDozownik, st);
        restSteps -= st;
    } else {
        runTest = false;
    }

}
/*
void DozownikSettings::nastepnyCykl()
{
    ++nrCykl;
    qDebug("%s:%d nastepny cykl %d", __FILE__, __LINE__, nrCykl);

    if (nrCykl <= 5) {
        runTest = true;
        srlmsg->setPosition(0,50000);
    } else {
        runTest = false;
    }

}
*/

void DozownikSettings::on_pbStopCykle_clicked()
{
    runTest = false;
    nrCykl = 0;
    srlmsg->setReset();
}


void DozownikSettings::on_spmlonsteps_valueChanged(int arg1)
{
    ratio_steps2ml = arg1;
}

