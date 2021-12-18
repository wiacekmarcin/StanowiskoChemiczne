#include "nowytest_5.h"
#include "ui_nowytest_5.h"

void NowyTest_5::setReadOnly(QCheckBox* checkBox, bool readOnly)
{
   checkBox->setAttribute(Qt::WA_TransparentForMouseEvents, readOnly);
   checkBox->setFocusPolicy(readOnly ? Qt::NoFocus : Qt::StrongFocus);
   checkBox->setWindowFlags(checkBox->windowFlags() | Qt::WindowTransparentForInput);
}


NowyTest_5::NowyTest_5(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::NowyTest_5)
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

    ui->frame_2->setEnabled(false);

    valid = false;


}

void NowyTest_5::initializePage()
{
    ui->lciecz->setText(field("Ciecz").toString());
    ui->ldozownik->setText(field("Dozownik").toString());
    ui->lobjetosc->setText(field("Ilosc").toString());
}

NowyTest_5::~NowyTest_5()
{
    delete ui;
}

bool NowyTest_5::isComplete() const
{
    if (wizard()->currentPage() != this)
        return true;

    if (!QWizardPage::isComplete())
        return false;

    return valid;
}

void NowyTest_5::on_pbStep1_clicked()
{
    ui->cbZawor1->setChecked(true);
    ui->cbZawor2->setChecked(true);
    ui->cbZawor3->setChecked(true);
    ui->cbZawor4->setChecked(true);
    ui->cbZawor5->setChecked(true);
    ui->cbZawor6->setChecked(true);
    ui->pbStep1->setEnabled(false);

    ui->frame_2->setEnabled(true);

    ui->pbStep3OK->setEnabled(false);
    ui->lStep3->setEnabled(false);
    ui->lStep4->setEnabled(false);
    ui->pbStep4OK->setEnabled(false);

}

void NowyTest_5::on_pbStep2OK_clicked()
{
    ui->pbStep2OK->setEnabled(false);
    ui->pbStep3OK->setEnabled(true);
    ui->lStep3->setEnabled(true);
}

void NowyTest_5::on_pbStep3OK_clicked()
{
    ui->pbStep3OK->setEnabled(false);
    if (field("Podcisnienie").toBool()) {
        ui->lStep4->setEnabled(true);
        ui->pbStep4OK->setEnabled(true);
    } else {
        valid = true;
        emit completeChanged();
    }
}

void NowyTest_5::on_pbStep4OK_clicked()
{
    ui->pbStep4OK->setEnabled(false);
    valid = true;
    emit completeChanged();
}
