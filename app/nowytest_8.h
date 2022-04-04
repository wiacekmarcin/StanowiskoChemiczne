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
    explicit NowyTest_8(QWidget *parent = nullptr);
    ~NowyTest_8();
    virtual void initializePage() override;

private slots:
    void runDone();
    void on_pbOK_1_clicked();
    void on_pbOK_2_clicked();
    void on_pbOK_3_clicked();

private:
    Ui::NowyTest_8 *ui;
};
#endif // NOWYTEST_6_H
