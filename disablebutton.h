#ifndef DISABLEBUTTON_H
#define DISABLEBUTTON_H

#include <QWidget>

namespace Ui {
class DisableButton;
}

class DisableButton : public QWidget
{
    Q_OBJECT

public:
    explicit DisableButton(QWidget *parent = 0);
    ~DisableButton();
    void setText(const QString & name);
    QString text() const;
    void setEnabled(bool en);
signals:
    void clicked();
private:
    Ui::DisableButton *ui;
};

#endif // DISABLEBUTTON_H
