#ifndef NOWYTEST_2_H
#define NOWYTEST_2_H

#include "testpage.h"

namespace Ui {
class NowyTest_2;
}

class Urzadzenia;

class NowyTest_2 : public TestPage
{
    Q_OBJECT

public:
    explicit NowyTest_2(Urzadzenia * u, unsigned short initCykle, QWidget *parent = nullptr);
    ~NowyTest_2();
    virtual void initializePage() override;
    virtual bool isComplete() const override;

public slots:
    void dozownikDone(bool succes);
    void zamknietaKomora(bool komora);
signals:
    void cykleDozownik(uint8_t, uint32_t);


private slots:
    void on_cbFull1Yes_toggled(bool checked);
    void on_cbFull1No_toggled(bool checked);

    void on_pbQuestion2_clicked();
    void on_pb3Run_clicked();
    void on_cbFull4Yes_toggled(bool checked);
    void on_cbFull4No_toggled(bool checked);
    void on_pb5Run_clicked();
    void on_pb5Full_clicked();
    void on_pb6Ok_clicked();


protected:
    void isInitFiling(bool yes);
    void isFilling4(bool full);

    void runDone1();
    void runDone2();

private:
    Ui::NowyTest_2 *ui;
    bool ign;
    bool dozownikFull;
    bool showPb6Ok;
    short tempFiling;
    short dozownikNr;
    unsigned short initCykle;
    bool czyZamkKom;
    bool dozownikPodParow;
};

#endif // NOWYTEST_2_H
