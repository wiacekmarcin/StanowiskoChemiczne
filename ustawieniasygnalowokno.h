#ifndef USTAWIENIASYGNALOWOKNO_H
#define USTAWIENIASYGNALOWOKNO_H

#include <QDialog>

namespace Ui {
class UstawieniaSygnalowOkno;
}

class UstawieniaSygnalowOkno : public QDialog
{
    Q_OBJECT

public:
    explicit UstawieniaSygnalowOkno(QWidget *parent = 0);
    ~UstawieniaSygnalowOkno();

private:
    Ui::UstawieniaSygnalowOkno *ui;
};

#endif // USTAWIENIASYGNALOWOKNO_H
