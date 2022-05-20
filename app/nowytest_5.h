#ifndef NOWYTEST_5_H
#define NOWYTEST_5_H

#include "testpage.h"
#include "ustawienia.h"

namespace Ui {
class NowyTest_5;
}

class QCheckBox;

class NowyTest_5 : public TestPage
{
    Q_OBJECT

public:
    explicit NowyTest_5(const UPomiarStezen &u, QWidget *parent = nullptr);
    ~NowyTest_5();
    virtual void initializePage() override;

private slots:
    void runDone();
    void runDone2();
    void on_pbOK_1_clicked();
    void on_pbOK_2_clicked();
    void on_pbOK_3_clicked();

private:
    Ui::NowyTest_5 *ui;
    unsigned long czasPracyPompki;
    unsigned long czasNaStezenie;
};
#endif // NOWYTEST_6_H
