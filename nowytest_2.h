#ifndef NOWYTEST_2_H
#define NOWYTEST_2_H

#include <QWizardPage>

namespace Ui {
class NowyTest_2;
}

class NowyTest_2 : public QWizardPage
{
    Q_OBJECT

public:
    explicit NowyTest_2(QWidget *parent = nullptr);
    ~NowyTest_2();

private slots:
    void on_cbDozownik_currentIndexChanged(int index);

    void on_cbCiecz_currentTextChanged(const QString &arg1);

    void on_cbZaplon_currentIndexChanged(int index);

private:
    Ui::NowyTest_2 *ui;
};

#endif // NOWYTEST_2_H
