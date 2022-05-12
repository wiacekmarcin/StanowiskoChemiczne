#ifndef SYGNALYCYFROWEUSTAWIENIA_H
#define SYGNALYCYFROWEUSTAWIENIA_H

#include <QFrame>

namespace Ui {
class SygnalyCyfroweUstawienia;
}

class SygnalyCyfroweUstawienia : public QFrame
{
    Q_OBJECT

public:
    explicit SygnalyCyfroweUstawienia(QWidget *parent = nullptr);
    ~SygnalyCyfroweUstawienia();

private:
    Ui::SygnalyCyfroweUstawienia *ui;
};

#endif // SYGNALYCYFROWEUSTAWIENIA_H
