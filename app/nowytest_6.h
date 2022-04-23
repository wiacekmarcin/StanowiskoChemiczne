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
    explicit NowyTest_6(QWidget *parent = nullptr);
    ~NowyTest_6();

    virtual void initializePage() override;
    virtual void updateWejscia() override;

private slots:
    void on_pbOK_1_clicked();

    void on_cbZaplon_2_currentTextChanged(const QString &arg1);

    void on_pbOK_2_clicked();

private:
    Ui::NowyTest_6 *ui;
    bool runDialog;
    bool step2;
};
#endif // NOWYTEST_7_H
