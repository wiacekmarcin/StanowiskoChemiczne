#ifndef NOWYTEST_8_H
#define NOWYTEST_8_H

#include "testpage.h"

namespace Ui {
class NowyTest_8;
}

class QCheckBox;

class NowyTest_8 : public TestPage
{
    Q_OBJECT

public:
    explicit NowyTest_8(unsigned int czasPracyPompki, unsigned int czasNaStezenie, QWidget *parent = nullptr);
    ~NowyTest_8();
    virtual void initializePage() override;

private slots:
    void runDone();
    void runDone2();
    void on_pbOK_1_clicked();
    void on_pbOK_2_clicked();
    void on_pbOK_3_clicked();
    void on_pbOK_4_clicked();


private:
    Ui::NowyTest_8 *ui;
    unsigned long czasPracyPompki;
    unsigned long czasNaStezenie;
};
#endif // NOWYTEST_6_H
