#ifndef OTWARTEZAWORY_H
#define OTWARTEZAWORY_H

#include <QDialog>
#include "ustawienia.h"
#include <QMap>
#include <QString>

namespace Ui {
class OtwarteZawory;
}

class OtwarteZawory : public QDialog
{
    Q_OBJECT

public:
    explicit OtwarteZawory(const QMap<unsigned int, QString> & names, QWidget *parent = nullptr);
    ~OtwarteZawory();

    void set(int id, bool open);

private slots:
    void on_pbOk_clicked();

private:
    Ui::OtwarteZawory *ui;
};

#endif // OTWARTEZAWORY_H
