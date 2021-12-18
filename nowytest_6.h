#ifndef NOWYTEST_6_H
#define NOWYTEST_6_H

#include <QWizardPage>

namespace Ui {
class NowyTest_6;
}

class QCheckBox;

class NowyTest_6 : public QWizardPage
{
    Q_OBJECT

public:
    explicit NowyTest_6(QWidget *parent = nullptr);
    ~NowyTest_6();
    void setReadOnly(QCheckBox* checkBox, bool readOnly);
    virtual bool isComplete() const override;

private slots:
    void on_pbStep1_clicked();
    void on_pbStep2_clicked();
    void on_pbStep3_clicked();

private:
    Ui::NowyTest_6 *ui;

    bool valid;
};

#endif // NOWYTEST_6_H
