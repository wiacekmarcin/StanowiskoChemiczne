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

private:
    Ui::GlowneOkno *ui;
};

#endif // GLOWNEOKNO_H
