#ifndef NOWYTEST_3_H
#define NOWYTEST_3_H

#include "testpage.h"
#include <QMutex>
#include <QTimer>
namespace Ui {
class NowyTest_3;
}
class QCheckBox;
class Urzadzenia;

class NowyTest_3 : public TestPage
{
    Q_OBJECT

public:
    explicit NowyTest_3(Urzadzenia * u, double cisnienie, QWidget *parent = nullptr);
    ~NowyTest_3();
    virtual bool isComplete() const override;
    virtual void initializePage() override;

public slots:
    void openZawor(unsigned int id, bool val);
    void cisnienieKomory(double val);






private slots:
    void updateCisnieie();

    void on_pbOk_1_clicked();

    void on_pb_Skip2_clicked();

    void on_pbUstaw_2_clicked();

    void on_pbOk_3_clicked();

    void on_pbOk_5_clicked();

    void on_pbRun_5_clicked();

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

    QTimer cisnienieTimer;
    bool prozniaTask;
    double wybrCisnienie;
    bool pomp_powietrza;
    Urzadzenia * urzadzenia;

    uint32_t timePompaProzniowa;
    double cisnienie_zad;
    double ustaloneCisnienie;
};

#endif // NOWYTEST_3_H
