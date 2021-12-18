#include "nowytest_4.h"
#include "ui_nowytest_4.h"
#include <QCheckBox>
//#include "windows.h"
#include <QTimer>

void NowyTest_4::setReadOnly(QCheckBox* checkBox, bool readOnly)
{
   checkBox->setAttribute(Qt::WA_TransparentForMouseEvents, readOnly);
   checkBox->setFocusPolicy(readOnly ? Qt::NoFocus : Qt::StrongFocus);
   checkBox->setWindowFlags(checkBox->windowFlags() | Qt::WindowTransparentForInput);
}

NowyTest_4::NowyTest_4(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::NowyTest_4)
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
    ui->lStep2B->setEnabled(false);

    ui->lstep2A->setEnabled(false);
    ui->pbStep2A_OK->setEnabled(false);
    ui->pbStep2A_Next->setEnabled(false);

    ui->pbStep2B_OK->setEnabled(false);

    ui->pbStep2_100->setEnabled(false);
    ui->pbStep2_Skip->setEnabled(false);
    ui->pbStep2_OK->setEnabled(false);
    ui->lstep2->setEnabled(false);

    registerField("Podcisnienie", this, "pracaPompy", "pracaPompyChanged");
    setField("Podcisnienie", QVariant::fromValue(false));
    cnt = 0;
}

NowyTest_4::~NowyTest_4()
{
    delete ui;
}

bool NowyTest_4::isComplete() const
{
    if (wizard()->currentPage() != this)
        return true;

    if (!QWizardPage::isComplete())
        return false;

    return valid;
}

void NowyTest_4::on_pbStep1_clicked()
{
    ui->cbZawor1->setChecked(true);
    ui->cbZawor2->setChecked(true);
    ui->cbZawor3->setChecked(true);
    ui->cbZawor4->setChecked(true);
    ui->cbZawor5->setChecked(true);

    if (! cnt++)
        return;

    ui->cbZawor6->setChecked(true);
    ui->pbStep1->setEnabled(false);

    ui->pbStep2_100->setEnabled(true);
    ui->pbStep2_OK->setEnabled(true);
    ui->pbStep2_Skip->setEnabled(true);
    ui->lstep2->setEnabled(true);

}

void NowyTest_4::on_pbStep2_100_clicked()
{
    ui->lStep2B->setEnabled(true);
    ui->pbStep2B_OK->setEnabled(true);

    ui->pbStep2_100->setEnabled(false);
    ui->pbStep2_OK->setEnabled(false);
    ui->pbStep2_Skip->setEnabled(false);

    setField("Podcisnienie", QVariant::fromValue(true));
}

void NowyTest_4::on_pbStep2_OK_clicked()
{
    ui->lstep2A->setEnabled(true);
    ui->pbStep2A_OK->setEnabled(true);
    ui->pbStep2A_Next->setEnabled(true);

    ui->pbStep2_100->setEnabled(false);
    ui->pbStep2_OK->setEnabled(false);
    ui->pbStep2_Skip->setEnabled(false);
}

void NowyTest_4::on_pbStep2_Skip_clicked()
{
    ui->pbStep2_100->setEnabled(false);
    ui->pbStep2_OK->setEnabled(false);
    ui->pbStep2_Skip->setEnabled(false);

    valid = true;
    emit completeChanged();
}

void NowyTest_4::on_pbStep2A_OK_clicked()
{
    ui->pbStep2A_OK->setEnabled(false);
    ui->pbStep2A_Next->setEnabled(false);
    valid = true;
    emit completeChanged();
}

void NowyTest_4::on_pbStep2A_Next_clicked()
{
    ui->pbStep2A_OK->setEnabled(false);
    ui->pbStep2A_Next->setEnabled(false);
    QTimer::singleShot(1000,this, &NowyTest_4::runDone3);
}

void NowyTest_4::on_pbStep2B_OK_clicked()
{
    ui->pbStep2B_OK->setEnabled(false);
    QTimer::singleShot(1000,this, &NowyTest_4::runDone4);
}

void NowyTest_4::runDone3()
{
    ui->pbStep2A_OK->setEnabled(true);
    ui->pbStep2A_Next->setEnabled(true);
}

void NowyTest_4::runDone4()
{
    //ui->pbStep2B_OK->setEnabled(true);
    valid = true;
    emit completeChanged();
}



