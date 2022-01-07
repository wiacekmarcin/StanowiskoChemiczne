#ifndef DIGITALOUTWIDGET_H
#define DIGITALOUTWIDGET_H

#include <QWidget>
#include <QList>

class QTimer;
class QSplineSeries;

class DigitalOutWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DigitalOutWidget(QWidget *parent = nullptr);
    ~DigitalOutWidget();
    void setLevel(bool high);
    void paintEvent(QPaintEvent *paint);
protected slots:
    void timeout();
private:

    static constexpr int lenHistory = 300;
    QTimer * timer;
    bool val;
    QList<bool> vals;
    QPoint points[lenHistory];

};

#endif // DIGITALOUTWIDGET_H
