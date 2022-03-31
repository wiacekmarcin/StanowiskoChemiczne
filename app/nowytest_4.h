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
    explicit NowyTest_4(QWidget *parent = nullptr);
    ~NowyTest_4();
    virtual bool isComplete() const override;
    virtual void initializePage() override;

private slots:
    void on_pbStep2OK_clicked();
    void on_pbStep3OK_clicked();
    void on_pbStep4OK_clicked();

    void runDone2();
    void runDone3();
    void runDone4();

    void on_pbOk_1_clicked();

private:
    Ui::NowyTest_4 *ui;
    QString infoString;
};
#endif // NOWYTEST_4_H
