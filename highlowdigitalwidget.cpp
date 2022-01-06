#include "highlowdigitalwidget.h"
#include "ui_highlowdigitalwidget.h"

HighLowDigitalWidget::HighLowDigitalWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HighLowDigitalWidget)
{
    ui->setupUi(this);
    ui->low->setChecked(true);
}

HighLowDigitalWidget::~HighLowDigitalWidget()
{
    delete ui;
}

void HighLowDigitalWidget::setLowText(const QString &low)
{
    ui->low->setText(low);
}

void HighLowDigitalWidget::setHighText(const QString &high)
{
    ui->high->setText(high);
}

bool HighLowDigitalWidget::isLow() const
{
    return ui->low->isChecked();
}

bool HighLowDigitalWidget::isHigh() const
{
    return ui->high->isChecked();
}



void HighLowDigitalWidget::on_high_clicked()
{
    if (!ui->high->isChecked()) {
        emit highTrig();
        emit valueChange(true);
    }
    ui->high->setChecked(true);
    ui->low->setChecked(false);
}

void HighLowDigitalWidget::on_low_clicked()
{
    if (!ui->low->isChecked()) {
        emit lowTrig();
        emit valueChange(false);
    }
    ui->low->setChecked(true);
    ui->high->setChecked(false);
}
