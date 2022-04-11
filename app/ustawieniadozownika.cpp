#include "ustawieniadozownika.h"
#include "ui_ustawieniadozownika.h"
#include <QDialogButtonBox>
#include <QRadioButton>

#include "ustawienia.h"

#define INIT_KIERUNEK(N) do { ui->rb_l_##N->setChecked(ustawienia.getReverse_##N());ui->rb_p_##N->setChecked(!ustawienia.getReverse_##N()); } while(false)

#define CONNECT_RB(N) do { connect(ui->rb_l_##N, &QRadioButton::clicked, this, &UstawieniaDozownika::on_rb_l_##N##_clicked); \
                        connect(ui->rb_p_##N, &QRadioButton::clicked, this, &UstawieniaDozownika::on_rb_p_##N##_clicked); } while(false)

UstawieniaDozownika::UstawieniaDozownika(Ustawienia & ust, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UstawieniaDozownika),
    ustawienia(ust)
{
    ui->setupUi(this);
    connect(ui->buttonBox, &QDialogButtonBox::clicked, this, &UstawieniaDozownika::on_buttonBox_clicked);
    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &UstawieniaDozownika::on_buttonBox_accepted);

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
    ui->steps_on_ml->setValue(ustawienia.getStepsOnMl());
}

UstawieniaDozownika::~UstawieniaDozownika()
{
    delete ui;
}

#define INIT_DIRECTION(N, D) do { ui->rb_l_##N->setChecked(D);ui->rb_p_##N->setChecked(!D); } while(false)
void UstawieniaDozownika::on_buttonBox_clicked(QAbstractButton *button)
{
    //qDebug"%s:%d", __FILE__, __LINE__);
    if ((QPushButton*)button == ui->buttonBox->button(QDialogButtonBox::RestoreDefaults))
    {
        INIT_DIRECTION(1, false);
        INIT_DIRECTION(2, false);
        INIT_DIRECTION(3, false);
        INIT_DIRECTION(4, false);
        INIT_DIRECTION(5, true);

        ui->MaxSteps->setValue(50000);
        ui->ImpulsTime->setValue(200);
        ui->steps_on_ml->setValue(10000.0);
    }
    if ((QPushButton*)button == ui->buttonBox->button(QDialogButtonBox::Apply))
    {
        save();
    }
}

void UstawieniaDozownika::on_buttonBox_accepted()
{
    //qDebug"%s:%d", __FILE__, __LINE__);
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
