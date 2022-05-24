#ifndef USTAWIENIADOZOWNIKA_H
#define USTAWIENIADOZOWNIKA_H

#include <QDialog>
#include <QAbstractButton>
#include "ustawienia.h"
#include "urzadzenia.h"

namespace Ui {
class UstawieniaDozownika;
}

#define ON_RB_CLICKED2(N) void on_rb_l_##N##_clicked(); void on_rb_p_##N##_clicked();

class UstawieniaDozownika : public QDialog
{
    Q_OBJECT

public:
    explicit UstawieniaDozownika(Urzadzenia * urzadzenia, Ustawienia & ust, const UserPrivilige & user, QWidget *parent = nullptr);
    ~UstawieniaDozownika();
    void save();

private slots:
    void buttonBox_clicked(QAbstractButton *button);
    void buttonBox_accepted();

    ON_RB_CLICKED2(1)
    ON_RB_CLICKED2(2)
    ON_RB_CLICKED2(3)
    ON_RB_CLICKED2(4)
    ON_RB_CLICKED2(5)

    void cbDozownik_currentIndexChanged(int index);
    void pbHomePosition_clicked();
    void pbCyklNapelniania_clicked();
    void pbSetPosition_clicked();
    void pbDozujSteps_clicked();
    void pbDozujMls_clicked();

    void setPositionHomeDone(bool success);
    void setPositionDone(bool success);
    void setCykleDone(bool success);
    void setStepsDone(bool success);


private:
    Ui::UstawieniaDozownika *ui;
    Ustawienia & ustawienia;
    short nrDozownika;
    Urzadzenia * urzadz;
};

#endif // USTAWIENIADOZOWNIKA_H
