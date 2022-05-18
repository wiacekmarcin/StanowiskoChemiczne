#ifndef SYGNALYANALOGOWESygnalyAnalogowyUstawieniaDialog_H
#define SYGNALYANALOGOWESygnalyAnalogowyUstawieniaDialog_H

#include <QDialog>
#include <QAbstractButton>

#include "ustawienia.h"

namespace Ui {
class SygnalyAnalogowyUstawieniaDialog;
}

class SygnalyAnalogowyUstawieniaDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SygnalyAnalogowyUstawieniaDialog(UserPrivilige &user, Ustawienia &u, QWidget *parent = nullptr);
    ~SygnalyAnalogowyUstawieniaDialog();

    void save();
private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_clicked(QAbstractButton *button);
private:
    Ui::SygnalyAnalogowyUstawieniaDialog *ui;
    Ustawienia & ust;
};

#endif // SYGNALYANALOGOWESygnalyAnalogowyUstawieniaDialog_H
