#include "ustawieniadozownika.h"
#include "ui_ustawieniadozownika.h"
#include <QDialogButtonBox>

UstawieniaDozownika::UstawieniaDozownika(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UstawieniaDozownika)
{
    ui->setupUi(this);
    connect(ui->buttonBox, &QDialogButtonBox::clicked, this, &UstawieniaDozownika::on_buttonBox_clicked);
    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &UstawieniaDozownika::on_buttonBox_accepted);
}

UstawieniaDozownika::~UstawieniaDozownika()
{
    delete ui;
}


#define INIT_DIRECTION(N) do { ui->rb_l_##N->setChecked(true);ui->rb_p_##N->setChecked(false); } while(false)
void UstawieniaDozownika::on_buttonBox_clicked(QAbstractButton *button)
{
    if ((QPushButton*)button == ui->buttonBox->button(QDialogButtonBox::RestoreDefaults))
    {
        INIT_DIRECTION(1);
        INIT_DIRECTION(2);
        INIT_DIRECTION(3);
        INIT_DIRECTION(4);
        INIT_DIRECTION(5);

        ui->MaxSteps->setValue(50000);
        ui->ImpulsTime->setValue(200);
    }
}

void UstawieniaDozownika::on_buttonBox_accepted()
{

}

