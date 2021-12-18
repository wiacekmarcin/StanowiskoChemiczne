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
    virtual void initializePage() override;
    virtual bool isComplete() const override;

private slots:
    void on_cbYes_toggled(bool checked);
    void on_cbNo_toggled(bool checked);

    void on_pbStep1_clicked();
    void on_pbStep2_clicked();
    void on_cbStep3Yes_toggled(bool checked);
    void on_cbStep3No_toggled(bool checked);
    void on_pbStep3aRun_clicked();
    void on_pbStep3aOk_clicked();
    void on_pbStep4_clicked();

    void runDone();



protected:
    void step1();
    void step3();
private:
    Ui::NowyTest_3 *ui;
    bool ign;
    bool dozownikFull;
};

#endif // NOWYTEST_3_H
