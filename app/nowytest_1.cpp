#include "nowytest_1.h"
#include "ui_nowytest_1.h"
#include <QDebug>

NowyTest_1::NowyTest_1(const QString & testName, QWidget *parent) :
    TestPage(parent),
    ui(new Ui::NowyTest_1)
{
    qDebug("%s:%d",__FILE__,__LINE__);
    ui->setupUi(this);
    qDebug("%s:%d",__FILE__,__LINE__);
    ui->nameTest->setText(testName);
    qDebug("%s:%d",__FILE__,__LINE__);
    valid = false;
    checkValid();
    qDebug("%s:%d",__FILE__,__LINE__);
    connect(ui->nameTest, &QLineEdit::textChanged, this, &NowyTest_1::nameTestChanged);
    connect(ui->cbDozownik, qOverload<int>(&QComboBox::currentIndexChanged), this, &NowyTest_1::dozownikChanged);
    connect(ui->cbCiecz, qOverload<int>(&QComboBox::currentIndexChanged), this, &NowyTest_1::cieczChanged);
    connect(ui->iloscCieczy, qOverload<double>(&QDoubleSpinBox::valueChanged), this, &NowyTest_1::iloscCieczyChanged);
    connect(ui->cbZaplon, qOverload<int>(&QComboBox::currentIndexChanged), this, &NowyTest_1::zaplonChanged);
    connect(ui->cbenergiaIskry, qOverload<int>(&QComboBox::currentIndexChanged), this, &NowyTest_1::energiaIskryChanged);
    qDebug("%s:%d",__FILE__,__LINE__);
}


NowyTest_1::~NowyTest_1()
{
    delete ui;
}

void NowyTest_1::initializePage()
{
    ui->pbNext->setEnabled(false);
}


QString NowyTest_1::getName() const
{
    return ui->nameTest->text();
}

int NowyTest_1::getDozownik() const
{
    if (ui->cbDozownik->currentText() == "--")
        return -1;
    bool ok;
    int r = ui->cbDozownik->currentText().toInt(&ok);
    if (!ok)
        return -1;
    return r;
}

QString NowyTest_1::getLuquid() const
{
    return ui->cbCiecz->currentText();
}

double NowyTest_1::getVolume() const
{
    return ui->iloscCieczy->value();
}

QString NowyTest_1::getIngition() const
{
    return ui->cbZaplon->currentText();
}

QString NowyTest_1::getIngitionExt() const
{
    return ui->cbenergiaIskry->currentText();
}

void NowyTest_1::nameTestChanged(const QString &arg1)
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

void NowyTest_1::dozownikChanged(int index)
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

void NowyTest_1::checkValid()
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

        if (ui->iloscCieczy->value() < 0.1) {
            v=false;
        }
        break;
    }
    ui->pbNext->setEnabled(v);
}

void NowyTest_1::cieczChanged(int index)
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

void NowyTest_1::iloscCieczyChanged(double arg1)
{
    valid = true;
    if (arg1 < 0)
        valid = false;
    if (arg1 > maxVal[ui->cbCiecz->currentText()])
        valid = false;

    checkValid();
}

void NowyTest_1::zaplonChanged(int index)
{
    if (index == 0) {
        valid = false;
        checkValid();
        return;
    }
    valid = true;
    ui->cbenergiaIskry->setEnabled(index == 1);
    ui->liskra->setEnabled(index == 1);
    checkValid();
}

void NowyTest_1::energiaIskryChanged(int index)
{
    valid = index != 0;
    checkValid();
}

void NowyTest_1::on_pbNext_clicked()
{
    setField(TestPage::nazwaTest, QVariant::fromValue(getName()));
    setField(TestPage::ciecz, QVariant::fromValue(getLuquid()));
    setField(TestPage::objetosc, QVariant::fromValue(getVolume()));
    setField(TestPage::calaObjetosc, QVariant::fromValue(0.0));
    setField(TestPage::dozownikNr, QVariant::fromValue(getDozownik()));
    setField(TestPage::zaplon, QVariant::fromValue(getIngition()));
    setField(TestPage::zaplonExtra, QVariant::fromValue(getIngitionExt()));
    setField(TestPage::rodzajZaplonu, QVariant::fromValue(ui->cbZaplon->currentIndex()));
    nextPage(nextPageId());
}

