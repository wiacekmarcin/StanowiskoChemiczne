#ifndef NOWYTEST_3_H
#define NOWYTEST_3_H

#include "testpage.h"
#include <QMutex>
#include <QTimer>
namespace Ui {
class NowyTest_3;
}
class QCheckBox;

class NowyTest_3 : public TestPage
{
    Q_OBJECT

public:
    explicit NowyTest_3(double cisnienie, QWidget *parent = nullptr);
    ~NowyTest_3();
    virtual bool isComplete() const override;
    virtual void initializePage() override;

public slots:
    void openZawor(unsigned int id, bool val);
    void cisnienieKomory(double val);

private slots:
    void on_pbSetProznia_clicked();
    void on_pbSetSkip_clicked();
    void on_pbProzniaDone_clicked();
    void on_podcisnienie_valueChanged(int arg1);
    void runDone2();
    void runDone3();
    void on_pb100mBar_clicked();
    void on_pbStep2B2_OK_clicked();

    void updateProznia();

protected:
    double getCisnKomory();
    void setCisnKomory(double newCisnKomory);
    double getAvgCisnienie();
private:
    Ui::NowyTest_3 *ui;
    short cnt;
    double cisnKomory;
    bool zaworProzni;
    bool zaworPowietrza;
    unsigned short task;
    double cisnieWProzni;
    QMutex mutexCisnienie;
    double prevCisnienie[16];
    unsigned short idPrev;
    QTimer prozniaTimer;
    bool prozniaTask;
};

#endif // NOWYTEST_3_H
