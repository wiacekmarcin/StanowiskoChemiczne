#include "sygnalycyfroweustawieniadialog.h"
#include "ui_sygnalycyfroweustawieniadialog.h"

SygnalyCyfroweUstawieniaDialog::SygnalyCyfroweUstawieniaDialog(Ustawienia &ust, const UserPrivilige &user, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SygnalyCyfroweUstawieniaDialog)
{
    ui->setupUi(this);
    ui->frame->setLabels(ust, user);
    ui->frame->setEnabled((user & U_SERVISANT) == U_SERVISANT);

}

SygnalyCyfroweUstawieniaDialog::~SygnalyCyfroweUstawieniaDialog()
{
    delete ui;
}
