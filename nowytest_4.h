#ifndef NOWYTEST_4_H
#define NOWYTEST_4_H

#include <QWizardPage>

namespace Ui {
class NowyTest_4;
}

class NowyTest_4 : public QWizardPage
{
    Q_OBJECT

public:
    explicit NowyTest_4(QWidget *parent = nullptr);
    ~NowyTest_4();

private:
    Ui::NowyTest_4 *ui;
};

#endif // NOWYTEST_4_H
