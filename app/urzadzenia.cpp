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
    //connect(ui->in_8, &HighLowDigitalWidget::valueChange, this, &Urzadzenia::changeDigital_8);
    //connect(ui->in_9, &HighLowDigitalWidget::valueChange, this, &Urzadzenia::changeDigital_9);

    /*serial*/
    connect(&smg, &SerialMessage::successOpenDevice, this, &Urzadzenia::successOpenDevice);
    connect(&smg, &SerialMessage::deviceName, this, &Urzadzenia::deviceName);
    connect(&smg, &SerialMessage::controllerOK, this, &Urzadzenia::controllerOK);
    connect(&smg, &SerialMessage::echoOK, this, &Urzadzenia::echoOK);
    connect(&smg, &SerialMessage::errorSerial, this, &Urzadzenia::errorSerial);
    connect(&smg, &SerialMessage::debug, this, &Urzadzenia::debug);


    connect(this, &Urzadzenia::connectToSerial, &smg, &SerialMessage::connectToSerial);
    connect(this, &Urzadzenia::echo, &smg, &SerialMessage::echo);
    connect(this, &Urzadzenia::setSettings, &smg, &SerialMessage::setSettings);
    connect(this, &Urzadzenia::setPosition, &smg, &SerialMessage::setPosition);


    smg.connectToSerial();
    dozownikLoop.setInterval(10000);
    connect(&dozownikLoop, &QTimer::timeout, this, &Urzadzenia::dozownikTimeout);

    ui->impulsyml->setValue(1.0);
    onlyOne = false;
    ui->rbDirectionLeft->setChecked(true);
    ui->maxSteps->setValue(1000);

    //timerDI100.setInterval(1000);
    //connect(&timerDI100, &QTimer::timeout, this, &Urzadzenia::timeoutDI100ms);
    //timerDI100.start();
    //dio.configure();
    //dio.writeValue(vals);
    //dio.writeValue(vals);

    timerAI100.setInterval(1000);
    connect(&timerAI100, &QTimer::timeout, this, &Urzadzenia::timeoutAI100ms);
    timerAI100.start();
    ai.configure();

    vals = 0;
    
    dozownikNr = 1;

    ui->analog_1->setMinimum(-10000);
    ui->analog_1->setMaximum(10000);
    ui->analog_1->setSingleStep(1);
    ui->analog_1->setValue(0);

    ui->analog_2->setMinimum(-10000);
    ui->analog_2->setMaximum(10000);
    ui->analog_2->setSingleStep(1);
    ui->analog_2->setValue(0);

    ui->analog_3->setMinimum(-10000);
    ui->analog_3->setMaximum(10000);
    ui->analog_3->setSingleStep(1);
    ui->analog_3->setValue(0);

    ui->analog_4->setMinimum(-10000);
    ui->analog_4->setMaximum(10000);
    ui->analog_4->setSingleStep(1);
    ui->analog_4->setValue(0);

    ui->analog_5->setMinimum(-10000);
    ui->analog_5->setMaximum(10000);
    ui->analog_5->setSingleStep(1);
    ui->analog_5->setValue(0);

    ui->analog_6->setMinimum(-10000);
    ui->analog_6->setMaximum(10000);
    ui->analog_6->setSingleStep(1);
    ui->analog_6->setValue(0);

    ui->analog_7->setMinimum(-10000);
    ui->analog_7->setMaximum(10000);
    ui->analog_7->setSingleStep(1);
    ui->analog_7->setValue(0);

}


Urzadzenia::~Urzadzenia()
{
    timerDI100.stop();
    timerAI100.stop();
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
    ui->l_out_a->setText(ust.wyjscie(10));


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
/*
void Urzadzenia::changeDigital_8(bool val)
{
    emit digitalValueChanged(8, val);
}

void Urzadzenia::changeDigital_9(bool val)
{
    emit digitalValueChanged(9, val);
}
*/
void Urzadzenia::dozownikTimeout()
{
    //qDebug("dozownikTimeout");
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
    //qDebug("Open device %d", open);
}

void Urzadzenia::deviceName(QString name)
{
    //qDebug("Device %s", name.toStdString().c_str());
}

void Urzadzenia::controllerOK()
{
    //qDebug("KontrolerOK");
    connect2Serial = true;
    emit dozownik(true);
    cntEcho = 0;
    dozownikLoop.start();
}

void Urzadzenia::echoOK()
{
    //qDebug("echo OK");
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

void Urzadzenia::on_ml_valueChanged(int arg1)
{
    if (onlyOne)
        return;
    onlyOne = true;
    ui->steps->setValue(ui->impulsyml->value()*arg1);
    onlyOne = false;
}


void Urzadzenia::on_steps_valueChanged(int arg1)
{
    if (onlyOne)
        return;
    onlyOne = true;
    ui->ml->setValue(arg1/ui->impulsyml->value());
    onlyOne = false;
}


void Urzadzenia::on_pbUstaw_clicked()
{
    emit setPosition(getDozownikNr(), ui->steps->value());
}


void Urzadzenia::on_rbDirectionLeft_clicked(bool checked)
{
    ui->rbDirectionLeft->setChecked(checked);
    ui->rbDirectionRight->setChecked(!checked);
}


void Urzadzenia::on_parameters_clicked()
{
    emit setSettings(ui->rbDirectionLeft->isChecked(), ui->maxSteps->value());
}


void Urzadzenia::on_pbReturn_clicked()
{
    emit setPositionHome(getDozownikNr());
}

void Urzadzenia::timeoutDI100ms()
{
    //qDebug("timeout");
    if (!dio.isConnected()) {
        qDebug("Not configure %s", dio.errStr().c_str());
        dio.configure();
        return;
    }
    
    uint16_t val;
    if (!dio.readValue(val)) {
        return;
    }
    
    //qDebug("val = %d", val);
    //in1 zakmniecie komory lewe
    ui->in_1->setValue(~val & drzwi_lewe);

    //in2 zamkniecie komory prawe
    ui->in_2->setValue(~val & drzwi_prawe);

    //in3 wentylacja 1
    ui->in_3->setValue(~val & wentylacja_lewa);

    //in4 wentylacja 2
    ui->in_4->setValue(~val & wentylacja_prawa);

    //in5 pomiar stezenia 1
    ui->in_5->setValue(~val & probka_in);

    //in6 pomiar stezenia 2
    ui->in_6->setValue(~val & probka_out);

    //in7 powietrze
    ui->in_7->setValue(~val & powietrze);

    //in8 proznia
    ui->in_8->setValue(~val & proznia);

    //ui->in_9->setValue(~val & 0x100);

}

void Urzadzenia::timeoutAI100ms()
{

    if (!ai.isConnected()) {
        qDebug("Not configure %s", ai.errStr().c_str());
        ai.configure();
        return;
    }

    float val0, val1, val2, val3, val4, val5, val6;
    if (!ai.readValue(val0, val1, val2, val3, val4, val5, val6)) {
        return;
    }

    //analog1 cisnienie w komorze ai4
    ui->analog_1->setValue(1000*val4);

    //analog2 stezenie voc ai0
    ui->analog_2->setValue(1000*val0);

    //analog3 stezenie o2 ai2
    ui->analog_3->setValue(1000*val2);

    //analog4 stezenie co2 ai3
    ui->analog_4->setValue(1000*val3);

    //analog5 temperatrura w komorze ai5
    ui->analog_5->setValue(1000*val5);

    //analog6 temperatrura parownika ai6
    ui->analog_6->setValue(1000*val6);

    //analog7 stezenie voc2 ai1
    ui->analog_7->setValue(1000*val1);

}


void Urzadzenia::on_tb_out_clicked(QToolButton * tb,  DigitalOutWidget * dow, uint16_t mask)
{
    if (tb->text() == QString("L")) {
        tb->setText("H");
        vals |= mask;
        dio.writeValue(vals);
        dow->setLevel(true);
    } else {
        tb->setText("L");
        vals &= ~mask;
        dio.writeValue(vals);
        dow->setLevel(false);
    }
}


void Urzadzenia::on_tb_out_1_clicked()
{
    //iskra elektryczna on_off
    on_tb_out_clicked(ui->tb_out_1, ui->out_1, high_voltage);
}

void Urzadzenia::on_tb_out_2_clicked()
{
    //iskra elektryczna bezpiecznik
    on_tb_out_clicked(ui->tb_out_2, ui->out_2, bezpiecznik);
}

void Urzadzenia::on_tb_out_3_clicked()
{
    //iskra elektryczna zaplon
    on_tb_out_clicked(ui->tb_out_3, ui->out_3, hw_iskra);
}


void Urzadzenia::on_tb_out_4_clicked()
{
    //iskra mechaniczna
    on_tb_out_clicked(ui->tb_out_4, ui->out_4, mech_iskra);
}

void Urzadzenia::on_tb_out_5_clicked()
{
    //plomien
    on_tb_out_clicked(ui->tb_out_5, ui->out_5, plomien);
}

void Urzadzenia::on_tb_out_6_clicked()
{
    //pompa_proznioewa
    on_tb_out_clicked(ui->tb_out_6, ui->out_6, pompa_proznia);
}

void Urzadzenia::on_tb_out_7_clicked()
{
    //pompa powietrza
    on_tb_out_clicked(ui->tb_out_7, ui->out_7, pompa_powietrza);
}

void Urzadzenia::on_tb_out_8_clicked()
{
    //wentylator
    on_tb_out_clicked(ui->tb_out_8, ui->out_8, wentylator);
}

void Urzadzenia::on_tb_out_9_clicked()
{
    //mieszadlo
    on_tb_out_clicked(ui->tb_out_9, ui->out_9, mieszadlo);
}

void Urzadzenia::on_tb_out_a_clicked()
{
    //trigger
    on_tb_out_clicked(ui->tb_out_a, ui->out_a, trigger);
}

void Urzadzenia::on_tb_out_b_clicked()
{
    //pilot zdalnego sterowania
    on_tb_out_clicked(ui->tb_out_b, ui->out_b, pilot);
}


void Urzadzenia::on_sbDozownik_valueChanged(int arg1)
{
    dozownikNr = arg1;
}




