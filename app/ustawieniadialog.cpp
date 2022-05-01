#include "ustawieniadialog.h"
#include "ui_ustawieniadialog.h"

#include "czujnikianalogoweustawieniaframe.h"

UstawieniaDialog::UstawieniaDialog(UserPrivilige & user, const Ustawienia & u, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UstawieniaDialog)
{
    ui->setupUi(this);
    CzujnikiAnalogoweUstawieniaFrame *frame = static_cast<CzujnikiAnalogoweUstawieniaFrame*>(ui->frame_cz_anal);
    frame->setUstawienia(user, u);
}

UstawieniaDialog::~UstawieniaDialog()
{
    delete ui;
}
