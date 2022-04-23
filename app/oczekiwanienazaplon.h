#ifndef OCZEKIWANIENAZAPLON_H
#define OCZEKIWANIENAZAPLON_H

#include <QDialog>
#include <QTimer>

class CreateTestWizard;
namespace Ui {
class OczekiwanieNaZaplon;
}

class OczekiwanieNaZaplon : public QDialog
{
    Q_OBJECT

public:
    explicit OczekiwanieNaZaplon(QWidget *parent, CreateTestWizard * wiz_, float tempB, float cisnB);
    ~OczekiwanieNaZaplon();
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
};
#endif // OCZEKIWANIENAZAPLON_H
