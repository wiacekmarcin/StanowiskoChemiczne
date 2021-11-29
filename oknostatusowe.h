#ifndef OKNOSTATUSOWE_H
#define OKNOSTATUSOWE_H

#include <QWidget>

namespace Ui {
class OknoStatusowe;
}

class OknoStatusowe : public QWidget
{
    Q_OBJECT

public:
    explicit OknoStatusowe(QWidget *parent = 0);
    ~OknoStatusowe();

private:
    Ui::OknoStatusowe *ui;
};

#endif // OKNOSTATUSOWE_H
