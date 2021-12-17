#ifndef NOWYTEST_1_H
#define NOWYTEST_1_H


#include <QWizardPage>
namespace Ui {
class NowyTest_1;
}

class NowyTest_1 : public QWizardPage
{
    Q_OBJECT

public:
    explicit NowyTest_1(QWidget *parent = 0);
    ~NowyTest_1();
    virtual bool isComplete() const override;

private:
    Ui::NowyTest_1 *ui;
};

#endif // NOWYTEST_1_H
