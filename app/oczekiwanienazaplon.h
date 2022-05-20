#ifndef OCZEKIWANIENAZAPLON_H
#define OCZEKIWANIENAZAPLON_H

#include <QDialog>
#include <QTimer>
#include "ustawienia.h"
class CreateTestWizard;
namespace Ui {
class OczekiwanieNaZaplon;
}

class OczekiwanieNaZaplon : public QDialog
{
    Q_OBJECT

public:
    explicit OczekiwanieNaZaplon(QWidget *parent, CreateTestWizard * wiz_, float tempB, float cisnB, bool plomien, const UEkran6 & ust);
    ~OczekiwanieNaZaplon();


    bool getIsZaplon() const;

private slots:
    void updateTime();
private:
    Ui::OczekiwanieNaZaplon *ui;
    unsigned int secs;
    unsigned int maxSecs;
    QTimer timer;
    CreateTestWizard * wiz;
    float tempPocz;
    float cisnPocz;
    float tempPrev;
    float cisnPrev;
    bool plomien;
    bool isZaplon;
    float deltaCisnienia;
};
#endif // OCZEKIWANIENAZAPLON_H
