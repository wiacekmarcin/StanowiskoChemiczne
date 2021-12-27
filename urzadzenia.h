#ifndef URZADZENIA_H
#define URZADZENIA_H

#include <QWidget>
#include <QDialog>
namespace Ui {
class Urzadzenia;
}

class Urzadzenia : public QDialog
{
    Q_OBJECT

public:
    explicit Urzadzenia(QWidget *parent = 0);
    ~Urzadzenia();


signals:

private:
    Ui::Urzadzenia *ui;
};

#endif // URZADZENIA_H
