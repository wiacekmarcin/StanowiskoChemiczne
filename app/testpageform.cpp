#include "testpageform.h"
#include "ui_testpageform.h"
#include "testpage.h"
#include "createtestwizard.h"
#include "otwartezawory.h"
#include "common.h"
#include "testdata.h"
#include <QMessageBox>

TestPageForm::TestPageForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TestPageForm)
{
    ui->setupUi(this);
    ui->subtitle->setVisible(false);
    ui->title->setVisible(false);
#if FINISHTEST
    ui->pbFinishTest->setVisible(true);
#else
    ui->pbFinishTest->setVisible(false);
#endif
}

void TestPageForm::addWidget(TestPage *page_)
{
    ui->testPageLayout->addWidget(page_);
    page = page_;
}

void TestPageForm::setTitle(const QString &title)
{
    ui->title->setText(QString("%1 - Krok %2").arg(title).arg(step));
    ui->title->setVisible(!title.isEmpty());
}

void TestPageForm::setSubTitle(const QString &title)
{
    ui->subtitle->setText(title);
    ui->subtitle->setVisible(!title.isEmpty());
}

void TestPageForm::initializePage()
{
    widget()->initializePage();
}

QFrame *TestPageForm::widgetFrame()
{
    return ui->frTestPage;
}

void TestPageForm::setCreateTestWizard(CreateTestWizard *wiz)
{
    wizard = wiz;
}

TestPageForm::~TestPageForm()
{
    delete ui;
}

void TestPageForm::setStep(short newStep)
{
    step = newStep;
}

void TestPageForm::visibleAbortBtn(bool vs)
{
    ui->pbAbort->setVisible(vs);
}


void TestPageForm::on_pbAbort_clicked()
{
    if (QMessageBox::warning(this, QString("Przerwanie test"), QString("Czy chcesz napewno przerwać test ?"),
                         QMessageBox::Yes | QMessageBox::No, QMessageBox::No) == QMessageBox::Yes)
        widget()->setFinished(false);
}


void TestPageForm::on_pbFinishTest_clicked()
{
    TestData * d = widget()->wizard()->getTestData();
    d->setDateTime(QDateTime::currentDateTime());
    d->setMembers(QStringList() << "Jan Kowalski" << "Leon Zimowiec" << "Klaudia Ładna");
    d->setLiquidName("Woda");
    d->setHumanity(75.5);
    d->setTemperaturaKomoryWarunkiPoczatkowe(25.3);
    d->setLiquidVolue(21.4);
    d->setLiquidVolue(15.0);

    d->setZrodloZaplonu("Iskra mechaniczna");
    d->setCisnienieKomoryDozowanie(1.0);
    d->setTemperaturaKomoryDozowanie(31.0);
    d->setTemperaturaParownika(34.0);
    d->setTemperaturaKomoryZaplon(40.9);
    d->setCisnienieKomoryZaplon(1.4);
    d->setStezeniaPrzedZaplonem(15.6, 16.7, 26.7, 32.0, 21.0);
    d->setUdanaProba(false, false, true);


    d->setZrodloZaplonu("Iskra elektryczna");
    d->setCisnienieKomoryDozowanie(1.5);
    d->setTemperaturaKomoryDozowanie(33.0);
    d->setTemperaturaParownika(35.0);
    d->setTemperaturaKomoryZaplon(42.9);
    d->setCisnienieKomoryZaplon(1.5);
    d->setStezeniaPrzedZaplonem(16.6, 17.7, 27.7, 31.0, 20.0);
    d->setUdanaProba(false, false, true);

    d->setZrodloZaplonu("Płomień");
    d->setCisnienieKomoryDozowanie(1.6);
    d->setTemperaturaKomoryDozowanie(34.0);
    d->setTemperaturaParownika(37.0);
    d->setTemperaturaKomoryZaplon(44.9);
    d->setCisnienieKomoryZaplon(1.7);
    d->setStezeniaPrzedZaplonem(11.6, 13.7, 21.7, 38.0, 22.0);
    d->setUdanaProba(true, true, true);

    d->setCisnienieKomoryWarunkiKoncowe(4.5);
    d->setTemperaturaKomoryWarunkiKoncowe(45.0);
    d->setStezeniaPoZaplonie(15.9, 16.9, 26.9, 32.9, 21.9);

    widget()->setFinished(true);
}

