#ifndef WIDOKCZUJNIKA_H
#define WIDOKCZUJNIKA_H

#include <QWidget>

namespace Ui {
class WidokCzujnika;
}

class WidokCzujnika : public QWidget
{
    Q_OBJECT

public:
    explicit WidokCzujnika(QWidget *parent = 0);
    ~WidokCzujnika();

private:
    Ui::WidokCzujnika *ui;
};

#endif // WIDOKCZUJNIKA_H
