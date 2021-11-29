#ifndef OKNOCZUJNIKOW_H
#define OKNOCZUJNIKOW_H

#include <QWidget>

namespace Ui {
class OknoCzujnikow;
}

class OknoCzujnikow : public QWidget
{
    Q_OBJECT

public:
    explicit OknoCzujnikow(QWidget *parent = 0);
    ~OknoCzujnikow();

private:
    Ui::OknoCzujnikow *ui;
};

#endif // OKNOCZUJNIKOW_H
