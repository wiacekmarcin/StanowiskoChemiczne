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
    qDebug("%s:%d", __FILE__, __LINE__);
    if ((QPushButton*)button == ui->buttonBox->button(QDialogButtonBox::RestoreDefaults))
    {
        ui->sBCykle->setValue(10);
    }
    if ((QPushButton*)button == ui->buttonBox->button(QDialogButtonBox::Apply))
    {
        save();
    }
}

void UstawieniaTestu::on_buttonBox_accepted()
{
    qDebug("%s:%d", __FILE__, __LINE__);
    save();
}

void UstawieniaTestu::save()
{
    ustawienia.setNrInitializeCycles(ui->sBCykle->value());
}
