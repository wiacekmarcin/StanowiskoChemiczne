#ifndef WERSJADLG_H
#define WERSJADLG_H

#include <QDialog>

namespace Ui {
class WersjaDlg;
}

class WersjaDlg : public QDialog
{
    Q_OBJECT

public:
    explicit WersjaDlg(QWidget *parent = nullptr);
    ~WersjaDlg();

private:
    Ui::WersjaDlg *ui;
};

#endif // WERSJADLG_H
