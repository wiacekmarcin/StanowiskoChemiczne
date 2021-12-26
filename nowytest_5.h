#ifndef NOWYTEST_5_H
#define NOWYTEST_5_H

#include "testpage.h"

namespace Ui {
class NowyTest_5;
}

class QCheckBox;

class NowyTest_5 : public TestPage
{
    Q_OBJECT

public:
    explicit NowyTest_5(QWidget *parent = nullptr);
    ~NowyTest_5();
    virtual bool isComplete() const override;
    virtual void initializePage() override;

private slots:
    void on_pbStep2_clicked();
    void on_pbStep3_clicked();
    void runDone2();

private:
    Ui::NowyTest_5 *ui;
};
#endif // NOWYTEST_6_H
