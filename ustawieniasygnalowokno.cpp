#include "ustawieniasygnalowokno.h"
#include "ui_ustawieniasygnalowokno.h"

UstawieniaSygnalowOkno::UstawieniaSygnalowOkno(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UstawieniaSygnalowOkno)
{
    ui->setupUi(this);
}

UstawieniaSygnalowOkno::~UstawieniaSygnalowOkno()
{
    delete ui;
}
