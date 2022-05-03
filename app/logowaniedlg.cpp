#include "logowaniedlg.h"
#include "ui_logowaniedlg.h"
#include <QComboBox>
#include <QPushButton>

LogowanieDlg::LogowanieDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LogowanieDlg)
{
    ui->setupUi(this);
    connect(ui->comboBox, qOverload<int>(&QComboBox::currentIndexChanged), this, &LogowanieDlg::userChanged);
    connect(ui->lpsswd, &QLineEdit::textChanged, this, &LogowanieDlg::hasloChanged);
    connect(ui->pbLogin, &QPushButton::clicked, this, &LogowanieDlg::logowanie);
    connect(ui->pbCancel, &QPushButton::clicked, this, &LogowanieDlg::anuluj);
    user = U_STUDENT;
    ui->error->setVisible(false);
    ui->lpsswd->setEchoMode(QLineEdit::Password);
    ui->lpsswd->setEnabled(false);
}

LogowanieDlg::~LogowanieDlg()
{
    delete ui;
}

void LogowanieDlg::userChanged(int index)
{
    ui->lpsswd->setEnabled(index > 0);
    ui->error->setVisible(false);
    ui->lpsswd->setText("");
}


void LogowanieDlg::logowanie()
{
    ui->error->setVisible(false);
    if (ui->comboBox->currentIndex() == 1) {
        if (ui->lpsswd->text() == "1234") {
            user = U_ADMIN;
            accept();
        }
    }

    if (ui->comboBox->currentIndex() == 2) {
        if (ui->lpsswd->text() == "1234") {
            user = U_SERVISANT;
            accept();
        }
    }

    if (ui->comboBox->currentIndex() == 0) {
        user = U_STUDENT;
        accept();
    }
    ui->error->setVisible(true);
}

UserPrivilige LogowanieDlg::getUser() const
{
    return user;
}


void LogowanieDlg::hasloChanged(const QString &arg1)
{
    ui->error->setVisible(false);
}

void LogowanieDlg::anuluj()
{
    user = U_STUDENT;
    reject();
}

