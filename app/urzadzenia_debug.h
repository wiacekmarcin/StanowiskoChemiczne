#ifndef URZADZENIADEBUG_H
#define URZADZENIADEBUG_H

#include <QWidget>
#include <QDialog>
#include <QTimer>
#include <QMap>

#include "ustawienia.h"
#include "serialmessage.h"


namespace Ui {
class UrzadzeniaDebug;
}

class QToolButton;
class DigitalOutWidget;
class HighLowDigitalWidget;
class QSlider;
class Urzadzenia;

class UrzadzeniaDebug : public QDialog
{
    Q_OBJECT
public:

#define NEWCARDS 1


public:
    explicit UrzadzeniaDebug(QWidget *parent = 0);
    ~UrzadzeniaDebug();

    void setLabels(const Ustawienia & ust);
    void setUrzadzenie(Urzadzenia *u);
signals:


    void ni_analogValueChanged(double val0, double val1, double val2, double val3, double val4, double val5, double val6);
    void ni_digitalRead(uint16_t vals);

    //void analogValueChanged(double val1, double val2, double val3, double val4, double val5, double val6, double val7, double val8);
    //void digitalValueChanged(int id, bool high);
    void writeValues(uint16_t mask);
public slots:
    void digitalWrite(uint16_t mask);

private slots:
    void updateAnalog();

    void on_analog_1_valueChanged(int value);
    void on_analog_2_valueChanged(int value);
    void on_analog_3_valueChanged(int value);
    void on_analog_4_valueChanged(int value);
    void on_analog_5_valueChanged(int value);
    void on_analog_6_valueChanged(int value);
    void on_analog_7_valueChanged(int value);
    void on_analog_8_valueChanged(int value);

    void changeDigital_1(bool val);
    void changeDigital_2(bool val);
    void changeDigital_3(bool val);
    void changeDigital_4(bool val);
    void changeDigital_5(bool val);
    void changeDigital_6(bool val);
    void changeDigital_7(bool val);
    void changeDigital_8(bool val);
    void changeDigital_9(bool val);

    void on_tb_out_1_clicked();
    void on_tb_out_2_clicked();
    void on_tb_out_3_clicked();
    void on_tb_out_4_clicked();
    void on_tb_out_5_clicked();
    void on_tb_out_6_clicked();
    void on_tb_out_7_clicked();
    void on_tb_out_8_clicked();
    void on_tb_out_9_clicked();
    void on_tb_out_a_clicked();


protected:

    void on_tb_out_clicked(QToolButton * tb,  DigitalOutWidget * dow, uint16_t mask);
    void changeAnalog(unsigned short aId, double val, bool device);
    void digitalChange(int id, bool val, bool device);
private:
    Ui::UrzadzeniaDebug *ui;
    uint16_t outputs; //wyjscia
    uint16_t inputs;

    const unsigned int inMap[10];
    const unsigned int outMap[11];
    const unsigned int anMap[8];
    QMap<unsigned int, HighLowDigitalWidget*> inRevMap;
    QMap<unsigned short, QSlider*> anRevMap;
    QMap<unsigned short, double> anValue;

    QTimer timerAnalog;

};

#endif // URZADZENIA_H
