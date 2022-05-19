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
    void updateVals();
private:

    static constexpr int lenHistory = 1900;
    QTimer * timer;
    bool val;
    QList<float> vals;
    QPoint points[lenHistory];
    QString unit;
    float minVal;
    float maxVal;

};


#endif // DIGITALOUTWIDGET_H
