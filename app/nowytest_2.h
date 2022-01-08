#ifndef NOWYTEST_2_H
#define NOWYTEST_2_H

#include "testpage.h"

namespace Ui {
class NowyTest_2;
}

class NowyTest_2 : public TestPage
{
    Q_OBJECT

public:
    explicit NowyTest_2(QWidget *parent = nullptr);
    ~NowyTest_2();
    virtual void initializePage() override;
    virtual bool isComplete() const override;

private slots:
    void on_cbYes_toggled(bool checked);
    void on_cbNo_toggled(bool checked);

    void on_pbStep1_clicked();
    void on_pbStep2_clicked();
    void on_cbStep3Yes_toggled(bool checked);
    void on_cbStep3No_toggled(bool checked);
    void on_pbStep3aRun_clicked();
    void on_pbStep3aOk_clicked();
    void on_pbStep4_clicked();




protected:
    void step1();
    void step3();
    void runDone1();
    void runDone2();

private:
    Ui::NowyTest_2 *ui;
    bool ign;
    bool dozownikFull;
};

#endif // NOWYTEST_2_H
