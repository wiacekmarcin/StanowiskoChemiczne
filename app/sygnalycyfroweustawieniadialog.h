#ifndef SYGNALYCYFROWEUSTAWIENIADIALOG_H
#define SYGNALYCYFROWEUSTAWIENIADIALOG_H

#include <QDialog>
#include "ustawienia.h"



namespace Ui {
class SygnalyCyfroweUstawieniaDialog;
}

class QAbstractButton;

class SygnalyCyfroweUstawieniaDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SygnalyCyfroweUstawieniaDialog(Ustawienia & ust, const UserPrivilige & user, QWidget *parent = nullptr);
    ~SygnalyCyfroweUstawieniaDialog();
    void save();
public slots:
    void setOnOff(uint16_t mask);
signals:
    void writeValue(digitalOut mask, uint8_t value);
    void setIskraEl();

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_clicked(QAbstractButton *button);
private:
    Ui::SygnalyCyfroweUstawieniaDialog *ui;
    Ustawienia & ustawienia;
};

#endif // SYGNALYCYFROWEUSTAWIENIADIALOG_H
