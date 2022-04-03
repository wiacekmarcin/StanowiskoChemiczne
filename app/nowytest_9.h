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
};
#endif // NOWYTEST_8_H
