#ifndef DISPUSHBUTTON_H
#define DISPUSHBUTTON_H

#include <QObject>
#include <QPushButton>
#include <QWidget>

class DisPushButton : public QPushButton
{
public:
    DisPushButton(QWidget * parent = nullptr);
    void paintEvent(QPaintEvent *paint);
    void setDone(bool en);
    bool isDone() const;
private:
    bool dis;
};

#endif // DISPUSHBUTTON_H
