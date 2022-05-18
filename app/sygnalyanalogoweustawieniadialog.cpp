#include "sygnalyanalogoweustawieniadialog.h"
#include "ui_sygnalyanalogoweustawieniadialog.h"

#include "czujnikianalogoweustawieniaframe.h"
#include <QPushButton>

#include <QDebug>

SygnalyAnalogowyUstawieniaDialog::SygnalyAnalogowyUstawieniaDialog(UserPrivilige & user, Ustawienia & u, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SygnalyAnalogowyUstawieniaDialog), ust(u)
{
    ui->setupUi(this);
    connect(ui->buttonBox, &QDialogButtonBox::clicked, this, &SygnalyAnalogowyUstawieniaDialog::on_buttonBox_clicked);
    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &SygnalyAnalogowyUstawieniaDialog::on_buttonBox_accepted);

    CzujnikiAnalogoweUstawieniaFrame *frame = static_cast<CzujnikiAnalogoweUstawieniaFrame*>(ui->frame_cz_anal);
    frame->setUstawienia(user, u);
    resize(1800, 900);
    QPushButton * btn;
    btn = ui->buttonBox->button(QDialogButtonBox::RestoreDefaults);
    if (btn)
        btn->setEnabled((user & U_ADMIN) == U_ADMIN);

    btn = ui->buttonBox->button(QDialogButtonBox::Ok);
    if (btn)
        btn->setEnabled((user & U_ADMIN) == U_ADMIN);
}

SygnalyAnalogowyUstawieniaDialog::~SygnalyAnalogowyUstawieniaDialog()
{
    delete ui;
}

void SygnalyAnalogowyUstawieniaDialog::save()
{
    CzujnikiAnalogoweUstawieniaFrame *frame = static_cast<CzujnikiAnalogoweUstawieniaFrame*>(ui->frame_cz_anal);
    frame->saveData(ust);
}

void SygnalyAnalogowyUstawieniaDialog::on_buttonBox_accepted()
{
    save();
}

void SygnalyAnalogowyUstawieniaDialog::on_buttonBox_clicked(QAbstractButton *button)
{
    if ((QPushButton*)button == ui->buttonBox->button(QDialogButtonBox::RestoreDefaults))
    {
        CzujnikiAnalogoweUstawieniaFrame *frame = static_cast<CzujnikiAnalogoweUstawieniaFrame*>(ui->frame_cz_anal);
        frame->saveData(ust);
    }
    if ((QPushButton*)button == ui->buttonBox->button(QDialogButtonBox::Apply))
    {
        save();
    }
}
