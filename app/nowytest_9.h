#ifndef NOWYTEST_9_H
#define NOWYTEST_9_H

#include "testpage.h"
namespace Ui {
class NowyTest_9;
}

class NowyTest_9 : public TestPage
{
    Q_OBJECT

public:
    explicit NowyTest_9(QWidget *parent = nullptr);
    ~NowyTest_9();

    virtual void initializePage() override;

private:
    Ui::NowyTest_9 *ui;
    QString textMessage;
    QString textMsg1, zaplonMsg, brakZaplMsg, textMsg2, textMsg3, textMsg4, textMsg5, textMsg6, textMsg7;
};
#endif // NOWYTEST_8_H
