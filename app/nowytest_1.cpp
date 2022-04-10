#include "nowytest_1.h"
#include "ui_nowytest_1.h"
#include <QDebug>
#include "ustawienia.h"

NowyTest_1::NowyTest_1(const QString & testName, QWidget *parent) :
    TestPage(parent),
    ui(new Ui::NowyTest_1)
{
    //qDebug"%s:%d",__FILE__,__LINE__);
    ui->setupUi(this);
    //qDebug"%s:%d",__FILE__,__LINE__);
    ui->nameTest->setText(testName);
    //qDebug"%s:%d",__FILE__,__LINE__);
    valid = false;
    checkValid();
    //qDebug"%s:%d",__FILE__,__LINE__);
    connect(ui->nameTest, &QLineEdit::textChanged, this, &NowyTest_1::nameTestChanged);
    connect(ui->cbDozownik, qOverload<int>(&QComboBox::currentIndexChanged), this, &NowyTest_1::dozownikChanged);
    connect(ui->cbCiecz, qOverload<int>(&QComboBox::currentIndexChanged), this, &NowyTest_1::cieczChanged);
    connect(ui->iloscCieczy, qOverload<double>(&QDoubleSpinBox::valueChanged), this, &NowyTest_1::iloscCieczyChanged);
    connect(ui->cbZaplon, qOverload<int>(&QComboBox::currentIndexChanged), this, &NowyTest_1::zaplonChanged);
    //qDebug"%s:%d",__FILE__,__LINE__);
    currCiecz = "";
    valCieczy = 0.0;

    maxVal[QString("Woda")] = 100;
    maxVal[QString("Benzyna")] = 10;
    maxVal[QString("Ropa")] = 20;
    maxVal[QString("Alkohol")] = 40;
}


NowyTest_1::~NowyTest_1()
{
    delete ui;
}

void NowyTest_1::initializePage()
{
    ui->pbNext->setEnabled(false);
    updateOutput(pompa_prozniowa, false);
    updateOutput(pompa_powietrza, false);
    updateOutput(wentylator, false);
    updateOutput(mieszadlo, false);
    setZ_criticalMask(0);
    setZ_warningMask(0);
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
        ui->iloscCieczy->setEnabled(false);
        //ui->cbZaplon->setEnabled(false);
        valid = false;
        checkValid();
        return;
    }
    valid = true;
    ui->cbCiecz->setEnabled(true);
    ui->cbCiecz->clear();
    valCieczy = 0.0;

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
    //qDebug("\n\n%s:%d v=%d",__FILE__,__LINE__, v);
    while(v) {
        if (ui->nameTest->text().isEmpty()) {
            v = false;
            break;
        }
        //qDebug("%s:%d v=%d",__FILE__,__LINE__, v);
        if (ui->cbDozownik->currentIndex() == 0) {
            v = false;
            break;
        }
        //qDebug("%s:%d v=%d",__FILE__,__LINE__, v);
        if (ui->cbCiecz->currentIndex() == 0) {
            v = false;
            break;
        }
        //qDebug("%s:%d v=%d",__FILE__,__LINE__, v);
        if (ui->cbZaplon->currentIndex() == 0) {
            v = false;
            break;
        }
        break;
        if (valCieczy < 0.1) {
            v = false;
            break;
        }
    }
    //qDebug("%s:%d v=%d",__FILE__,__LINE__, v);
    ui->pbNext->setEnabled(v);
}

void NowyTest_1::cieczChanged(int index)
{
    currCiecz = ui->cbCiecz->currentText();

    //qDebug("%s:%d %d %s",__FILE__,__LINE__, index, currCiecz.toStdString().c_str());
    if (index == 0) {
        ui->iloscCieczy->setEnabled(false);
        ui->cbZaplon->setEnabled(false);
        valid = false;
        checkValid();
        return;
    }
    valid = true;
    ui->iloscCieczy->setEnabled(true);
    ui->cbZaplon->setEnabled(true);

    QString key = ui->cbCiecz->currentText();
    ui->maxcieczy->setText(QString::number(maxVal[key]));


    checkValid();
}

void NowyTest_1::iloscCieczyChanged(double arg1)
{
    valid = true;
    valCieczy = arg1;
    if (arg1 < 0.1)
        valid = false;
    if (arg1 > maxVal[currCiecz])
        valid = false;
    //qDebug("%s:%d %s %f < %f",__FILE__, __LINE__, currCiecz.toStdString().c_str(), arg1, maxVal[currCiecz]);
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
    setField(TestPage::rodzajZaplonu, QVariant::fromValue(ui->cbZaplon->currentIndex()));
    nextPage(nextPageId());
}

