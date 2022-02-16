#ifndef OKNOSTATUSOWE_H
#define OKNOSTATUSOWE_H

#include <QWidget>
#include <QTimer>
#include <QFrame>

#include "ustawienia.h"

namespace Ui {
class OknoStatusowe;
}

class OknoStatusowe : public QFrame
{
    Q_OBJECT

public:
    explicit OknoStatusowe(QWidget *parent = 0);
    ~OknoStatusowe();
    void setLabels(const Ustawienia & set);
public slots:
    void setDigitalValue(int id, bool val);
    void setDozownik(bool ok);

private slots:
    void timeout();
private:
    Ui::OknoStatusowe *ui;
    QTimer timer;
};

#endif // OKNOSTATUSOWE_H
