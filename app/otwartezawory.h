#ifndef OTWARTEZAWORY_H
#define OTWARTEZAWORY_H

#include <QDialog>
#include "ustawienia.h"
namespace Ui {
class OtwarteZawory;
}

class OtwarteZawory : public QDialog
{
    Q_OBJECT

public:
    explicit OtwarteZawory(QWidget *parent = nullptr);
    ~OtwarteZawory();

    void set(int id, bool open);
    void setSettings(const Ustawienia &set);

private slots:
    void on_pbOk_clicked();

private:
    Ui::OtwarteZawory *ui;
};

#endif // OTWARTEZAWORY_H
