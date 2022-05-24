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
    void addValue(float value);
    void paintEvent(QPaintEvent *paint);
    void setUnit(const QString & un);
    void setTitle(const QString & tl);
    void updateVals();
private:

    static constexpr int lenHistory = 2000;
    QTimer * timer;
    bool val;
    QList<float> vals;
    QPoint points[lenHistory];
    QString unit;
    QString title;
    float minVal;
    float maxVal;

};


#endif // DIGITALOUTWIDGET_H
