#include "nowytestdlg.h"
#include "ui_nowytestdlg.h"
#include <QPushButton>

#include <QDebug>

NowyTestDlg::NowyTestDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NowyTestDlg)
{
    ui->setupUi(this);
    valid = false;
    ui->cbDozownik->setEnabled(false);
    checkValid();
}

NowyTestDlg::~NowyTestDlg()
{
    delete ui;
}

QString NowyTestDlg::getName() const
{
    return ui->nameTest->text();
}

int NowyTestDlg::getDozownik() const
{
    if (ui->cbDozownik->currentText() == "--")
        return -1;
    bool ok;
    int r = ui->cbDozownik->currentText().toInt(&ok);
    if (!ok)
        return -1;
    return r;
}

QString NowyTestDlg::getLuquid() const
{
    return ui->cbCiecz->currentText();
}

double NowyTestDlg::getVolume() const
{
    return ui->iloscCieczy->value();
}

QString NowyTestDlg::getIngition() const
{
    return ui->cbZaplon->currentText();
}

QString NowyTestDlg::getIngitionExt() const
{
    return ui->cbenergiaIskry->currentText();
}

void NowyTestDlg::on_nameTest_textChanged(const QString &arg1)
{
    if (arg1.isEmpty()) {
        valid = false;
        checkValid();
        return;
    }
    valid = true;
    if (!ui->cbDozownik->isEnabled())
        ui->cbDozownik->setEnabled(true);
    checkValid();
}

void NowyTestDlg::on_cbDozownik_currentIndexChanged(int index)
{
    if (index == 0) {
        ui->iloscCieczy->setValue(0);
        ui->cbZaplon->setCurrentIndex(0);
        ui->cbenergiaIskry->setCurrentIndex(0);
        ui->iloscCieczy->setEnabled(false);
        ui->cbZaplon->setEnabled(false);
        ui->cbenergiaIskry->setEnabled(false);
        checkValid();
        valid = false;
        return;
    }
    valid = true;
    ui->cbCiecz->setEnabled(true);
    ui->cbCiecz->clear();

    QList<QStringList> ciecze;
    ciecze.append(QStringList() << "--");
    ciecze.append(QStringList() << "--" << "Woda" << "Benzyna" << "Ropa" << "Alkohol"); //1
    ciecze.append(QStringList() << "--" << "Woda"); //2
    ciecze.append(QStringList() << "--" << "Woda" << "Alkohol"); //3
    ciecze.append(QStringList() << "--" << "Benzyna" << "Ropa"); //4
    ciecze.append(QStringList() << "--" << "Alkohol"); //5

    ui->cbCiecz->addItems(ciecze.at(index));
    checkValid();

}

void NowyTestDlg::checkValid()
{
    bool v = valid;
    while(v) {
        if (ui->nameTest->text().isEmpty()) {
            v = false;
            break;
        }

        if (ui->cbDozownik->currentIndex() == 0) {
            v = false;
            break;
        }

        if (ui->cbCiecz->currentIndex() == 0) {
            v = false;
            break;
        }

        if (ui->cbZaplon->currentIndex() == 0) {
            v = false;
            break;
        }

        if (ui->cbZaplon->currentIndex() == 1) {
            v = ui->cbenergiaIskry->currentIndex() > 0;
        }
        break;
    }
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(v);
}

void NowyTestDlg::on_cbCiecz_currentIndexChanged(int index)
{
    if (index == 0) {
        ui->iloscCieczy->setEnabled(false);
        ui->cbZaplon->setEnabled(false);
        ui->cbenergiaIskry->setEnabled(false);
        valid = false;
        checkValid();
        return;
    }
    valid = true;
    ui->iloscCieczy->setEnabled(true);
    ui->cbZaplon->setEnabled(true);

    QMap<QString, int> maxVal;
    maxVal["Woda"] = 100;
    maxVal["Benzyna"] = 10;
    maxVal["Ropa"] = 20;
    maxVal["Alkohol"] = 40;
    QString key = ui->cbCiecz->currentText();
    ui->maxcieczy->setText(QString::number(maxVal[key]));
    ui->iloscCieczy->setMaximum(maxVal[key]);
    if (ui->iloscCieczy->value() > maxVal[key])
        ui->iloscCieczy->setValue(maxVal[key]);


    checkValid();
}

void NowyTestDlg::on_iloscCieczy_valueChanged(double arg1)
{
    valid = true;
    if (arg1 < 0)
        valid = false;
    if (arg1 > maxVal[ui->cbCiecz->currentText()])
        valid = false;

    checkValid();
}

void NowyTestDlg::on_cbZaplon_currentIndexChanged(int index)
{
    if (index == 0) {
        valid = false;
        checkValid();
        return;
    }
    valid = true;
    ui->cbenergiaIskry->setEnabled(index == 1);
    checkValid();
}

void NowyTestDlg::on_cbenergiaIskry_currentIndexChanged(int index)
{
    valid = index != 0;
    checkValid();
}
