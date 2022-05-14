#include "sygnalycyfroweustawieniadialog.h"
#include "ui_sygnalycyfroweustawieniadialog.h"
#include <QPushButton>
#include <QDebug>

SygnalyCyfroweUstawieniaDialog::SygnalyCyfroweUstawieniaDialog(Ustawienia &ust, const UserPrivilige &user, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SygnalyCyfroweUstawieniaDialog),
    ustawienia(ust)
{
    ui->setupUi(this);
    ui->frame->setLabels(ust, user);
    ui->frame->setEnabled((user & U_ADMIN) == U_ADMIN);

    connect(ui->buttonBox, &QDialogButtonBox::clicked, this, &SygnalyCyfroweUstawieniaDialog::on_buttonBox_clicked);
    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &SygnalyCyfroweUstawieniaDialog::on_buttonBox_accepted);

    QPushButton * btn;
    btn = ui->buttonBox->button(QDialogButtonBox::RestoreDefaults);
    qDebug() << btn;
    if (btn)
        btn->setEnabled((user & U_ADMIN) == U_ADMIN);
    btn = ui->buttonBox->button(QDialogButtonBox::Ok);
        qDebug() << btn;
    if (btn)
        btn->setEnabled((user & U_ADMIN) == U_ADMIN);

    connect(ui->frame, &SygnalyCyfroweUstawienia::writeValue, this, &SygnalyCyfroweUstawieniaDialog::writeValue);
}

SygnalyCyfroweUstawieniaDialog::~SygnalyCyfroweUstawieniaDialog()
{
    disconnect(ui->frame, nullptr, nullptr, nullptr);
    delete ui;
}

void SygnalyCyfroweUstawieniaDialog::setOnOff(uint16_t mask)
{
    ui->frame->setOnOff(mask);
}

void SygnalyCyfroweUstawieniaDialog::on_buttonBox_clicked(QAbstractButton *button)
{

    if ((QPushButton*)button == ui->buttonBox->button(QDialogButtonBox::RestoreDefaults))
    {
        ui->frame->restoreDefaults();
    }
    if ((QPushButton*)button == ui->buttonBox->button(QDialogButtonBox::Apply))
    {
        save();
    }
}

void SygnalyCyfroweUstawieniaDialog::on_buttonBox_accepted()
{
    save();
}


void SygnalyCyfroweUstawieniaDialog::save()
{
    ui->frame->save(ustawienia);
}
