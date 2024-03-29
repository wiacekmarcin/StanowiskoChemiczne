#include "nowyprojectdlg.h"
#include "ui_nowyprojectdlg.h"
#include <QFileDialog>
#include <QDate>
#include <QTime>
NowyProjectDlg::NowyProjectDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NowyProjectDlg)
{
    ui->setupUi(this);
    currDateTime = QDateTime::currentDateTime();
    ui->date->setText(currDateTime.toString("yyyy-MM-dd hh:mm"));
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);

}

NowyProjectDlg::~NowyProjectDlg()
{
    delete ui;
}

void NowyProjectDlg::on_pbChoiceDir_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Wybierz katalog"),
                                                    "/home",
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);
    if (!dir.isEmpty()) {
        ui->dirname->setText(dir);
    }
}

void NowyProjectDlg::on_name_textChanged(const QString &arg1)
{
    if (arg1.isEmpty())
        return;

    if (ui->dirname->text().isEmpty())
        return;

    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
}

void NowyProjectDlg::on_dirname_textChanged(const QString &arg1)
{
    if (arg1.isEmpty())
        return;

    if (ui->name->text().isEmpty())
        return;

    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
}

QString NowyProjectDlg::getName() const { return ui->name->text(); }
QString NowyProjectDlg::getMembers() const { return ui->participants->toPlainText(); }
QString NowyProjectDlg::getComment() const { return ui->comment->toPlainText(); }
QString NowyProjectDlg::getWorkDir() const { return ui->dirname->text(); }
QString NowyProjectDlg::getDate() const { return ui->date->text(); }

QDateTime NowyProjectDlg::getDateTime() const
{
    return currDateTime;
}
