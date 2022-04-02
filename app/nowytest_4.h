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

signals:
    void updateOutput(uint16_t mask, bool on);

private slots:
    void dozownikDone(bool success);
    void on_pbOk_1_clicked();
    void on_pbOk_2_clicked();
    void on_pbOk_3_clicked();
    void on_pbOk_4_clicked();
    void runDone();


private:
    Ui::NowyTest_4 *ui;
    QString infoString;
};
#endif // NOWYTEST_4_H
