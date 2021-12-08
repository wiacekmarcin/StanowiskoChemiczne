#ifndef GLOWNEOKNO_H
#define GLOWNEOKNO_H

#include <QMainWindow>

namespace Ui {
class GlowneOkno;
}

class GlowneOkno : public QMainWindow
{
    Q_OBJECT

public:
    explicit GlowneOkno(QWidget *parent = 0);
    ~GlowneOkno();

private slots:
    void on_actionUstawienia_sygna_w_triggered();

private:
    Ui::GlowneOkno *ui;
};

#endif // GLOWNEOKNO_H
