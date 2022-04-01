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


    valid = false;


}

void NowyTest_4::initializePage()
{
    infoString.replace("[CIECZ]", field("ciecz").toString());
    infoString.replace("[DOZOWNIK]", field("dozownik").toString());
    infoString.replace("[OBJETOSC]", field("objetosc").toString());
    ui->text_1->setTextFormat(Qt::RichText);
    ui->text_1->setText(infoString);
    ui->arrow_1->setVisible(true);
    ui->arrow_2->setVisible(true);
    ui->frame_2->setVisible(false);
    ui->frame_3->setVisible(false);
    showButton(false);
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
    }

}

void NowyTest_4::on_pbOk_1_clicked()
{
    ui->arrow_1->setVisible(false);
    ui->frame_2->setVisible(true);
    ui->pbOk_1->setEnabled(false);
    //emit dozownik(field("dozownik").toInt(), field("objetosc").toInt());
    QTimer::singleShot(1000, this, &NowyTest_4::runDone);
}

void NowyTest_4::on_pbOk_2_clicked()
{
    if (field("pompa_prozniowa").toBool()) {
        ui->arrow_2->setVisible(false);
        ui->frame_3->setVisible(true);
        ui->pbOk_3->setEnabled(false);
    } else {
        nextPage(nextPageId());
    }
}

void NowyTest_4::runDone()
{
    dozownikDone(true);
}

