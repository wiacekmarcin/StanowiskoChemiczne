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
    void on_pbOK_2_clicked();
    void on_rb3_zaplon_toggled(bool checked);
    void on_rb3_ciecz_toggled(bool checked);
    void on_pbOK_3_clicked();

    void on_pbOk_4_clicked();

private:
    Ui::NowyTest_7 *ui;
    PageId next;
    bool initial;
    bool wykrytyZaplon;
    bool dozowanieCieczy;
    bool rbYes;
};

#endif // NOWYTEST_7_H
