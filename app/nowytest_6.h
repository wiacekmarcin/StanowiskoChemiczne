#ifndef NOWYTEST_6_H
#define NOWYTEST_6_H

#include "testpage.h"
namespace Ui {
class NowyTest_6;
}

class NowyTest_6 : public TestPage
{
    Q_OBJECT

public:
    explicit NowyTest_6(const UEkran6 & ust, QWidget *parent = nullptr);
    ~NowyTest_6();

    virtual void initializePage() override;
    virtual void updateWejscia() override;

signals:
    void showDglSignal();
private slots:
    void on_pbOK_1_clicked();
    void on_cbZaplon_2_currentTextChanged(const QString &arg1);
    void on_pbOK_2_clicked();
    void showDlgSlot();
private:
    Ui::NowyTest_6 *ui;
    const UEkran6 & ust;
    bool runDialog;
    bool step2;
};
#endif // NOWYTEST_7_H
