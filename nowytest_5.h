#ifndef NOWYTEST_5_H
#define NOWYTEST_5_H

#include <QWizardPage>

namespace Ui {
class NowyTest_5;
}
class QCheckBox;
class NowyTest_5 : public QWizardPage
{
    Q_OBJECT

public:
    explicit NowyTest_5(QWidget *parent = nullptr);
    ~NowyTest_5();
    virtual bool isComplete() const override;
    virtual void initializePage() override;

private slots:
    void on_pbStep1_clicked();
    void on_pbStep2OK_clicked();
    void on_pbStep3OK_clicked();
    void on_pbStep4OK_clicked();

private:
    static void setReadOnly(QCheckBox* checkBox, bool readOnly);
    Ui::NowyTest_5 *ui;
    bool valid;
};

#endif // NOWYTEST_5_H
