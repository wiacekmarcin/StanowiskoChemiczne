#include "nowytest_4.h"
#include "ui_nowytest_4.h"

#include "createtestwizard.h"
#include "testpage.h"
#include <QTimer>
#include <QMessageBox>

NowyTest_4::NowyTest_4(QWidget *parent) :
    TestPage(parent),
    ui(new Ui::NowyTest_4),
    infoString(QString("Rozpocznij dozowanie <b>[CIECZ]</b> z dozonika <b>[DOZOWNIK]</b> w objętości <b>[OBJETOSC]</b>."))
{
    ui->setupUi(this);

    ui->frame_2->setVisible(false);
    ui->frame_3->setVisible(false);
    ui->frame_4->setVisible(false);

    valid = false;


}

void NowyTest_4::initializePage()
{
    infoString.replace("[CIECZ]", field(ciecz).toString());
    infoString.replace("[DOZOWNIK]", QString::number(field(dozownikNr).toUInt()));
    infoString.replace("[OBJETOSC]", QString::number(field(objetosc).toUInt()));
    ui->text_2->setTextFormat(Qt::RichText);
    ui->text_2->setText(infoString);
    ui->arrow_1->setVisible(true);
    ui->arrow_2->setVisible(true);
    ui->arrow_3->setVisible(true);
    ui->frame_2->setVisible(false);
    ui->frame_3->setVisible(false);
    ui->frame_4->setVisible(false);
    valid = false;
    emit completeChanged();
}

NowyTest_4::~NowyTest_4()
{
    delete ui;
}

bool NowyTest_4::isComplete() const
{
    if (wizard()->currentPage() != this)
        return true;

    if (!TestPage::isComplete())
        return false;

    return valid;
}

void NowyTest_4::dozownikDone(bool success)
{
    if (!success) {
        QMessageBox msgBox;
        msgBox.setText("Nie udało się zadozować cieczy.");
        msgBox.setInformativeText("Czy chcesz kontynuować");
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msgBox.setDefaultButton(QMessageBox::No);
        int ret = msgBox.exec();
        if (ret == QMessageBox::No)
            return;
    } else {
        ui->arrow_2->setVisible(false);
        ui->frame_3->setVisible(true);
        ui->pbOk_2->setVisible(true);
    }

}

void NowyTest_4::on_pbOk_1_clicked()
{
    sprawdzZawory(ui->pbOk_1, ui->arrow_1, ui->frame_2);
}

void NowyTest_4::on_pbOk_2_clicked()
{
    //emit dozownik(field(dozownikNr).toUInt(), field(objetosc).toUInt());
    emit updateOutput(pompa_prozniowa, false);

    QTimer::singleShot(1000, this, &NowyTest_4::runDone);
}

void NowyTest_4::on_pbOk_3_clicked()
{
    qDebug( "%s:%d %d", __FILE__, __LINE__, field(czyPompaMebr).toBool());

    if (field(czyPompaMebr).toBool()) {
        ui->pbOk_3->setEnabled(false);
        ui->arrow_3->setVisible(false);
        ui->frame_4->setVisible(true);
    } else {
        nextPage(nextPageId());
    }
}

void NowyTest_4::runDone()
{
    dozownikDone(true);
}


void NowyTest_4::on_pbOk_4_clicked()
{
    if (!sprawdzOtwarteZaworPowietrza())
        return;
    nextPage(nextPageId());
}

