#include "otwartezawory.h"
#include "ui_otwartezawory.h"

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

void OtwarteZawory::set(int id, bool open)
{
    //qDebug("OtwarteZawory::set(int id, bool open) %d %d", id, open);
    switch(id) {
    case 1:
        ui->zawor_1_open->setVisible(!open);
        ui->zawor_1_close->setVisible(open);
        break;
    case 2:
        ui->zawor_2_open->setVisible(!open);
        ui->zawor_2_close->setVisible(open);
        break;
    case 3:
        ui->zawor_3_open->setVisible(!open);
        ui->zawor_3_close->setVisible(open);
        break;
    case 4:
        ui->zawor_4_open->setVisible(!open);
        ui->zawor_4_close->setVisible(open);
        break;
    case 5:
        ui->zawor_5_open->setVisible(!open);
        ui->zawor_5_close->setVisible(open);
        break;
    case 6:
        ui->zawor_6_open->setVisible(!open);
        ui->zawor_6_close->setVisible(open);
        break;
    case 7:
        ui->zawor_7_open->setVisible(!open);
        ui->zawor_7_close->setVisible(open);
        break;
    case 8:
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
