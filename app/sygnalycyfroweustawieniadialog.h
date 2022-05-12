#ifndef SYGNALYCYFROWEUSTAWIENIADIALOG_H
#define SYGNALYCYFROWEUSTAWIENIADIALOG_H

#include <QDialog>
#include "ustawienia.h"



namespace Ui {
class SygnalyCyfroweUstawieniaDialog;
}

class SygnalyCyfroweUstawieniaDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SygnalyCyfroweUstawieniaDialog(Ustawienia & ust, const UserPrivilige & user, QWidget *parent = nullptr);
    ~SygnalyCyfroweUstawieniaDialog();

private:
    Ui::SygnalyCyfroweUstawieniaDialog *ui;
};

#endif // SYGNALYCYFROWEUSTAWIENIADIALOG_H
