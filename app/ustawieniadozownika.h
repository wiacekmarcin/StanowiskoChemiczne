#ifndef USTAWIENIADOZOWNIKA_H
#define USTAWIENIADOZOWNIKA_H

#include <QDialog>
#include <QAbstractButton>
#include "ustawienia.h"

namespace Ui {
class UstawieniaDozownika;
}

#define ON_RB_CLICKED(N) void on_rb_l_##N##_clicked(); void on_rb_p_##N##_clicked();

class UstawieniaDozownika : public QDialog
{
    Q_OBJECT

public:
    explicit UstawieniaDozownika(Ustawienia & ust, QWidget *parent = nullptr);
    ~UstawieniaDozownika();

private slots:
    void on_buttonBox_clicked(QAbstractButton *button);
    void on_buttonBox_accepted();

    ON_RB_CLICKED(1)
    ON_RB_CLICKED(2)
    ON_RB_CLICKED(3)
    ON_RB_CLICKED(4)
    ON_RB_CLICKED(5)
protected:
    void save();
private:
    Ui::UstawieniaDozownika *ui;
    Ustawienia & ustawienia;
};

#endif // USTAWIENIADOZOWNIKA_H
