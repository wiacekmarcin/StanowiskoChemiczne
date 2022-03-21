#ifndef USTAWIENIADOZOWNIKA_H
#define USTAWIENIADOZOWNIKA_H

#include <QDialog>
#include <QAbstractButton>
namespace Ui {
class UstawieniaDozownika;
}

class UstawieniaDozownika : public QDialog
{
    Q_OBJECT

public:
    explicit UstawieniaDozownika(QWidget *parent = nullptr);
    ~UstawieniaDozownika();

private slots:
    void on_buttonBox_clicked(QAbstractButton *button);
    void on_buttonBox_accepted();

private:
    Ui::UstawieniaDozownika *ui;
};

#endif // USTAWIENIADOZOWNIKA_H
