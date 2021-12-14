#ifndef NOWYTEST_1_H
#define NOWYTEST_1_H

#include <QDialog>

namespace Ui {
class NowyTest_1;
}

class NowyTest_1 : public QDialog
{
    Q_OBJECT

public:
    explicit NowyTest_1(QWidget *parent = 0);
    ~NowyTest_1();

signals:
    void valid(bool val);

private slots:
    void on_nameTest_textChanged(const QString &arg1);

private:
    Ui::NowyTest_1 *ui;
};

#endif // NOWYTEST_1_H
