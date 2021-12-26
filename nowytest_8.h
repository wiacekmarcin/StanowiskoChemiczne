#ifndef NOWYTEST_8_H
#define NOWYTEST_8_H

#include "testpage.h"
namespace Ui {
class NowyTest_8;
}

class NowyTest_8 : public TestPage
{
    Q_OBJECT

public:
    explicit NowyTest_8(QWidget *parent = nullptr);
    ~NowyTest_8();

    virtual void initializePage() override;

private:
    Ui::NowyTest_8 *ui;
};
#endif // NOWYTEST_8_H
