#ifndef NOWYTEST_2_H
#define NOWYTEST_2_H

#include <QDialog>

namespace Ui {
class NowyTest_2;
}

class NowyTest_2 : public QDialog
{
    Q_OBJECT

public:
    explicit NowyTest_2(QWidget *parent = nullptr);
    ~NowyTest_2();
signals:
    void valid(bool val);

private slots:
    void on_cbDozownik_currentTextChanged(const QString &arg1);

private:
    Ui::NowyTest_2 *ui;
};

#endif // NOWYTEST_2_H
