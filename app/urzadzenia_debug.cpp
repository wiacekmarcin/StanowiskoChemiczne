#include "urzadzenia.h"

#include "ustawienia.h"
#if 0
#define DEBUG_GUI 1

#ifdef DEBUG_GUI
#include "ui_urzadzenia.h"
#endif


//#define NEWCARDS


#define DBG_IN(N) qDebug("%s:%d %04x %p", __FILE__,__LINE__, inMap[N], ui->in_##N)

#define HVON(X) setDigital(hv_onoff, X); ui->out_1->setLevel(X); ui->tb_out_1->setText(X ? "H" : "L");
#define HBEZ(X) setDigital(hv_bezpieczenstwa, X); ui->out_2->setLevel(X); ui->tb_out_2->setText(X ? "H" : "L");
#define HZAP(X) setDigital(hw_iskra, X); ui->out_3->setLevel(X); ui->tb_out_3->setText(X ? "H" : "L");


Urzadzenia::Urzadzenia(QWidget *parent) :
    QDialog(parent),
#ifdef DEBUG_GUI
    ui(new Ui::Urzadzenia),
#endif
    dozownikLoop(this),
    readDigString("USB6501/port2,USB6501/port1/Line4"),
    writeDigString("USB6501/port0,USB6501/port1/Line0:2"),
    readAnalString("USB6210/ai0, USB6210/ai1, USB6210/ai2, USB6210/ai3, USB6210/ai4, USB6210/ai5, USB6210/ai6"),
    ust(NULL),
    inMap{0, drzwi_lewe, drzwi_prawe, pilot, wentylacja_lewa, wentylacja_prawa, pom_stez_1, pom_stez_2, proznia, wlot_powietrza},
    outMap{0, hv_onoff, hv_bezpieczenstwa, hw_iskra, mech_iskra, plomien, pompa_prozniowa, pompa_powietrza, wentylator, mieszadlo, trigger},
    anMap{a_vol1, a_vol2, a_o2, a_co2, a_cisn_komora, a_temp_komory, a_temp_parownik, a_8 }
{
#ifdef DEBUG_GUI
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

    anRevMap.clear();
    anRevMap[0] = ui->analog_1;
    anRevMap[1] = ui->analog_2;
    anRevMap[2] = ui->analog_3;
    anRevMap[3] = ui->analog_4;
    anRevMap[4] = ui->analog_5;
    anRevMap[5] = ui->analog_6;
    anRevMap[6] = ui->analog_7;
    anRevMap[7] = ui->analog_8;

    for (int i =0 ; i < 8; ++i) {
        anRevMap[i]->setMinimum(-10000);
        anRevMap[i]->setMaximum(10000);
        anRevMap[i]->setSingleStep(1);
        anRevMap[i]->setValue(0);
    }
    

    inRevMap.clear();
    inRevMap[inMap[1]] = ui->in_1;    //DBG_IN(1);
    inRevMap[inMap[2]] = ui->in_2;    //DBG_IN(2);
    inRevMap[inMap[3]] = ui->in_3;    //DBG_IN(3);
    inRevMap[inMap[4]] = ui->in_4;    //DBG_IN(4);
    inRevMap[inMap[5]] = ui->in_5;    //DBG_IN(5);
    inRevMap[inMap[6]] = ui->in_6;    //DBG_IN(6);
    inRevMap[inMap[7]] = ui->in_7;    //DBG_IN(7);
    inRevMap[inMap[8]] = ui->in_8;    //DBG_IN(8);
    inRevMap[inMap[9]] = ui->in_9;    //DBG_IN(9);
#endif


    /*serial*/
    connect(&smg, &SerialDevice::successOpenDevice, this, &Urzadzenia::successOpenDevice);
    connect(&smg, &SerialDevice::dozownik, this, &Urzadzenia::echoOK);
    connect(&smg, &SerialDevice::errorSerial, this, &Urzadzenia::errorSerial);
    connect(&smg, &SerialDevice::debug, this, &Urzadzenia::debug);


    smg.connectToSerial();
    dozownikLoop.setInterval(10000);
    connect(&dozownikLoop, &QTimer::timeout, this, &Urzadzenia::dozownikTimeout);
#ifdef DEBUG_UI
    ui->impulsyml->setValue(1.0);
    onlyOne = false;
    ui->rbDirectionLeft->setChecked(true);
    ui->maxSteps->setValue(1000);
#endif
#ifndef NEWCARDS
    timerDI100.setInterval(100);
    connect(&timerDI100, &QTimer::timeout, this, &Urzadzenia::timeoutDI100ms);
    timerDI100.start();

    timerAI100.setInterval(100);
    connect(&timerAI100, &QTimer::timeout, this, &Urzadzenia::timeoutAI100ms);
    timerAI100.start();
#endif
    timerCheckDevice.setInterval(5000);
    connect(&timerCheckDevice, &QTimer::timeout, this, &Urzadzenia::timerUsbDevice);
    timerCheckDevice.start();

    vals = 0;
    
    dozownikNr = 1;
    HVON(false)
    HBEZ(true)
    HZAP(false)

#ifndef NEWCARDS
    connect(&nicards, &NICards::digitalRead, this, &Urzadzenia::ni_digitalRead);
    connect(&nicards, &NICards::error, this, &Urzadzenia::ni_error);
    connect(&nicards, &NICards::timeout, this, &Urzadzenia::ni_timeout);
    connect(&nicards, &NICards::debug, this, &Urzadzenia::ni_debug);
    cconnect(&nicards, &NICards::usb6210, this, &Urzadzenia::ni_usb6210);
    connect(&nicards, &NICards::usb6501, this, &Urzadzenia::usb6501);
    connect(&nicards, &NICards::analogValueChanged, this, &Urzadzenia::analogValueChanged);
#endif
}


Urzadzenia::~Urzadzenia()
{
    timerDI100.stop();
    timerAI100.stop();
    timerCheckDevice.stop();
#ifdef DEBUG_UI
    delete ui;
#endif
}

#define AN_SET(N) ui->cz_label_##N##->setText(ust.getName(anMap[N-1]))
#define IN_SET(N) ui->l_in_##N##->setText(ust.wejscie(inMap[N]))
#define OUT_SET(N,M) ui->l_out_##N##->setText(ust.wejscie(outMap[M]))

void Urzadzenia::setLabels(const Ustawienia &ust)
{
#ifdef DEBUG_UI
    AN_SET(1);
    AN_SET(2);
    AN_SET(3);
    AN_SET(4);
    AN_SET(5);
    AN_SET(6);
    AN_SET(7);
    AN_SET(8);

    IN_SET(1);
    IN_SET(2);
    IN_SET(3);
    IN_SET(4);
    IN_SET(5);
    IN_SET(6);
    IN_SET(7);
    IN_SET(8);
    IN_SET(9);


    OUT_SET(1,1);
    OUT_SET(2,2);
    OUT_SET(3,3);
    OUT_SET(4,4);
    OUT_SET(5,5);
    OUT_SET(6,6);
    OUT_SET(7,7);
    OUT_SET(8,8);
    OUT_SET(9,9);
    OUT_SET(a,10);

#endif
    //for (short in = 0; in < 9 ; in++) {
        emit analogValueChanged(0, 0, 0, 0, 0, 0, 0, 0);
    //}

    for (short in = 1 ; in < 10; in++) {
         emit digitalValueChanged(inMap[in], false);
    }

}

void Urzadzenia::setUstawienia(Ustawienia * ust_)
{
    ust = ust_;
}


void Urzadzenia::setIskra()
{
    qDebug("Rozpoczynam zaplon iskry elektrycznej");
    //setDigital(hv_onoff, true);
    HVON(true)
    HBEZ(false)
    HZAP(false)

    delayMs(2000);
    qDebug("Wyzwolenie iskry");

    HVON(false)
    HBEZ(false)
    HZAP(true)

    delayMs(1000);
    qDebug("Koniec pracy");
    HVON(false)
    HBEZ(true)
    HZAP(false)

}

void Urzadzenia::setUstawienia(const Ustawienia &ust)
{
    smg.setSettings5(ust.getReverse_1(),ust.getReverse_2(),ust.getReverse_3(),ust.getReverse_4(),ust.getReverse_5(),
                     ust.getMaxImp(), ust.getImpTime());
}

#define FUN_ANALOG_CHANGE(N) void Urzadzenia::on_analog_##N##_valueChanged(int value) { \
    double ratio = ust ? ust->getRatio(anMap[N-1]) : 1.0; \
    changeAnalog(N-1, ratio*value/10000, false); }

FUN_ANALOG_CHANGE(1)
FUN_ANALOG_CHANGE(2)
FUN_ANALOG_CHANGE(3)
FUN_ANALOG_CHANGE(4)
FUN_ANALOG_CHANGE(5)
FUN_ANALOG_CHANGE(6)
FUN_ANALOG_CHANGE(7)
FUN_ANALOG_CHANGE(8)

#define FUN_DIGITAL_CHANGE(N) void Urzadzenia::changeDigital_##N(bool val) { \
    digitalChange(inMap[N], val); }

FUN_DIGITAL_CHANGE(1)
FUN_DIGITAL_CHANGE(2)
FUN_DIGITAL_CHANGE(3)
FUN_DIGITAL_CHANGE(4)
FUN_DIGITAL_CHANGE(5)
FUN_DIGITAL_CHANGE(6)
FUN_DIGITAL_CHANGE(7)
FUN_DIGITAL_CHANGE(8)
FUN_DIGITAL_CHANGE(9)

void Urzadzenia::digitalChange(int id, bool val)
{
    emit digitalValueChanged(id, val);
}

void Urzadzenia::changeDigital(int maks, bool val)
{
    //qDebug("%s:%d mask = %04x val = %d", __FILE__, __LINE__, maks, val);
#ifdef DEBUG_GUI
    inRevMap[maks]->setValue(val);
#endif
}

void Urzadzenia::dozownikTimeout()
{
    qDebug("dozownikTimeout");
    if (!connect2Serial) {
        emit connectToSerial();
        return;
    }
    //cntEcho += 1;
    //if (cntEcho > 3) {
    //    emit dozownik(false);
    //    connect2Serial = false;
    //}
    //emit echo();
}

void Urzadzenia::timerUsbDevice()
{
    //static unsigned short counter = 0;
    //if (++counter == 20) {
#ifndef NEWCARDS
        checkUsbCard();
#endif
        checkSerial();
        //counter = 0;
    //}
    //timeoutAI100ms();
    //timeoutDI100ms();
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
    //qDebug("%s:%d bools = %d %d", __FILE__, __LINE__, usbAnal, usbDio);
    if (usbAnal && usbDio)
        return;
    char buf[128];
#ifndef L_COMP
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
                qDebug("%s:%d uusbAnal = %d", __FILE__,__LINE__, usbAnal);
                if (!usbAnal) {
                    readAnalString.replace("USB6210", name);
#ifndef NEWCARDS
                    usbAnal = ai.configure(readAnalString);
#endif
                    qDebug("%s:%d configure %d", __FILE__,__LINE__,usbAnal);
                    //qDebug("%s:%d %s", __FILE__,__LINE__, ai.errStr().c_str());
                    emit usb6210(usbAnal);
                }
            }

            if (QString(buffer2) == QString("USB-6501") && deviceid == 14646 && serialid == 33665651) {
                qDebug("%d usbDio = %d", __LINE__,usbDio);
                if (!usbDio) {
                    readDigString.replace("USB6501", name);
                    writeDigString.replace("USB6501", name);
#ifndef NEWCARDS
                    usbDio = dio.configure(readDigString, writeDigString);
#endif
                    //qDebug("%s:%d configure %d", __FILE__,__LINE__,usbDio);
                    emit usb6501(usbDio);
                    setDigital(hv_onoff, false);
                    setDigital(hv_bezpieczenstwa, true);
                    setDigital(hw_iskra, false);
                    setDigital(mech_iskra, false);
                    setDigital(plomien, false);
                    setDigital(pompa_prozniowa, false);
                    setDigital(pompa_powietrza, false);
                    setDigital(wentylator, false);
                    setDigital(mieszadlo, false);
                    setDigital(unknown, false);
                    setDigital(trigger, false);
                }
            }
        }
    }
#endif
}

void Urzadzenia::checkSerial()
{
    //qDebug("%s:%d check Serial Device", __FILE__, __LINE__);
    //smg.echo();
    if (connect2Serial)
        return;

    smg.closeDevice();
    smg.connectToSerial();
}


void Urzadzenia::successOpenDevice(bool open)
{
    //qDebug("%s:%d open Device %d", __FILE__,__LINE__, open);
    connect2Serial = open;
    if (open) {

    }
}

void Urzadzenia::echoOK(bool ok)
{
    //qDebug("%s %d echo %d", __FILE__, __LINE__, ok);
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


void Urzadzenia::timeoutDI100ms()
{
#ifndef NEWCARDS
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
    changeDigital(drzwi_lewe, ~val & drzwi_lewe);
    emit drzwi_komory(false, val & drzwi_lewe);

    //in2 zamkniecie komory prawe
    changeDigital(drzwi_prawe, ~val & drzwi_prawe);
    emit drzwi_komory(true, val & drzwi_prawe);

    //in3 wentylacja 1
    changeDigital(wentylacja_lewa, ~val & wentylacja_lewa);
    emit zawor(wentylacja_lewa, val & wentylacja_lewa);

    //in4 wentylacja 2
    changeDigital(wentylacja_prawa, ~val & wentylacja_prawa);
    emit zawor(wentylacja_prawa, val & wentylacja_prawa);

    //in5 pomiar stezenia 1
    changeDigital(pom_stez_1, ~val & pom_stez_1);
    emit zawor(pom_stez_1, val & pom_stez_1);

    //in6 pomiar stezenia 2
    changeDigital(pom_stez_2, ~val & pom_stez_2);
    emit zawor(pom_stez_2, val & pom_stez_2);

    //in7 wlot_powietrza
    changeDigital(wlot_powietrza, ~val & wlot_powietrza);
    emit zawor(wlot_powietrza, val & wlot_powietrza);

    //in8 proznia
    changeDigital(proznia, ~val & proznia);
    emit zawor(proznia, val & proznia);

    //in9 pilot
    changeDigital(pilot, ~val & pilot);
    emit pilot_btn(val & pilot);
#endif
}

#define AN_CHANGE(N) changeAnalog(anMap[N], val##N, true)

void Urzadzenia::timeoutAI100ms()
{
 #ifndef NEWCARDS
    if (!usbAnal)
        return;

    float val0, val1, val2, val3, val4, val5, val6;
    if (!ai.readValue(val0, val1, val2, val3, val4, val5, val6)) {
        qDebug("%s:%d false read...", __FILE__, __LINE__);
        emit usb6210(false);
        usbAnal = false;
        return;
    }
    //qDebug("%s:%d %f %f %f %f %f %f %f", __FILE__,__LINE__, val0, val1, val2, val3, val4, val5, val6);
    AN_CHANGE(0);
    AN_CHANGE(1);
    AN_CHANGE(2);
    AN_CHANGE(3);
    AN_CHANGE(4);
    AN_CHANGE(5);
    AN_CHANGE(6);
    emit analogValueChanged(val0 * ust->getRatio(0),
                      val1 * ust->getRatio(1),
                      val2 * ust->getRatio(2),
                      val3 * ust->getRatio(3),
                      val4 * ust->getRatio(4),
                      val5 * ust->getRatio(5),
                      val6 * ust->getRatio(6),
                      val0 * ust->getRatio(0) * 100
                      );
#endif
}

void Urzadzenia::changeAnalog(unsigned short aId, double val, bool device)
{

    //qDebug("%s:%d id=%d val=%f ratio=%f", __FILE__, __LINE__, aId, val, ust->getRatio(aId));
#ifdef DEBUG_GUI
    if (device) {
        anRevMap[aId]->setValue((int)10000*val);
    }// else {
     //   emit analogValueChanged(aId, ust->getRatio(aId)*val);
    //}
#else
    //emit analogValueChanged(aId, ust->getRatio(aId)*val);
#endif
}


void Urzadzenia::setDigital(uint16_t mask, bool value)
{
    if (value) {
        vals |= mask;
#ifndef NEWCARDS
        dio.writeValue(vals);
#endif
    } else {
        vals &= ~mask;
#ifndef NEWCARDS
        dio.writeValue(vals);
#endif
    }
}

void Urzadzenia::on_tb_out_clicked(QToolButton * tb,  DigitalOutWidget * dow, uint16_t mask)
{
    if (tb->text() == QString("L")) {
        tb->setText("H");
        vals |= mask;
#ifndef NEWCARDS
        dio.writeValue(vals);
#endif
        dow->setLevel(true);
    } else {
        tb->setText("L");
        vals &= ~mask;
#ifndef NEWCARDS        
        dio.writeValue(vals);
#endif
        dow->setLevel(false);
    }
}


void Urzadzenia::on_tb_out_1_clicked()
{
    //iskra elektryczna on_off
    on_tb_out_clicked(ui->tb_out_1, ui->out_1, hv_onoff);
}

void Urzadzenia::on_tb_out_2_clicked()
{
    //iskra elektryczna bezpiecznik
    on_tb_out_clicked(ui->tb_out_2, ui->out_2, hv_bezpieczenstwa);
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
    //grzalka
    on_tb_out_clicked(ui->tb_out_5, ui->out_5, plomien);
}

void Urzadzenia::on_tb_out_6_clicked()
{
    //pompa_proznioewa
    on_tb_out_clicked(ui->tb_out_6, ui->out_6, pompa_prozniowa);
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





void Urzadzenia::on_pb_iskramechaniczna_clicked()
{
    setIskra();
}

#ifdef NEWCARDS

void Urzadzenia::ni_digitalRead(uint16_t vals)
{

}

void Urzadzenia::ni_error(const QString &s)
{

}

void Urzadzenia::ni_timeout(const QString &s)
{

}

void Urzadzenia::ni_debug(const QString &d)
{
    qDebug("NI %s", d.toStdString().c_str());
}

void Urzadzenia::ni_usb6210(bool ok)
{

}

void Urzadzenia::ni_usb6501(bool ok)
{

}

void Urzadzenia::ni_analogValueChanged(double val0, double val1, double val2, double val3, double val4, double val5, double val6)
{

}
#endif
#endif
