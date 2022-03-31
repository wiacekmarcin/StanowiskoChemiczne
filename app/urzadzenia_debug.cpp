#include "ui_urzadzenia_debug.h"
#include "urzadzenia_debug.h"

#include "ustawienia.h"
#include "urzadzenia.h"


#define DBG_IN(N) qDebug("%s:%d %04x %p", __FILE__,__LINE__, inMap[N], ui->in_##N)

UrzadzeniaDebug::UrzadzeniaDebug(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UrzadzeniaDebug),

    inMap{0, drzwi_lewe, drzwi_prawe, pilot, wentylacja_lewa, wentylacja_prawa, pom_stez_1, pom_stez_2, proznia, wlot_powietrza},
    outMap{0, hv_onoff, hv_bezpieczenstwa, hw_iskra, mech_iskra, plomien, pompa_prozniowa, pompa_powietrza, wentylator, mieszadlo, trigger},
    anMap{a_vol1, a_vol2, a_o2, a_co2, a_cisn_komora, a_temp_komory, a_temp_parownik, a_8 }
{
    inputs = 0;
    ui->setupUi(this);
    timerAnalog.setInterval(100);
    connect(&timerAnalog, &QTimer::timeout, this, &UrzadzeniaDebug::updateAnalog);
    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);

    connect(ui->in_1, &HighLowDigitalWidget::valueChange, this, &UrzadzeniaDebug::changeDigital_1);
    connect(ui->in_2, &HighLowDigitalWidget::valueChange, this, &UrzadzeniaDebug::changeDigital_2);
    connect(ui->in_3, &HighLowDigitalWidget::valueChange, this, &UrzadzeniaDebug::changeDigital_3);
    connect(ui->in_4, &HighLowDigitalWidget::valueChange, this, &UrzadzeniaDebug::changeDigital_4);
    connect(ui->in_5, &HighLowDigitalWidget::valueChange, this, &UrzadzeniaDebug::changeDigital_5);
    connect(ui->in_6, &HighLowDigitalWidget::valueChange, this, &UrzadzeniaDebug::changeDigital_6);
    connect(ui->in_7, &HighLowDigitalWidget::valueChange, this, &UrzadzeniaDebug::changeDigital_7);
    connect(ui->in_8, &HighLowDigitalWidget::valueChange, this, &UrzadzeniaDebug::changeDigital_8);
    connect(ui->in_9, &HighLowDigitalWidget::valueChange, this, &UrzadzeniaDebug::changeDigital_9);

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
        anRevMap[i]->setMinimum(0);
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
}


UrzadzeniaDebug::~UrzadzeniaDebug()
{
    delete ui;
}

void UrzadzeniaDebug::setUrzadzenie(Urzadzenia * u)
{
    connect(this, &UrzadzeniaDebug::ni_analogValueChanged, u, &Urzadzenia::ni_analogValueChanged);
    connect(this, &UrzadzeniaDebug::ni_digitalRead, u, &Urzadzenia::ni_digitalRead);

    connect(u, &Urzadzenia::digitalWrite, this, &UrzadzeniaDebug::digitalWrite);
    connect(this, &UrzadzeniaDebug::writeValues, u, &Urzadzenia::digitalWriteDebug);
}

void UrzadzeniaDebug::digitalWrite(uint16_t mask)
{
    QToolButton * tb[] = {ui->tb_out_1, ui->tb_out_2, ui->tb_out_3, ui->tb_out_4, ui->tb_out_5, ui->tb_out_6,
                          ui->tb_out_7, ui->tb_out_8, ui->tb_out_9, nullptr, ui->tb_out_a};

    DigitalOutWidget * dow[] = {ui->out_1, ui->out_2, ui->out_3, ui->out_4, ui->out_5, ui->out_6,
                                ui->out_7, ui->out_8, ui->out_9, nullptr, ui->out_a};
    uint16_t maskIn = 0;
    DigitalOutWidget * dowOne;
    QToolButton * tbOne;
    bool val;
    for (unsigned int i = 0; i < Ustawienia::maxCzujekCyfrOut; ++i) {
        maskIn = 0x1 << i;
        tbOne = tb[i];
        dowOne = dow[i];
        if (tbOne == nullptr || dowOne == nullptr)
            continue;
        val = (mask & maskIn) ? true : false;
        tbOne->setText(val ? "H" : "L");
        dowOne->setLevel(val);
    }
    outputs = mask;
}

void UrzadzeniaDebug::updateAnalog()
{
    emit ni_analogValueChanged(anValue[0], anValue[1], anValue[2], anValue[3], anValue[4], anValue[5], anValue[6]);
}

#define AN_SET(N) ui->cz_label_##N->setText(ust.getName(anMap[N-1]))
#define IN_SET(N) ui->l_in_##N->setText(ust.wejscie(inMap[N]))
#define OUT_SET(N,M) ui->l_out_##N->setText(ust.wyjscie(outMap[M]))

void UrzadzeniaDebug::setLabels(const Ustawienia &ust)
{

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


     //emit analogValueChanged(0, 0, 0, 0, 0, 0, 0, 0);

    //for (short in = 1 ; in < 10; in++) {
    //     emit digitalValueChanged(inMap[in], false);
    //}

}


#define FUN_ANALOG_CHANGE(N) void UrzadzeniaDebug::on_analog_##N##_valueChanged(int value) { \
    double ratio = 1.0; /*ust ? ust->getRatio(anMap[N-1]) : 1.0;*/ \
    changeAnalog(N-1, ratio*value/10000, false); }

FUN_ANALOG_CHANGE(1)
FUN_ANALOG_CHANGE(2)
FUN_ANALOG_CHANGE(3)
FUN_ANALOG_CHANGE(4)
FUN_ANALOG_CHANGE(5)
FUN_ANALOG_CHANGE(6)
FUN_ANALOG_CHANGE(7)
FUN_ANALOG_CHANGE(8)

#define FUN_DIGITAL_CHANGE(N) void UrzadzeniaDebug::changeDigital_##N(bool val) { \
    digitalChange(inMap[N], val, false); }

FUN_DIGITAL_CHANGE(1)
FUN_DIGITAL_CHANGE(2)
FUN_DIGITAL_CHANGE(3)
FUN_DIGITAL_CHANGE(4)
FUN_DIGITAL_CHANGE(5)
FUN_DIGITAL_CHANGE(6)
FUN_DIGITAL_CHANGE(7)
FUN_DIGITAL_CHANGE(8)
FUN_DIGITAL_CHANGE(9)





//devs=USB6210
//product-type USB-6210
//product-id 14643
//serial 33770223

//devs=USB6501
//product-type USB-6501
//product-id 14646
//serial 33665651



void UrzadzeniaDebug::changeAnalog(unsigned short aId, double val, bool device)
{
    anValue[aId] = val;
    emit ni_analogValueChanged(anValue[0], anValue[1], anValue[2], anValue[3], anValue[4], anValue[5], anValue[6]);
}

void UrzadzeniaDebug::digitalChange(int id, bool val, bool device)
{
    uint16_t prev = inputs;
    if (val) {
        inputs |= id;
    } else {
        inputs &= ~id;
    }
    if (prev != inputs) {
        emit ni_digitalRead(inputs);
    }
}

void UrzadzeniaDebug::on_tb_out_clicked(QToolButton * tb,  DigitalOutWidget * dow, uint16_t mask)
{
    uint16_t prev = outputs;
    if (tb->text() == QString("L")) {
        tb->setText("H");
        outputs |= mask;
        dow->setLevel(true);
    } else {
        tb->setText("L");
        outputs &= ~mask;
        dow->setLevel(false);
    }
    if (prev != outputs) {
        emit writeValues(outputs);
    }
}


void UrzadzeniaDebug::on_tb_out_1_clicked()
{
    //iskra elektryczna on_off
    on_tb_out_clicked(ui->tb_out_1, ui->out_1, hv_onoff);
}

void UrzadzeniaDebug::on_tb_out_2_clicked()
{
    //iskra elektryczna bezpiecznik
    on_tb_out_clicked(ui->tb_out_2, ui->out_2, hv_bezpieczenstwa);
}

void UrzadzeniaDebug::on_tb_out_3_clicked()
{
    //iskra elektryczna zaplon
    on_tb_out_clicked(ui->tb_out_3, ui->out_3, hw_iskra);
}

void UrzadzeniaDebug::on_tb_out_4_clicked()
{
    //iskra mechaniczna
    on_tb_out_clicked(ui->tb_out_4, ui->out_4, mech_iskra);
}

void UrzadzeniaDebug::on_tb_out_5_clicked()
{
    //grzalka
    on_tb_out_clicked(ui->tb_out_5, ui->out_5, plomien);
}

void UrzadzeniaDebug::on_tb_out_6_clicked()
{
    //pompa_proznioewa
    on_tb_out_clicked(ui->tb_out_6, ui->out_6, pompa_prozniowa);
}

void UrzadzeniaDebug::on_tb_out_7_clicked()
{
    //pompa powietrza
    on_tb_out_clicked(ui->tb_out_7, ui->out_7, pompa_powietrza);
}

void UrzadzeniaDebug::on_tb_out_8_clicked()
{
    //wentylator
    on_tb_out_clicked(ui->tb_out_8, ui->out_8, wentylator);
}

void UrzadzeniaDebug::on_tb_out_9_clicked()
{
    //mieszadlo
    on_tb_out_clicked(ui->tb_out_9, ui->out_9, mieszadlo);
}

void UrzadzeniaDebug::on_tb_out_a_clicked()
{
    //trigger
    on_tb_out_clicked(ui->tb_out_a, ui->out_a, trigger);
}


