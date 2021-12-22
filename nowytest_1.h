#ifndef NOWYTEST_1_H
#define NOWYTEST_1_H

#include "testpage.h"
#include "testpage.h"

namespace Ui {
class NowyTest_1;
}

class NowyTest_1 : public TestPage
{
    Q_OBJECT

public:
    explicit NowyTest_1(QWidget *parent = 0);
    ~NowyTest_1();
    virtual void initialPage() override;
protected:


private:
    Ui::NowyTest_1 *ui;
};

#endif // NOWYTEST_1_H
