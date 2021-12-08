#ifndef OKNOSTATUSOWE_H
#define OKNOSTATUSOWE_H

#include <QWidget>
#include <QTimer>

namespace Ui {
class OknoStatusowe;
}

class OknoStatusowe : public QWidget
{
    Q_OBJECT

public:
    explicit OknoStatusowe(QWidget *parent = 0);
    ~OknoStatusowe();

private slots:
    void timeout();
private:
    Ui::OknoStatusowe *ui;
    QTimer timer;
};

#endif // OKNOSTATUSOWE_H
