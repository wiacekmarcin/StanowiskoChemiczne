#ifndef LOGOWANIEDLG_H
#define LOGOWANIEDLG_H

#include "ustawienia.h"
#include <QDialog>

namespace Ui {
class LogowanieDlg;
}

class LogowanieDlg : public QDialog
{
    Q_OBJECT

public:
    explicit LogowanieDlg(QWidget *parent = nullptr);
    ~LogowanieDlg();

    UserPrivilige getUser() const;

private slots:
    void userChanged(int index);
    void hasloChanged(const QString &arg1);
    void anuluj();
    void logowanie();

private:
    Ui::LogowanieDlg *ui;
    UserPrivilige user;
};

#endif // LOGOWANIEDLG_H
