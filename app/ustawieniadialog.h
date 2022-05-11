#ifndef USTAWIENIADIALOG_H
#define USTAWIENIADIALOG_H

#include <QDialog>
#include "ustawienia.h"

namespace Ui {
class UstawieniaDialog;
}

class UstawieniaDialog : public QDialog
{
    Q_OBJECT

public:
    explicit UstawieniaDialog(UserPrivilige &user, const Ustawienia &u, QWidget *parent = nullptr);
    ~UstawieniaDialog();

    void saveData(Ustawienia & ust);

private:
    Ui::UstawieniaDialog *ui;
};

#endif // USTAWIENIADIALOG_H
