#include "urzadzenia.h"
#include "ui_urzadzenia.h"

Urzadzenia::Urzadzenia(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Urzadzenia)
{
    ui->setupUi(this);

}

Urzadzenia::~Urzadzenia()
{
    delete ui;
}
