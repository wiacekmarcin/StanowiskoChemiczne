#include "ustawieniadialog.h"
#include "ui_ustawieniadialog.h"

#include "czujnikianalogoweustawieniaframe.h"

#include <QDebug>

UstawieniaDialog::UstawieniaDialog(UserPrivilige & user, const Ustawienia & u, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UstawieniaDialog)
{
    ui->setupUi(this);
    CzujnikiAnalogoweUstawieniaFrame *frame = static_cast<CzujnikiAnalogoweUstawieniaFrame*>(ui->frame_cz_anal);
    frame->setUstawienia(user, u);
    resize(1800, 900);
}

UstawieniaDialog::~UstawieniaDialog()
{
    delete ui;
}

void UstawieniaDialog::saveData(Ustawienia &ust)
{
    CzujnikiAnalogoweUstawieniaFrame *frame = static_cast<CzujnikiAnalogoweUstawieniaFrame*>(ui->frame_cz_anal);
    frame->saveData(ust);
}
