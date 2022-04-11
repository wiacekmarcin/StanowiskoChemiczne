#ifndef OCZEKIWANIENAZAPLON_H
#define OCZEKIWANIENAZAPLON_H

#include <QDialog>
#include <QTimer>

namespace Ui {
class OczekiwanieNaZaplon;
}

class OczekiwanieNaZaplon : public QDialog
{
    Q_OBJECT

public:
    explicit OczekiwanieNaZaplon(QWidget *parent = nullptr);
    ~OczekiwanieNaZaplon();
private slots:
    void updateTime();
private:
    Ui::OczekiwanieNaZaplon *ui;
    unsigned int secs;
    unsigned int maxSecs;
    QTimer timer;
};

#endif // OCZEKIWANIENAZAPLON_H
