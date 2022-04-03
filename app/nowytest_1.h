#ifndef NOWYTEST_1_H
#define NOWYTEST_1_H

#include "testpage.h"
#include "testpage.h"

namespace Ui {
class NowyTest_1;
}

class NowyTest_1 : public TestPage
{
    Q_OBJECT

public:
    explicit NowyTest_1(const QString& testName, QWidget *parent = 0);
    ~NowyTest_1();
    virtual void initializePage() override;

    virtual PageId nextPageId() const override { return PAGE_6; };
protected slots:
    void nameTestChanged(const QString &arg1);
    void dozownikChanged(int index);
    void cieczChanged(int index);
    void iloscCieczyChanged(double arg1);
    void zaplonChanged(int index);
    void energiaIskryChanged(int index);



protected:
    void checkValid();
    QString getIngitionExt() const;
    QString getIngition() const;
    double getVolume() const;
    QString getLuquid() const;
    int getDozownik() const;
    QString getName() const;
private slots:
    void on_pbNext_clicked();

private:
    bool valid;
    QMap<QString, int> maxVal;
    Ui::NowyTest_1 *ui;
};

#endif // NOWYTEST_1_H
