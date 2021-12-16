#ifndef NOWYTEST_3_H
#define NOWYTEST_3_H

#include <QDialog>

namespace Ui {
class NowyTest_3;
}

class NowyTest_3 : public QDialog
{
    Q_OBJECT

public:
    explicit NowyTest_3(QWidget *parent = nullptr);
    ~NowyTest_3();

signals:
    void valid(bool val);

private:
    Ui::NowyTest_3 *ui;
};

#endif // NOWYTEST_3_H
