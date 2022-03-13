#include "urzadzenia.h"
#include "ui_urzadzenia.h"
#include "ustawienia.h"


Urzadzenia::Urzadzenia(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Urzadzenia),
    dozownikLoop(this),
    readDigString("USB6501/port2,USB6501/port1/Line5:6"),
    writeDigString("USB6501/port0,USB6501/port1/Line0:4"),
    readAnalString("USB6210/ai0, USB6210/ai1, USB6210/ai2, USB6210/ai3, USB6210/ai4, USB6210/ai5, USB6210/ai6"),
    ust(NULL)
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
    connect(&smg, &SerialMessage::dozownik, this, &Urzadzenia::echoOK);
    connect(&smg, &SerialMessage::errorSerial, this, &Urzadzenia::errorSerial);
    connect(&smg, &SerialMessage::debug, this, &Urzadzenia::debug);


    smg.connectToSerial();
    dozownikLoop.setInterval(10000);
    connect(&dozownikLoop, &QTimer::timeout, this, &Urzadzenia::dozownikTimeout);

    ui->impulsyml->setValue(1.0);
    onlyOne = false;
    ui->rbDirectionLeft->setChecked(true);
    ui->maxSteps->setValue(1000);

    timerDI100.setInterval(100);
    connect(&timerDI100, &QTimer::timeout, this, &Urzadzenia::timeoutDI100ms);
    timerDI100.start();



    timerAI100.setInterval(100);
    connect(&timerAI100, &QTimer::timeout, this, &Urzadzenia::timeoutAI100ms);
    timerAI100.start();


    timerCheckDevice.setInterval(1000);
    connect(&timerCheckDevice, &QTimer::timeout, this, &Urzadzenia::timerUsbDevice);
    timerCheckDevice.start();

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
    timerCheckDevice.stop();
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

    for (short in = 0; in < 7 ; in++) {
        emit analogValueChanged(in+1, 0);
    }

    for (short in = 0 ; in < 10; in++) {
         emit digitalValueChanged(in+1, false);
    }
}

void Urzadzenia::setUstawienia(Ustawienia * ust_)
{
    ust = ust_;
}

void Urzadzenia::setUstawienia(const Ustawienia &ust)
{
    smg.setSettings(ust.getReverseMotors(), ust.getMaxImp());
}

void Urzadzenia::on_analog_1_valueChanged(int value)
{
    double ratio = ust ? ust->getRatio(1) : 1.00;
    emit analogValueChanged(1, ratio*value);
}

void Urzadzenia::on_analog_2_valueChanged(int value)
{
    double ratio = ust ? ust->getRatio(2) : 1.00;
    emit analogValueChanged(2, ratio*value);
}

void Urzadzenia::on_analog_3_valueChanged(int value)
{
    double ratio = ust ? ust->getRatio(3) : 1.00;
    emit analogValueChanged(3, ratio*value);
}

void Urzadzenia::on_analog_4_valueChanged(int value)
{
    double ratio = ust ? ust->getRatio(4) : 1.00;
    emit analogValueChanged(4, ratio*value);
}

void Urzadzenia::on_analog_5_valueChanged(int value)
{
    double ratio = ust ? ust->getRatio(5) : 1.00;
    emit analogValueChanged(5, ratio*value);
}

void Urzadzenia::on_analog_6_valueChanged(int value)
{
    double ratio = ust ? ust->getRatio(6) : 1.00;
    emit analogValueChanged(6, ratio*value);
}

void Urzadzenia::on_analog_7_valueChanged(int value)
{
    double ratio = ust ? ust->getRatio(7) : 1.00;
    emit analogValueChanged(7, ratio*value);
}

void Urzadzenia::on_analog_8_valueChanged(int value)
{
    double ratio = ust ? ust->getRatio(8) : 1.00;
    emit analogValueChanged(8, ratio*value);
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

void Urzadzenia::timerUsbDevice()
{
    checkUsbCard();
    checkSerial();
}

//devs=USB6210
//product-type USB-6210
//product-id 14643
//serial 33770223

//devs=USB6501
//product-type USB-6501
//product-id 14646
//serial 33665651

#define DAQmxErrChk(functionCall) if( DAQmxFailed(error=(functionCall)) ) goto Error; else
#include <NIDAQmx.h>
void Urzadzenia::checkUsbCard()
{
    int32		error;
    //qDebug("bools = %d %d", usbAnal, usbDio);
    if (usbAnal && usbDio)
        return;
    char buf[128];
    DAQmxErrChk(DAQmxGetSysDevNames(buf, 128));

Error:
    char errBuff[2048];
    if (DAQmxFailed(error)) {
        DAQmxGetExtendedErrorInfo(errBuff, 2048);
        qDebug("err:%s", errBuff);
        usbAnal = usbDio = false;
    }

    const std::string expectedProductType = "1234";
    constexpr size_t bufferSize = 1000;
    char buffer[bufferSize] = {};
    if (!DAQmxFailed(DAQmxGetSysDevNames(buffer, bufferSize))) {
        QString allNames(buffer);
        QStringList names;
        names = allNames.split(",");
        for (auto & name : names) {
            char buffer2[bufferSize] = {};
            if (DAQmxFailed(DAQmxGetDevProductType(name.toStdString().c_str(), buffer2, bufferSize))) {
                continue;
            }
            int32 deviceid;
            if (DAQmxFailed(DAQmxGetDevProductCategory(name.toStdString().c_str(), &deviceid))) {
                continue;
            }
            uInt32 serialid;
            if (DAQmxFailed(DAQmxGetDevSerialNum(name.toStdString().c_str(), &serialid))) {
                continue;
            }

            qDebug("devs=%s deviceid=%d serial=%d => name=%s", buffer2, deviceid, serialid, name.toStdString().c_str());
            if (QString(buffer2) == QString("USB-6210") && deviceid == 14643 && serialid == 33770223) {
                if (!usbAnal) {
                    readAnalString.replace("USB6210", name);
                    usbAnal = ai.configure(readAnalString);
                    emit usb6210(usbAnal);
                }
            }

            if (QString(buffer2) == QString("USB-6501") && deviceid == 14646 && serialid == 33665651) {
                qDebug("%d usbDio = %d", __LINE__,usbDio);
                if (!usbDio) {
                    readDigString.replace("USB6501", name);
                    writeDigString.replace("USB6501", name);
                    usbDio = dio.configure(readDigString, writeDigString);
                    qDebug("%d usbDio = %d", __LINE__,usbDio);
                    if (usbDio)
                        qDebug("%d write ret %d %s", __LINE__, dio.writeValue(vals),dio.errStr().c_str());
                    emit usb6501(usbDio);
                }
            }
        }
    }
}

void Urzadzenia::checkSerial()
{
    smg.echo();
    if (connect2Serial)
        return;

    smg.closeDevice();
    smg.connectToSerial();
}


void Urzadzenia::successOpenDevice(bool open)
{
    connect2Serial = open;
    if (open) {

    }
}

void Urzadzenia::echoOK(bool ok)
{
    //qDebug("%s %d", __FILE__, __LINE__);
    emit dozownik(ok);
}

void Urzadzenia::errorSerial(QString err)
{
    qDebug("%s %d", __FILE__, __LINE__);
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
    smg.setPosition(getDozownikNr(), ui->steps->value());
}


void Urzadzenia::on_rbDirectionLeft_clicked(bool checked)
{
    ui->rbDirectionLeft->setChecked(checked);
    ui->rbDirectionRight->setChecked(!checked);
}


void Urzadzenia::on_parameters_clicked()
{
    if (ust) {
        ust->setReverseMotors(ui->rbDirectionLeft->isChecked());
        ust->setMaxImp(ui->maxSteps->value());
    }
    smg.setSettings(ui->rbDirectionLeft->isChecked(), ui->maxSteps->value());
}


void Urzadzenia::on_pbReturn_clicked()
{
    smg.setPositionHome(getDozownikNr());
}

void Urzadzenia::timeoutDI100ms()
{
    if(!usbDio)
        return;
    
    uint16_t val;
    if (!dio.readValue(val)) {
        usbDio = false;
        emit usb6501(false);
        return;
    }
    

    //qDebug("val = %d", val);
    //in1 zakmniecie komory lewe
    ui->in_1->setValue(~val & drzwi_lewe);
    emit drzwi_komory(false, val & drzwi_lewe);

    //in2 zamkniecie komory prawe
    ui->in_2->setValue(~val & drzwi_prawe);
    emit drzwi_komory(true, val & drzwi_prawe);

    //in3 wentylacja 1
    ui->in_3->setValue(~val & wentylacja_lewa);
    emit zawor(wentylacja_lewa, val & wentylacja_lewa);

    //in4 wentylacja 2
    ui->in_4->setValue(~val & wentylacja_prawa);
    emit zawor(wentylacja_prawa, val & wentylacja_prawa);

    //in5 pomiar stezenia 1
    ui->in_5->setValue(~val & probka_in);
    emit zawor(probka_in, val & probka_in);

    //in6 pomiar stezenia 2
    ui->in_6->setValue(~val & probka_out);
    emit zawor(probka_out, val & probka_out);

    //in7 powietrze
    ui->in_7->setValue(~val & wlot_powietrza);
    emit zawor(wlot_powietrza, val & wlot_powietrza);

    //in8 proznia
    ui->in_8->setValue(~val & proznia);
    emit zawor(proznia, val & proznia);

    //in9 pilot
    ui->in_9->setValue(~val & pilot);
    emit pilot_btn(val & pilot);

}

void Urzadzenia::timeoutAI100ms()
{
    if (!usbAnal)
        return;

    float val0, val1, val2, val3, val4, val5, val6;
    if (!ai.readValue(val0, val1, val2, val3, val4, val5, val6)) {
        emit usb6210(false);
        usbAnal = false;
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

void Urzadzenia::on_sbDozownik_valueChanged(int arg1)
{
    dozownikNr = arg1;
}




