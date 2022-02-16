#ifndef USTAWIENIADOZOWNIKAOKNO_H
#define USTAWIENIADOZOWNIKAOKNO_H

#include <QDialog>

namespace Ui {
class UstawieniaDozownikaOkno;
}

class UstawieniaDozownikaOkno : public QDialog
{
    Q_OBJECT

public:
    explicit UstawieniaDozownikaOkno(QWidget *parent = 0);
    ~UstawieniaDozownikaOkno();

private:
    Ui::UstawieniaDozownikaOkno *ui;
};

#endif // USTAWIENIADOZOWNIKAOKNO_H
