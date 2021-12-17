#ifndef NOWYTEST_3_H
#define NOWYTEST_3_H

#include <QWizardPage>

namespace Ui {
class NowyTest_3;
}

class NowyTest_3 : public QWizardPage
{
    Q_OBJECT

public:
    explicit NowyTest_3(QWidget *parent = nullptr);
    ~NowyTest_3();


private:
    Ui::NowyTest_3 *ui;
};

#endif // NOWYTEST_3_H
