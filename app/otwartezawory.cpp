#include "otwartezawory.h"
#include "ui_otwartezawory.h"
#include "ustawienia.h"

#define SET_LABEL(N,ID) ui->label_##N->setText(names[ID]); set(ID, false)

OtwarteZawory::OtwarteZawory(const QMap<unsigned int, QString> & names, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OtwarteZawory)
{
    ui->setupUi(this);
    setWindowTitle("Ostrzeżenie, otwarte zawory");

    SET_LABEL(1, i_wentylacja_lewa);
    SET_LABEL(2, i_wentylacja_prawa);
    SET_LABEL(3, i_proznia);
    SET_LABEL(4, i_wlot_powietrza);
    SET_LABEL(5, i_pom_stez_1);
    SET_LABEL(6, i_pom_stez_2);
    SET_LABEL(7, i_drzwi_lewe);
    SET_LABEL(8, i_drzwi_prawe);
    ui->frame_1->setVisible(false);
    ui->frame_2->setVisible(false);
    adjustSize();
}

OtwarteZawory::~OtwarteZawory()
{
    delete ui;
}

void OtwarteZawory::set(int id, bool open)
{
    ////qDebug"OtwarteZawory::set(int id, bool open) %d %d", id, open);
    switch(id) {
    case i_wentylacja_lewa:
        ui->zawor_1_open->setVisible(!open);
        ui->zawor_1_close->setVisible(open);
        break;
    case i_wentylacja_prawa:
        ui->zawor_2_open->setVisible(!open);
        ui->zawor_2_close->setVisible(open);
        break;
    case i_proznia:
        ui->zawor_3_open->setVisible(!open);
        ui->zawor_3_close->setVisible(open);
        break;
    case i_wlot_powietrza:
        ui->zawor_4_open->setVisible(!open);
        ui->zawor_4_close->setVisible(open);
        break;
    case i_pom_stez_1:
        ui->zawor_5_open->setVisible(!open);
        ui->zawor_5_close->setVisible(open);
        break;
    case i_pom_stez_2:
        ui->zawor_6_open->setVisible(!open);
        ui->zawor_6_close->setVisible(open);
        break;
    case i_drzwi_lewe:
        ui->zawor_7_open->setVisible(!open);
        ui->zawor_7_close->setVisible(open);
        break;
    case i_drzwi_prawe:
        ui->zawor_8_open->setVisible(!open);
        ui->zawor_8_close->setVisible(open);
        break;
    default:
        break;
    }
}

void OtwarteZawory::on_pbOk_clicked()
{
    emit reject();
}
