#ifndef NOWYPROJECTDLG_H
#define NOWYPROJECTDLG_H

#include <QDialog>

namespace Ui {
class NowyProjectDlg;
}

class NowyProjectDlg : public QDialog
{
    Q_OBJECT

public:
    explicit NowyProjectDlg(QWidget *parent = nullptr);
    ~NowyProjectDlg();
    QString getName() const;
    QString getMembers() const;
    QString getComment() const;
    QString getWorkDir() const;
    QString getDate() const;
private slots:
    void on_pbChoiceDir_clicked();
    void on_name_textChanged(const QString &arg1);
    void on_dirname_textChanged(const QString &arg1);

private:
    Ui::NowyProjectDlg *ui;
};

#endif // NOWYPROJECTDLG_H
