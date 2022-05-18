#ifndef NOWYTEST_3_H
#define NOWYTEST_3_H

#include "testpage.h"
#include <QMutex>
#include <QTimer>
#include <QMap>

namespace Ui {
class NowyTest_3;
}
class QCheckBox;
class Urzadzenia;

class NowyTest_3 : public TestPage
{
    Q_OBJECT

public:
    explicit NowyTest_3(const double & mnoznik, const QString & unit,
                        uint64_t timePompaProzniowa,
                        short nrHisterezy,
                        double wspolczynnikDolny,
                        QWidget *parent = nullptr);
    ~NowyTest_3();
    virtual void initializePage() override;
    virtual void updateWejscia() override;
    virtual void setCisnKomory(const double & val) override;

public:
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
    double getAvgCisnienie();

    void debugStr(const QString & debug);

    QString getCisnienie(double val);




private:

    typedef enum _odciaganieProzniEnum {
        OP_IDLE,
        FIRST_WORK,
        FIRST_POMPA_RUN,
        FIRST_WAIT,
        NEXT_WORK,
        NEXT_WAIT,
        FINISH_STABLE,
        TIMEOUT,
    } OdciaganieProznieType;


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

    uint64_t timePompaProzniowaMax; //maksymalny czas pompy
    uint64_t timePompaProzniowa;

    uint64_t czas_wylaczeniaPompy;
    uint64_t czas_przerwyPompy;
    uint64_t czas_brakuWzrostuCisnienia;
    short maxHisterez; //maksymalna ilosc petli
    short nrHisterezy; //ilosc petli histerezy
    double wspolczynnikDolny; //o ile % ma zejsc na dol z sciaganiem cisnienia
    double cisnienie_zad; //cisnienie zadane
    double ustaloneCisnienie; //cisnienie koncowe ustalone
    bool ustalanieCisnienia; //czy nadal trwa ustalanie cisnienia
    double mnoznik; //mnoznik do obliczen
    bool oczekiwanieNaWzrost;

    OdciaganieProznieType actWork;
    QMap<OdciaganieProznieType, uint64_t> czasWork;
    QString unit;
};

#endif // NOWYTEST_3_H
