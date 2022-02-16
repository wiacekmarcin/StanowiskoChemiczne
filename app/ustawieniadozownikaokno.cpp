#include "ustawieniadozownikaokno.h"
#include "ui_ustawieniadozownikaokno.h"

UstawieniaDozownikaOkno::UstawieniaDozownikaOkno(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UstawieniaDozownikaOkno)
{
    ui->setupUi(this);
}

UstawieniaDozownikaOkno::~UstawieniaDozownikaOkno()
{
    delete ui;
}
