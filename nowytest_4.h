#ifndef NOWYTEST_4_H
#define NOWYTEST_4_H

#include <QWizardPage>

namespace Ui {
class NowyTest_4;
}
class QCheckBox;

class NowyTest_4 : public QWizardPage
{
    Q_OBJECT

public:
    explicit NowyTest_4(QWidget *parent = nullptr);
    ~NowyTest_4();
    virtual bool isComplete() const override;

    Q_PROPERTY(bool pracaPompy READ pracaPompy WRITE setPracaPompy NOTIFY pracaPompyChanged)

    bool pracaPompy() const  { return pracaPompy_; }
    void setPracaPompy(bool val) { pracaPompy_ = val; emit pracaPompyChanged(val); }
signals:
    void pracaPompyChanged(bool);
private slots:
    void on_pbStep1_clicked();
    void on_pbStep2_100_clicked();
    void on_pbStep2_OK_clicked();
    void on_pbStep2_Skip_clicked();
    void on_pbStep2A_OK_clicked();
    void on_pbStep2A_Next_clicked();
    void on_pbStep2B_OK_clicked();

    void runDone3();
    void runDone4();
private:
    static void setReadOnly(QCheckBox* checkBox, bool readOnly);
    Ui::NowyTest_4 *ui;
    bool valid;
    short cnt;
    bool pracaPompy_;
};

#endif // NOWYTEST_4_H
