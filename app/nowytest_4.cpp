#include "nowytest_4.h"
#include "ui_nowytest_4.h"

#include "createtestwizard.h"
#include "testpage.h"
#include <QTimer>
#include <QMessageBox>

NowyTest_4::NowyTest_4(QWidget *parent) :
    TestPage(parent),
    ui(new Ui::NowyTest_4),
    infoString("Rozpocznij dozowanie <b>[CIECZ]</b> z dozonika <b>[DOZOWNIK]</b> w objętości <b>[OBJETOSC]</b> ml.")
{
    ui->setupUi(this);

    ui->frame_2->setVisible(false);
    ui->frame_3->setVisible(false);
    ui->frame_4->setVisible(false);
}

void NowyTest_4::initializePage()
{
    qDebug("%s:%d %f %f",__FILE__,__LINE__, field(TestPage::objetosc).toDouble(), field(TestPage::calaObjetosc).toDouble());
    QString info = infoString;
    info.replace("[CIECZ]", field(ciecz).toString());
    info.replace("[DOZOWNIK]", QString::number(field(dozownikNr).toUInt()));
    info.replace("[OBJETOSC]", QString::number(field(objetosc).toDouble()));
    if (field(calaObjetosc).toDouble() > 0) {
        info += QString(" Razem bedzie <b>%1<b> ml").arg(QString::number(
                            field(calaObjetosc).toDouble() + field(objetosc).toDouble()
                                                                       ));
        }


    ui->text_2->setTextFormat(Qt::RichText);
    ui->text_2->setText(info);
    ui->arrow_1->setVisible(true);
    ui->arrow_2->setVisible(true);
    ui->arrow_3->setVisible(true);
    ui->arrow_4->setVisible(true);

    ui->frame_2->setVisible(false);
    ui->frame_3->setVisible(false);
    ui->frame_4->setVisible(false);

    ui->pbOk_1->setEnabled(true);
    ui->pbOk_2->setEnabled(true);
    ui->pbOk_3->setEnabled(true);
    ui->pbOk_4->setEnabled(true);
}

NowyTest_4::~NowyTest_4()
{
    delete ui;
}

void NowyTest_4::dozownikDone(bool success)
{
    qDebug("%s:%d",__FILE__, __LINE__);
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
        setField(calaObjetosc, QVariant::fromValue(field(calaObjetosc).toDouble() + field(objetosc).toDouble()));
        ui->pbOk_2->setEnabled(false);
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
    ui->pbOk_2->setEnabled(false);
    dozownikMl(field(dozownikNr).toUInt()-1, (unsigned int)10*field(objetosc).toDouble());
    updateOutput(mieszadlo, true);
}

void NowyTest_4::on_pbOk_3_clicked()
{
    //qDebug "%s:%d %d", __FILE__, __LINE__, field(czyPompaMebr).toBool());

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

