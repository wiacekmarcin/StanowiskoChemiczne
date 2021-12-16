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

    bool getNext() const;


signals:
    void valid(bool val);
    void next(short step);
    void prev(short step);
    void abort(short step);

private slots:
    void on_nameTest_textChanged(const QString &arg1);
    void form_reset();
    void form_next();
    void form_prev();
    void form_abort();


private:
    Ui::NowyTest_1 *ui;
    //bool next;
};

#endif // NOWYTEST_1_H
