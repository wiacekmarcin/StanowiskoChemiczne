#ifndef NOWYTEST_7_H
#define NOWYTEST_7_H

#include "testpage.h"
class Urzadzenia;

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
    virtual PageId nextPageId() const override;


private slots:


    void on_pbOK_1_clicked();
    void on_rb1_yes_toggled(bool checked);
    void on_rb1_no_toggled(bool checked);

    void on_pbOK_2yes_clicked();

    void on_pBNo_3_clicked();

    void on_pbYes_3_clicked();

private:
    Ui::NowyTest_7 *ui;
    PageId next;
    bool initial;
    bool wykrytyZaplon;
    bool rbYes;
};

#endif // NOWYTEST_7_H
