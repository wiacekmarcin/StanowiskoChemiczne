#ifndef NOWYTEST_7_H
#define NOWYTEST_7_H

#include <QWizardPage>

namespace Ui {
class NowyTest_7;
}
class QCheckBox;
class NowyTest_7 : public QWizardPage
{
    Q_OBJECT

public:
    explicit NowyTest_7(QWidget *parent = nullptr);
    ~NowyTest_7();
    void setReadOnly(QCheckBox* checkBox, bool readOnly);
    virtual void initializePage() override;
    virtual bool isComplete() const override;

private slots:
    void on_pbStep1_clicked();

    void on_pbStep2_clicked();

private:
    Ui::NowyTest_7 *ui;
    bool valid;
};

#endif // NOWYTEST_7_H
