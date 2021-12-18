#include "nowytest_6.h"
#include "ui_nowytest_6.h"

void NowyTest_6::setReadOnly(QCheckBox* checkBox, bool readOnly)
{
   checkBox->setAttribute(Qt::WA_TransparentForMouseEvents, readOnly);
   checkBox->setFocusPolicy(readOnly ? Qt::NoFocus : Qt::StrongFocus);
   checkBox->setWindowFlags(checkBox->windowFlags() | Qt::WindowTransparentForInput);
}

NowyTest_6::NowyTest_6(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::NowyTest_6)
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

NowyTest_6::~NowyTest_6()
{
    delete ui;
}

bool NowyTest_6::isComplete() const
{
    if (wizard()->currentPage() != this)
        return true;

    if (!QWizardPage::isComplete())
        return false;

    return valid;
}

void NowyTest_6::on_pbStep1_clicked()
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
    ui->lStep3->setEnabled(false);
    ui->pbStep3->setEnabled(false);
}

void NowyTest_6::on_pbStep2_clicked()
{
    ui->pbStep2->setEnabled(false);
    ui->lStep3->setEnabled(true);
    ui->pbStep3->setEnabled(true);
}

void NowyTest_6::on_pbStep3_clicked()
{
    ui->pbStep3->setEnabled(false);
    valid = true;
    emit completeChanged();
}
