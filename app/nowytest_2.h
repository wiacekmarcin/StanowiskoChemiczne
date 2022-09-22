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
    explicit NowyTest_2(unsigned short initCykle, QWidget *parent = nullptr);
    ~NowyTest_2();
    virtual void initializePage() override;
    virtual void updateWejscia() override;

    virtual void dozownikDone(bool succes) override;
    virtual void checkPositionHomeDone(bool ok, bool d1, bool d2, bool d3, bool d4, bool d5) override;



private slots:

    void on_pbOk_1_clicked();
    void on_rb1_yes_toggled(bool checked);
    void on_rb1_no_toggled(bool checked);
    void on_pbOk_3_clicked();
    void on_pbOK_2_clicked();
    void on_rb4_yes_toggled(bool checked);
    void on_rb4_no_toggled(bool checked);
    void on_pbOk_4_clicked();

    void runDone();


    void on_pbOk_5_clicked();

private:

    Ui::NowyTest_2 *ui;
    short dozownik;
    unsigned short initCykle;

    bool m_DozownikPelny;
    bool pojedynczyCykl;

    bool showWarning;
    bool okDozownik;
    bool homeDozownik[5];

};

#endif // NOWYTEST_2_H
