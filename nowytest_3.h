#ifndef NOWYTEST_3_H
#define NOWYTEST_3_H

#include "testpage.h"

namespace Ui {
class NowyTest_3;
}
class QCheckBox;

class NowyTest_3 : public TestPage
{
    Q_OBJECT

public:
    explicit NowyTest_3(QWidget *parent = nullptr);
    ~NowyTest_3();
    virtual bool isComplete() const override;

    virtual void initializePage() override;
private slots:
    void on_pbStep2_OK_clicked();
    void on_pbStep2_Skip_clicked();
    void on_pbStep2A_OK_clicked();
    void on_pbStep2A_Next_clicked();
    void on_pbStep2B_OK_clicked();

    void runDone3();
    void runDone4();
    void on_podcisnienie_valueChanged(int arg1);

private:
    Ui::NowyTest_3 *ui;
    short cnt;
};

#endif // NOWYTEST_3_H
