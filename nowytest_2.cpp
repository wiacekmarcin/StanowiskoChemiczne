#include "nowytest_2.h"
#include "ui_nowytest_2.h"

NowyTest_2::NowyTest_2(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::NowyTest_2)
{
    ui->setupUi(this);
    ui->iloscCieczy->setMinimum(0);
    registerField("Dozownik", ui->cbDozownik, "currentText", "currentTextChanged");
    registerField("Ciecz", ui->cbCiecz, "currentText", "currentTextChanged");
    registerField("Ilosc", ui->iloscCieczy, "value", "valueChanged");
    registerField("Zaplon", ui->cbZaplon, "currentText", "currentTextChanged");
    registerField("EnergiaIskry", ui->cbenergiaIskry, "currentText", "currentTextChanged");
}

NowyTest_2::~NowyTest_2()
{
    delete ui;
}

void NowyTest_2::on_cbDozownik_currentIndexChanged(int index)
{
    if (wizard()->currentPage() != this)
        return;
    ui->cbCiecz->setEnabled(index != 0);


    if (index == 0) {
        ui->iloscCieczy->setValue(0);
        ui->cbZaplon->setCurrentIndex(0);
        ui->cbenergiaIskry->setCurrentIndex(0);
        ui->iloscCieczy->setEnabled(false);
        ui->cbZaplon->setEnabled(false);
        ui->cbenergiaIskry->setEnabled(false);
        emit completeChanged();
        return;
    }

    ui->cbCiecz->clear();

    QList<QStringList> ciecze;
    ciecze.append(QStringList() << "--");
    ciecze.append(QStringList() << "--" << "Woda" << "Benzyna" << "Ropa" << "Alkohol"); //1
    ciecze.append(QStringList() << "--" << "Woda"); //2
    ciecze.append(QStringList() << "--" << "Woda" << "Alkohol"); //3
    ciecze.append(QStringList() << "--" << "Benzyna" << "Ropa"); //4
    ciecze.append(QStringList() << "--" << "Alkohol"); //5

    ui->cbCiecz->addItems(ciecze.at(index));
    emit completeChanged();
}

void NowyTest_2::on_cbCiecz_currentTextChanged(const QString &arg1)
{
    if (wizard()->currentPage() != this)
        return;

    if (arg1 == "--") {
        ui->iloscCieczy->setEnabled(false);
        ui->cbZaplon->setEnabled(false);
        ui->cbenergiaIskry->setEnabled(false);
        emit completeChanged();
        return;
    }
    ui->iloscCieczy->setEnabled(true);
    QMap<QString, int> maxVal;
    maxVal["Woda"] = 100;
    maxVal["Benzyna"] = 10;
    maxVal["Ropa"] = 20;
    maxVal["Alkohol"] = 40;
    ui->maxcieczy->setText(QString::number(maxVal[arg1]));
    ui->iloscCieczy->setMaximum(maxVal[arg1]);
    if (ui->iloscCieczy->value() > maxVal[arg1])
        ui->iloscCieczy->setValue(maxVal[arg1]);

    ui->cbZaplon->setEnabled(true);
    emit completeChanged();
}

void NowyTest_2::on_cbZaplon_currentIndexChanged(int index)
{
    if (wizard()->currentPage() != this)
        return;

    if (index == 0) {
        emit completeChanged();
        return;
    }
    ui->cbenergiaIskry->setEnabled(index == 1);
    emit completeChanged();
}

bool NowyTest_2::isComplete() const
{
    if (wizard()->currentPage() != this)
        return true;

    if (!QWizardPage::isComplete())
        return false;

    if (!ui->cbZaplon->isEnabled() || ui->cbZaplon->currentIndex() == 0)
        return false;

    if ( ui->cbZaplon->currentIndex() == 1 && ui->cbenergiaIskry->currentIndex() == 0) //iskra elektryczna
        return false;

    return true;
}

void NowyTest_2::on_cbenergiaIskry_currentIndexChanged(int /*index*/)
{
    if (wizard()->currentPage() != this)
        return;
    emit completeChanged();
}
