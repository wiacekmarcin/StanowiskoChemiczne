#include "otwartezawory.h"
#include "ui_otwartezawory.h"
#include "ustawienia.h"

OtwarteZawory::OtwarteZawory(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OtwarteZawory)
{
    ui->setupUi(this);
    setWindowTitle("Ostrzeżenie, otwarte zawory");
    for (int i=0;i<8;++i)
        set(i+1, false);
    ui->frame_1->setVisible(false);
    ui->frame_2->setVisible(false);
    adjustSize();
}

OtwarteZawory::~OtwarteZawory()
{
    delete ui;
}

void OtwarteZawory::setSettings(const Ustawienia &set)
{
    ui->label_1->setText(set.wejscie(wentylacja_lewa));
    ui->label_2->setText(set.wejscie(wentylacja_prawa));
    ui->label_3->setText(set.wejscie(proznia));
    ui->label_4->setText(set.wejscie(wlot_powietrza));
    ui->label_5->setText(set.wejscie(pom_stez_1));
    ui->label_6->setText(set.wejscie(pom_stez_2));
    ui->label_7->setText(set.wejscie(drzwi_lewe));
    ui->label_7->setText(set.wejscie(drzwi_prawe));
}

//typedef enum _iDig {
//drzwi_prawe     = 0x001, //P2.0
//wentylacja_lewa = 0x002, //P2.1
//proznia         = 0x004, //P2.2
//pom_stez_1      = 0x008, //P2.3
//drzwi_lewe      = 0x010, //p2.4
//wentylacja_prawa= 0x020, //P2.5
//wlot_powietrza  = 0x040, //P2.6
//pom_stez_2      = 0x080, //P2.7
//pilot           = 0x100, //P1.4
//} digitalIn;
void OtwarteZawory::set(int id, bool open)
{
    //qDebug("OtwarteZawory::set(int id, bool open) %d %d", id, open);
    switch(id) {
    case wentylacja_lewa:
        ui->zawor_1_open->setVisible(!open);
        ui->zawor_1_close->setVisible(open);
        break;
    case wentylacja_prawa:
        ui->zawor_2_open->setVisible(!open);
        ui->zawor_2_close->setVisible(open);
        break;
    case proznia:
        ui->zawor_3_open->setVisible(!open);
        ui->zawor_3_close->setVisible(open);
        break;
    case wlot_powietrza:
        ui->zawor_4_open->setVisible(!open);
        ui->zawor_4_close->setVisible(open);
        break;
    case pom_stez_1:
        ui->zawor_5_open->setVisible(!open);
        ui->zawor_5_close->setVisible(open);
        break;
    case pom_stez_2:
        ui->zawor_6_open->setVisible(!open);
        ui->zawor_6_close->setVisible(open);
        break;
    case drzwi_lewe:
        ui->zawor_7_open->setVisible(!open);
        ui->zawor_7_close->setVisible(open);
        break;
    case drzwi_prawe:
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
