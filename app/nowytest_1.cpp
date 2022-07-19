#include "nowytest_1.h"
#include "ui_nowytest_1.h"
#include <QDebug>
#include "ustawienia.h"

NowyTest_1::NowyTest_1(const QString & testName, bool visDozownik5, const UEkran1 & ekr,
                       QWidget *parent) :
    TestPage(parent),
    ui(new Ui::NowyTest_1)
{
    ui->setupUi(this);
    ui->nameTest->setText(testName);
    valid = false;
    checkValid();

    connect(ui->nameTest, &QLineEdit::textChanged, this, &NowyTest_1::nameTestChanged);
    connect(ui->cbDozownik, qOverload<int>(&QComboBox::currentIndexChanged), this, &NowyTest_1::dozownikChanged);
    connect(ui->cbCiecz, qOverload<int>(&QComboBox::currentIndexChanged), this, &NowyTest_1::cieczChanged);
    connect(ui->iloscCieczy, qOverload<double>(&DoubleLineEdit::valueChanged), this, &NowyTest_1::iloscCieczyChanged);
    //connect(ui->humanity, &TextEdit::)

    currCiecz = "";
    valCieczy = 0.0;

    ui->iloscCieczy->setPrec(1);
    ui->iloscCieczy->setMin(0.1);

    ui->humanity->setPrec(1);
    ui->humanity->setMin(0);
    ui->humanity->setMax(100);

    maxVal[QString("Aceton")] = ekr.maxAceton;
    maxVal[QString("Etanol")] = ekr.maxEtanol;
    //maxVal[QString("Olej napędowy")] = 20;
    maxVal[QString("Izopropanol")] = ekr.maxIzopropanol;
    maxVal[QString("Benzyna")] = ekr.maxBenzyna;
    maxVal[QString("Toluen")] = ekr.maxToluen;

    if (visDozownik5) {
        ui->cbDozownik->addItem(QString("5"));
    }
    ui->cbDozownik->setEnabled(true);
    ui->cbCiecz->setEnabled(true);

    ui->iloscCieczy->setPrec(1);
    ui->iloscCieczy->setMin(0.1);
    ui->iloscCieczy->setMax(0);
}


NowyTest_1::~NowyTest_1()
{
    delete ui;
}

void NowyTest_1::initializePage()
{
    ui->pbNext->setEnabled(false);
    updateOutput(o_pompa_prozniowa, false);
    updateOutput(o_pompa_powietrza, false);
    updateOutput(o_wentylator, false);
    updateOutput(o_mieszadlo, false);
    setZ_criticalMask(0);
    setZ_warningMask(0);
}

QString NowyTest_1::getName() const
{
    return ui->nameTest->text();
}

float NowyTest_1::getHumanity() const
{
    return humanity;
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

void NowyTest_1::nameTestChanged(const QString &arg1)
{
    if (arg1.isEmpty()) {
        valid = false;
        checkValid();
        return;
    }
    valid = true;
    checkValid();
}

void NowyTest_1::dozownikChanged(int index)
{
    if (index == 0) {
        ui->iloscCieczy->setValue(0);
        ui->iloscCieczy->setEnabled(false);
        valid = false;
        checkValid();
        return;
    } else if (index == 5) {
        ui->iloscCieczy->setMax(10000000);
        ui->maxcieczy->setText("Bez limitu");
        setField(TestPage::maksymalnaIloscCieczy, QVariant::fromValue((double)10000000));
    }
    valid = false;
    valCieczy = 0.0;

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

        if (ui->iloscCieczy->text().isEmpty()) {
            v = false;
            break;
        }

        if (valCieczy < 0.1) {
            v = false;
            break;
        }

        if (ui->humanity->text().isEmpty()) {
            v = false;
            break;
        }

        break;
    }
    
    ui->pbNext->setEnabled(v);
}

void NowyTest_1::cieczChanged(int index)
{
    currCiecz = ui->cbCiecz->currentText();

    
    if (index == 0) {
        ui->iloscCieczy->setEnabled(false);
        valid = false;
        checkValid();
        return;
    }
    valid = true;
    ui->iloscCieczy->setEnabled(true);

    QString key = ui->cbCiecz->currentText();
    if (ui->cbDozownik->currentIndex() == 5) {
        ui->iloscCieczy->setMax(10000000);
        ui->maxcieczy->setText("Bez limitu");
        setField(TestPage::maksymalnaIloscCieczy, QVariant::fromValue((double)10000000));
    } else {
        ui->maxcieczy->setText(QString::number(maxVal[key]));
        ui->iloscCieczy->setMax(maxVal[key]);
        setField(TestPage::maksymalnaIloscCieczy, QVariant::fromValue((double)maxVal[key]));
    }
    if (valCieczy > maxVal[currCiecz])
        valid = false;

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
    TestData * d = getTestData();
    d->setNazwaTestu(getName());
    setField(TestPage::nazwaTest, QVariant::fromValue(getName()));

    d->setLiquidName(getLuquid());
    setField(TestPage::ciecz, QVariant::fromValue(getLuquid()));

    d->setLiquidVolue(getVolume());
    setField(TestPage::objetosc, QVariant::fromValue(getVolume()));

    d->setHumanity(getHumanity());
    d->setTemperaturaKomoryWarunkiPoczatkowe(getCzujnik(a_temp_komory));


    setField(TestPage::calaObjetosc, QVariant::fromValue(0.0));
    setField(TestPage::dozownikNr, QVariant::fromValue(getDozownik()));
    setField(TestPage::wybranyPlomien, QVariant::fromValue(false));

    setField(TestPage::maksymalnaIloscCieczy, QVariant::fromValue(
                 field(TestPage::maksymalnaIloscCieczy).toDouble() - field(TestPage::objetosc).toDouble()));
    nextPage(nextPageId());
}


void NowyTest_1::on_humanity_textEdited(const QString &arg1)
{
    QString value = arg1;
    value.replace(',','.');
    bool ok;
    humanity = value.toFloat(&ok);
    if (!ok) {
        humanity = 0;
    }
    valid = ok;
    checkValid();
}

