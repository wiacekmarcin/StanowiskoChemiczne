#include "projektwidget.h"
#include "ui_projektwidget.h"

ProjektWidget::ProjektWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProjektWidget)
{
    ui->setupUi(this);
}

ProjektWidget::~ProjektWidget()
{
    delete ui;
}

void ProjektWidget::setDane(const QString &nazwa, const QString & uczestnicy,
                             const QString &katalog, const QString &komentarz,
                             const QString &data)
{
    ui->nazwa->setText(nazwa);
    ui->uczestnicy->setText(uczestnicy);
    ui->katalog->setText(katalog);
    ui->komentarz->setText(komentarz);
    ui->data->setText(data);
}

