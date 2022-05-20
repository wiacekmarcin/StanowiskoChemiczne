#ifndef NOWYTEST_4_H
#define NOWYTEST_4_H

#include "testpage.h"

namespace Ui {
class NowyTest_4;
}

class NowyTest_4 : public TestPage
{
    Q_OBJECT

public:
    explicit NowyTest_4(const UEkran4 & u4, QWidget *parent = nullptr);
    ~NowyTest_4();
    virtual void initializePage() override;

private slots:
    void dozownikDone(bool success) override;
    void on_pbOk_1_clicked();
    void on_pbOk_2_clicked();
    void on_pbOk_3_clicked();
    void on_pbOk_4_clicked();
    void runDone();
    void runDone2();


private:
    Ui::NowyTest_4 *ui;
    const UEkran4 & u4;
    const QString infoString;
};
#endif // NOWYTEST_4_H
