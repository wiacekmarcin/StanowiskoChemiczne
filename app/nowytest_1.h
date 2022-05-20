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
    explicit NowyTest_1(const QString& testName, bool visDozownik5, const UEkran1 &ekr, QWidget *parent = 0);
    ~NowyTest_1();
    virtual void initializePage() override;

protected slots:
    void nameTestChanged(const QString &arg1);
    void dozownikChanged(int index);
    void cieczChanged(int index);
    void iloscCieczyChanged(double arg1);
    void zaplonChanged(int index);

protected:
    void checkValid();
    double getVolume() const;
    QString getLuquid() const;
    int getDozownik() const;
    QString getName() const;
    float getHumanity() const;
private slots:
    void on_pbNext_clicked();

    void on_humanity_textEdited(const QString &arg1);

private:
    bool valid;
    QMap<QString, int> maxVal;
    Ui::NowyTest_1 *ui;
    double valCieczy;
    QString currCiecz;
    float humanity;

};

#endif // NOWYTEST_1_H
