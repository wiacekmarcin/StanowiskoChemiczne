#ifndef HIGHLOWDIGITALWIDGET_H
#define HIGHLOWDIGITALWIDGET_H

#include <QWidget>

namespace Ui {
class HighLowDigitalWidget;
}

class HighLowDigitalWidget : public QWidget
{
    Q_OBJECT

public:
    explicit HighLowDigitalWidget(QWidget *parent = 0);
    ~HighLowDigitalWidget();

    void setLowText(const QString & low);
    void setHighText(const QString & high);

    bool isLow() const;
    bool isHigh() const;

    void setValue(bool val);

signals:
    void highTrig();
    void lowTrig();
    void valueChange(bool high);
private slots:
    void on_high_clicked();
    void on_low_clicked();

private:
    Ui::HighLowDigitalWidget *ui;
};

#endif // HIGHLOWDIGITALWIDGET_H
