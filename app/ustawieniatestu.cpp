#include "ustawieniatestu.h"
#include "ui_ustawieniatestu.h"
#include <QAbstractButton>

UstawieniaTestu::UstawieniaTestu(Ustawienia & ust, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UstawieniaTestu),
    ustawienia(ust)
{
    ui->setupUi(this);
    connect(ui->buttonBox, &QDialogButtonBox::clicked, this, &UstawieniaTestu::on_buttonBox_clicked);
    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &UstawieniaTestu::on_buttonBox_accepted);
}

UstawieniaTestu::~UstawieniaTestu()
{
    delete ui;
}

void UstawieniaTestu::on_buttonBox_clicked(QAbstractButton *button)
{
    
    if ((QPushButton*)button == ui->buttonBox->button(QDialogButtonBox::RestoreDefaults))
    {
        ui->sBCykle->setValue(10);
        ui->prozniaVal->setValue(0.01);
    }
    if ((QPushButton*)button == ui->buttonBox->button(QDialogButtonBox::Apply))
    {
        save();
    }
}

void UstawieniaTestu::on_buttonBox_accepted()
{
    
    save();
}

void UstawieniaTestu::save()
{
    ustawienia.setNrInitializeCycles(ui->sBCykle->value());
    ustawienia.setCisnienieProzni(ui->prozniaVal->value());
}
