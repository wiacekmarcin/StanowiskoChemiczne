#ifndef DOZOWNIKSETTINGS_H
#define DOZOWNIKSETTINGS_H

#include <QDialog>
class SerialMessage;
namespace Ui {
class DozownikSettings;
}

#define ON_RB_CLICKED(N) void on_rbP_##N##_clicked(); void on_rbL_##N##_clicked();

#define ON_SB_VALUECHANGED(N) void on_sb_steps_##N##_valueChanged(int arg1); void on_sb_ml_##N##_valueChanged(double arg1);

#define ON_PB_CLICKED(N) void on_pb_home_##N##_clicked(); void on_pb_set_##N##_clicked();

class DozownikSettings : public QDialog
{
    Q_OBJECT

public:
    explicit DozownikSettings(QWidget *parent = nullptr);
    ~DozownikSettings();
    void setSmg(SerialMessage * msg_);

private slots:
    ON_RB_CLICKED(1)
    ON_RB_CLICKED(2)
    ON_RB_CLICKED(3)
    ON_RB_CLICKED(4)
    ON_RB_CLICKED(5)

    ON_SB_VALUECHANGED(1)
    ON_SB_VALUECHANGED(2)
    ON_SB_VALUECHANGED(3)
    ON_SB_VALUECHANGED(4)
    ON_SB_VALUECHANGED(5)

    ON_PB_CLICKED(1)
    ON_PB_CLICKED(2)
    ON_PB_CLICKED(3)
    ON_PB_CLICKED(4)
    ON_PB_CLICKED(5)

private slots:
    void resetDone();
    void setParamsDone();
    void dozownik(bool conn);
    void errorSerial(QString);
    void debug(QString);
    void donePositionHome(bool ok);
    void donePosition();
    void successOpenDevice(bool success);


    void on_pbSetParameters_clicked();

private:
    Ui::DozownikSettings *ui;
    double ratio_steps2ml;
    bool praweObr[5];
    unsigned int steps[5];
    short nomodifysteps[5];
    SerialMessage * srlmsg;
    short NrDozownik;
    bool setHome;
    bool onlySetParameters;
};

#endif // DOZOWNIKSETTINGS_H
