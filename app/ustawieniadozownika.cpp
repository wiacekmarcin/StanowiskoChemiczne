#include "ustawieniadozownika.h"
#include "ui_ustawieniadozownika.h"
#include <QDialogButtonBox>
#include <QRadioButton>
#include <QMessageBox>
#include <QPushButton>

#include "ustawienia.h"

#include <QDebug>

#define INIT_KIERUNEK(N) do { ui->rb_l_##N->setChecked(ustawienia.getReverse_##N());ui->rb_p_##N->setChecked(!ustawienia.getReverse_##N()); } while(false)

#define CONNECT_RB(N) do { connect(ui->rb_l_##N, &QRadioButton::clicked, this, &UstawieniaDozownika::on_rb_l_##N##_clicked); \
                        connect(ui->rb_p_##N, &QRadioButton::clicked, this, &UstawieniaDozownika::on_rb_p_##N##_clicked); } while(false)

UstawieniaDozownika::UstawieniaDozownika(Urzadzenia * urzadzenia, Ustawienia & ust, const UserPrivilige &user, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UstawieniaDozownika),
    ustawienia(ust),
    nrDozownika(0),
    urzadz(urzadzenia)
{
    ui->setupUi(this);
    connect(ui->buttonBox, &QDialogButtonBox::clicked, this, &UstawieniaDozownika::buttonBox_clicked);
    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &UstawieniaDozownika::buttonBox_accepted);
    connect(ui->cbDozownik, qOverload<int>(&QComboBox::currentIndexChanged), this, &UstawieniaDozownika::cbDozownik_currentIndexChanged);
    connect(ui->pbHomePosition, &QToolButton::clicked, this, &UstawieniaDozownika::pbHomePosition_clicked);
    connect(ui->pbCyklNapelniania, &QToolButton::clicked, this, &UstawieniaDozownika::pbCyklNapelniania_clicked);
    connect(ui->pbSetPosition, &QToolButton::clicked, this, &UstawieniaDozownika::pbSetPosition_clicked);
    connect(ui->pbDozujSteps, &QToolButton::clicked, this, &UstawieniaDozownika::pbDozujSteps_clicked);
    connect(ui->pbDozujMls, &QToolButton::clicked, this, &UstawieniaDozownika::pbDozujMls_clicked);

    void setPositionHomeDone(bool success);
    void setPositionDone(bool success);
    void setCykleDone(bool success);
    void setStepsDone(bool success);

    connect(urzadzenia, &Urzadzenia::setPositionHomeDone, this, &UstawieniaDozownika::setPositionHomeDone);
    connect(urzadzenia, &Urzadzenia::setPositionDone, this, &UstawieniaDozownika::setPositionDone);
    connect(urzadzenia, &Urzadzenia::setCykleDone, this, &UstawieniaDozownika::setCykleDone);
    connect(urzadzenia, &Urzadzenia::setStepsDone, this, &UstawieniaDozownika::setStepsDone);
    //connect(urzadzenia, &Urzadzenia::)


    CONNECT_RB(1);
    CONNECT_RB(2);
    CONNECT_RB(3);
    CONNECT_RB(4);
    CONNECT_RB(5);

    INIT_KIERUNEK(1);
    INIT_KIERUNEK(2);
    INIT_KIERUNEK(3);
    INIT_KIERUNEK(4);
    INIT_KIERUNEK(5);

    ui->ImpulsTime->setValue(ustawienia.getImpTime());
    ui->MaxSteps->setValue(ustawienia.getMaxImp());
    ui->steps_on_ml->setMin(0);
    ui->steps_on_ml->setMax(1000000);
    ui->steps_on_ml->setPrec(3);
    ui->steps_on_ml->setValue(ustawienia.getStepsOnMl());

    ui->position->setMin(0);
    ui->position->setPrec(0);
    ui->position->setMax(ustawienia.getMaxImp());
    ui->position->setValue(0);

    ui->steps->setMin(0);
    ui->steps->setPrec(0);
    ui->steps->setMax(10000000);
    ui->steps->setValue(0);

    ui->mls->setMin(0);
    ui->mls->setMax(1000);
    ui->mls->setPrec(1);
    ui->mls->setValue(0);

    ui->frame->setEnabled((user & U_SERVISANT) == U_SERVISANT);

    QPushButton * btn;
    btn = ui->buttonBox->button(QDialogButtonBox::RestoreDefaults);
    if (btn)
        btn->setEnabled((user & U_SERVISANT) == U_SERVISANT);

    btn = ui->buttonBox->button(QDialogButtonBox::Ok);
    if (btn)
        btn->setEnabled((user & U_SERVISANT) == U_SERVISANT);
}

UstawieniaDozownika::~UstawieniaDozownika()
{
    delete ui;
}

#define INIT_DIRECTION(N, D) do { ui->rb_l_##N->setChecked(D);ui->rb_p_##N->setChecked(!D); } while(false)
void UstawieniaDozownika::buttonBox_clicked(QAbstractButton *button)
{
    
    if ((QPushButton*)button == ui->buttonBox->button(QDialogButtonBox::RestoreDefaults))
    {
        INIT_DIRECTION(1, false);
        INIT_DIRECTION(2, false);
        INIT_DIRECTION(3, false);
        INIT_DIRECTION(4, false);
        INIT_DIRECTION(5, true);

        ui->MaxSteps->setValue(50000);
        ui->ImpulsTime->setValue(200);
        ui->steps_on_ml->setValue(14000.0);
    }
    if ((QPushButton*)button == ui->buttonBox->button(QDialogButtonBox::Apply))
    {
        save();
    }
}

void UstawieniaDozownika::buttonBox_accepted()
{
    save();
}

#define SAVE_REVERSE(N) do { ustawienia.setReverse_##N(ui->rb_l_##N->isChecked()); } while(false)
void UstawieniaDozownika::save()
{
    SAVE_REVERSE(1);
    SAVE_REVERSE(2);
    SAVE_REVERSE(3);
    SAVE_REVERSE(4);
    SAVE_REVERSE(5);
    ustawienia.setMaxImp(ui->MaxSteps->value());
    ustawienia.setImpTime(ui->ImpulsTime->value());
    ustawienia.setStepsOnMl(ui->steps_on_ml->value());
}

#define ON_RB_FUNCTION(N) \
void UstawieniaDozownika::on_rb_l_##N##_clicked() \
{\
    ui->rb_l_##N->setChecked(true);\
    ui->rb_p_##N->setChecked(false);\
}\
void UstawieniaDozownika::on_rb_p_##N##_clicked()\
{\
    ui->rb_l_##N->setChecked(false);\
    ui->rb_p_##N->setChecked(true);\
}


ON_RB_FUNCTION(1)
ON_RB_FUNCTION(2)
ON_RB_FUNCTION(3)
ON_RB_FUNCTION(4)
ON_RB_FUNCTION(5)

void UstawieniaDozownika::cbDozownik_currentIndexChanged(int index)
{
    nrDozownika = index;
}

void UstawieniaDozownika::pbHomePosition_clicked()
{
    ui->pbHomePosition->setEnabled(false);
    urzadz->setDozownikHome(nrDozownika);
}


void UstawieniaDozownika::pbCyklNapelniania_clicked()
{
    urzadz->setCykle(nrDozownika, ui->cyckle->value());
    ui->pbCyklNapelniania->setEnabled(false);
}

void UstawieniaDozownika::pbSetPosition_clicked()
{
    if (!ui->position->getValid())
        return;
    urzadz->setPosition(nrDozownika, ui->position->value());
    ui->pbSetPosition->setEnabled(false);
}

void UstawieniaDozownika::pbDozujSteps_clicked()
{
    if (!ui->steps->getValid())
        return;
    ui->pbDozujSteps->setEnabled(false);
    urzadz->setSteps(nrDozownika, ui->steps->value());

}

void UstawieniaDozownika::pbDozujMls_clicked()
{
    if (!ui->mls->getValid())
        return;
    if (!ui->steps_on_ml->getValid())
        return;

    ui->pbDozujMls->setEnabled(false);
    urzadz->setMl(nrDozownika, 10*ui->mls->value()*ui->steps_on_ml->value());
}

void UstawieniaDozownika::setCykleDone(bool success)
{
    ui->pbCyklNapelniania->setEnabled(true);
    if (!success) {
        QMessageBox::warning(this, "Napełnianie dozownika", "Nie udało się napełnić dozownika");
    }
}

void UstawieniaDozownika::setStepsDone(bool success)
{
    ui->pbDozujSteps->setEnabled(true);
    ui->pbDozujMls->setEnabled(true);
    if (!success) {
        QMessageBox::warning(this, "Dozowanie", "Nie udało się zadozować");
    }
}

void UstawieniaDozownika::setPositionHomeDone(bool success)
{
    ui->pbHomePosition->setEnabled(true);
    if (!success) {
        QMessageBox::warning(this, "Ustawienie pozycji startowej", "Nie udało się wrócić do pozycji bazowej");
    }
}

void UstawieniaDozownika::setPositionDone(bool success)
{
    ui->pbSetPosition->setEnabled(true);
    if (!success) {
        QMessageBox::warning(this, "Ustawienie pozycji", "Nie udało się ustawić pozycji");
    }
}

