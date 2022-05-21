#ifndef USTAWIENIATESTU_H
#define USTAWIENIATESTU_H

#include <QDialog>

#include "ustawienia.h"

namespace Ui {
class UstawieniaTestu;
}
class QAbstractButton;

class UstawieniaTestu : public QDialog
{
    Q_OBJECT

public:
    explicit UstawieniaTestu(Ustawienia & ust, const UserPrivilige & user, QWidget *parent = nullptr);
    ~UstawieniaTestu();
    void save();

private slots:
    void on_buttonBox_clicked(QAbstractButton *button);
    void on_buttonBox_accepted();


private:
    Ui::UstawieniaTestu *ui;
    Ustawienia & ustawienia;
};

#endif // USTAWIENIATESTU_H
