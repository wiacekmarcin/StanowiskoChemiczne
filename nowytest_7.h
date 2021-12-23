#ifndef NOWYTEST_7_H
#define NOWYTEST_7_H

#include "testpage.h"

namespace Ui {
class NowyTest_7;
}

class NowyTest_7 : public TestPage
{
    Q_OBJECT

public:
    explicit NowyTest_7(QWidget *parent = 0);
    ~NowyTest_7();
    virtual void initializePage() override;
    virtual bool isComplete() const override;
    virtual int nextPage() const override;

private slots:
    void on_rbZaplon_clicked();
    void on_rbBrakZaplonu_clicked();
    void on_pbStep1OK_clicked();

    void on_pbBrakZaplonu_clicked();
    void on_pbYes_clicked();
    void on_pbNo_clicked();
    void on_pbZaplon_clicked();

private:
    Ui::NowyTest_7 *ui;
    int next;
};

#endif // NOWYTEST_7_H
