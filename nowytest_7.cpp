#include "nowytest_7.h"
#include "ui_nowytest_7.h"

void NowyTest_7::setReadOnly(QCheckBox* checkBox, bool readOnly)
{
   checkBox->setAttribute(Qt::WA_TransparentForMouseEvents, readOnly);
   checkBox->setFocusPolicy(readOnly ? Qt::NoFocus : Qt::StrongFocus);
   checkBox->setWindowFlags(checkBox->windowFlags() | Qt::WindowTransparentForInput);
}

NowyTest_7::NowyTest_7(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::NowyTest_7)
{
    ui->setupUi(this);
    ui->cbZawor1->setCheckable(true);
    ui->cbZawor2->setCheckable(true);
    ui->cbZawor3->setCheckable(true);
    ui->cbZawor4->setCheckable(true);
    ui->cbZawor5->setCheckable(true);
    ui->cbZawor6->setCheckable(true);

    ui->cbZawor1->setChecked(false);
    ui->cbZawor2->setChecked(false);
    ui->cbZawor3->setChecked(false);
    ui->cbZawor4->setChecked(false);
    ui->cbZawor5->setChecked(false);
    ui->cbZawor6->setChecked(false);

    setReadOnly(ui->cbZawor1, true);
    setReadOnly(ui->cbZawor2, true);
    setReadOnly(ui->cbZawor3, true);
    setReadOnly(ui->cbZawor4, true);
    setReadOnly(ui->cbZawor5, true);
    setReadOnly(ui->cbZawor6, true);

    valid = false;
    ui->frame_2->setEnabled(false);
}

NowyTest_7::~NowyTest_7()
{
    delete ui;
}

void NowyTest_7::initializePage()
{
    ui->lZaplon->setText(field("Zaplon").toString());
}

bool NowyTest_7::isComplete() const
{
    if (wizard()->currentPage() != this)
        return true;

    if (!QWizardPage::isComplete())
        return false;

    return valid;
}

void NowyTest_7::on_pbStep1_clicked()
{
    ui->cbZawor1->setChecked(true);
    ui->cbZawor2->setChecked(true);
    ui->cbZawor3->setChecked(true);
    ui->cbZawor4->setChecked(true);
    ui->cbZawor5->setChecked(true);
    ui->cbZawor6->setChecked(true);
    ui->pbStep1->setEnabled(false);

    ui->frame_2->setEnabled(true);
    ui->pbStep1->setEnabled(false);
}



void NowyTest_7::on_pbStep2_clicked()
{
    valid = true;
    ui->pbStep2->setEnabled(false);
    emit completeChanged();
}
