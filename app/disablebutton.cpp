#include "disablebutton.h"
#include "ui_disablebutton.h"
#include <QPushButton>
#include <QLabel>

DisableButton::DisableButton(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DisableButton)
{
    ui->setupUi(this);
    connect(ui->p, &QPushButton::clicked, this, &DisableButton::clicked);
    ui->l->setVisible(false);
    ui->p->setVisible(true);
}

DisableButton::~DisableButton()
{
    delete ui;
}

void DisableButton::setText(const QString &name)
{
    ui->p->setText(name);
}

QString DisableButton::text() const
{
    return ui->p->text();
}

void DisableButton::setEnabled(bool en)
{
    ui->l->setVisible(!en);
    ui->p->setVisible(en);
}
