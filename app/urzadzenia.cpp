#include "urzadzenia.h"
#include "ui_urzadzenia.h"

Urzadzenia::Urzadzenia(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Urzadzenia),
    dozownikLoop(this)
{
    ui->setupUi(this);
    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);

    connect(ui->in_1, &HighLowDigitalWidget::valueChange, this, &Urzadzenia::changeDigital_1);
    connect(ui->in_2, &HighLowDigitalWidget::valueChange, this, &Urzadzenia::changeDigital_2);
    connect(ui->in_3, &HighLowDigitalWidget::valueChange, this, &Urzadzenia::changeDigital_3);
    connect(ui->in_4, &HighLowDigitalWidget::valueChange, this, &Urzadzenia::changeDigital_4);
    connect(ui->in_5, &HighLowDigitalWidget::valueChange, this, &Urzadzenia::changeDigital_5);
    connect(ui->in_6, &HighLowDigitalWidget::valueChange, this, &Urzadzenia::changeDigital_6);
    connect(ui->in_7, &HighLowDigitalWidget::valueChange, this, &Urzadzenia::changeDigital_7);
    connect(ui->in_8, &HighLowDigitalWidget::valueChange, this, &Urzadzenia::changeDigital_8);
    connect(ui->in_9, &HighLowDigitalWidget::valueChange, this, &Urzadzenia::changeDigital_9);

    /*serial*/
    connect(&smg, &SerialMessage::successOpenDevice, this, &Urzadzenia::successOpenDevice);
    connect(&smg, &SerialMessage::deviceName, this, &Urzadzenia::deviceName);
    connect(&smg, &SerialMessage::controllerOK, this, &Urzadzenia::controllerOK);
    connect(&smg, &SerialMessage::echoOK, this, &Urzadzenia::echoOK);
    connect(&smg, &SerialMessage::errorSerial, this, &Urzadzenia::errorSerial);
    connect(&smg, &SerialMessage::debug, this, &Urzadzenia::debug);


    connect(this, &Urzadzenia::connectToSerial, &smg, &SerialMessage::connectToSerial);
    connect(this, &Urzadzenia::echo, &smg, &SerialMessage::echo);

    emit connectToSerial();
    dozownikLoop.setInterval(10000);
    connect(&dozownikLoop, &QTimer::timeout, this, &Urzadzenia::dozownikTimeout);

}


Urzadzenia::~Urzadzenia()
{
    delete ui;
}

void Urzadzenia::setLabels(const Ustawienia &ust)
{
    ui->cz_label_1->setText(ust.getName(1));
    ui->cz_label_2->setText(ust.getName(2));
    ui->cz_label_3->setText(ust.getName(3));
    ui->cz_label_4->setText(ust.getName(4));
    ui->cz_label_5->setText(ust.getName(5));
    ui->cz_label_6->setText(ust.getName(6));
    ui->cz_label_7->setText(ust.getName(7));
    ui->cz_label_8->setText(ust.getName(8));

    ui->l_in_1->setText(ust.wejscie(1));
    ui->l_in_2->setText(ust.wejscie(2));
    ui->l_in_3->setText(ust.wejscie(3));
    ui->l_in_4->setText(ust.wejscie(4));
    ui->l_in_5->setText(ust.wejscie(5));
    ui->l_in_6->setText(ust.wejscie(6));
    ui->l_in_7->setText(ust.wejscie(7));
    ui->l_in_8->setText(ust.wejscie(8));
    ui->l_in_9->setText(ust.wejscie(9));

    ui->l_out_1->setText(ust.wyjscie(1));
    ui->l_out_2->setText(ust.wyjscie(2));
    ui->l_out_3->setText(ust.wyjscie(3));
    ui->l_out_4->setText(ust.wyjscie(4));
    ui->l_out_5->setText(ust.wyjscie(5));
    ui->l_out_6->setText(ust.wyjscie(6));
    ui->l_out_7->setText(ust.wyjscie(7));
    ui->l_out_8->setText(ust.wyjscie(8));
    ui->l_out_9->setText(ust.wyjscie(9));
    ui->l_out_10->setText(ust.wyjscie(10));


}

void Urzadzenia::on_analog_1_valueChanged(int value)
{
    emit analogValueChanged(1, value);
}

void Urzadzenia::on_analog_2_valueChanged(int value)
{
    emit analogValueChanged(2, value);
}

void Urzadzenia::on_analog_3_valueChanged(int value)
{
    emit analogValueChanged(3, value);
}

void Urzadzenia::on_analog_4_valueChanged(int value)
{
    emit analogValueChanged(4, value);
}

void Urzadzenia::on_analog_5_valueChanged(int value)
{
    emit analogValueChanged(5, value);
}

void Urzadzenia::on_analog_6_valueChanged(int value)
{
    emit analogValueChanged(6, value);
}

void Urzadzenia::on_analog_7_valueChanged(int value)
{
    emit analogValueChanged(7, value);
}

void Urzadzenia::on_analog_8_valueChanged(int value)
{
    emit analogValueChanged(8, value);
}

void Urzadzenia::changeDigital_1(bool val)
{
    emit digitalValueChanged(1, val);
}

void Urzadzenia::changeDigital_2(bool val)
{
    emit digitalValueChanged(2, val);
}

void Urzadzenia::changeDigital_3(bool val)
{
    emit digitalValueChanged(3, val);
}

void Urzadzenia::changeDigital_4(bool val)
{
    emit digitalValueChanged(4, val);
}

void Urzadzenia::changeDigital_5(bool val)
{
    emit digitalValueChanged(5, val);
}

void Urzadzenia::changeDigital_6(bool val)
{
    emit digitalValueChanged(6, val);
}

void Urzadzenia::changeDigital_7(bool val)
{
    emit digitalValueChanged(7, val);
}

void Urzadzenia::changeDigital_8(bool val)
{
    emit digitalValueChanged(8, val);
}

void Urzadzenia::changeDigital_9(bool val)
{
    emit digitalValueChanged(9, val);
}

void Urzadzenia::dozownikTimeout()
{
    qDebug("dozownikTimeout");
    if (!connect2Serial) {
        emit connectToSerial();
        return;
    }
    cntEcho += 1;
    if (cntEcho > 3) {
        emit dozownik(false);
        connect2Serial = false;
    }
    emit echo();
}

void Urzadzenia::successOpenDevice(bool open)
{
    qDebug("Open device %d", open);
}

void Urzadzenia::deviceName(QString name)
{
    qDebug("Device %s", name.toStdString().c_str());
}

void Urzadzenia::controllerOK()
{
    qDebug("KontrolerOK");
    connect2Serial = true;
    emit dozownik(true);
    cntEcho = 0;
    dozownikLoop.start();
}

void Urzadzenia::echoOK()
{
    qDebug("echo OK");
    if (cntEcho > 3)
        emit dozownik(true);
    cntEcho = 0;
}

void Urzadzenia::errorSerial(QString err)
{
    qDebug("errorSerial : %s", err.toStdString().c_str());
}

void Urzadzenia::debug(QString debug)
{
    qDebug("debug %s", debug.toStdString().c_str());
}
