#ifndef CZUJNIKANALOGOWYWIDGET_H
#define CZUJNIKANALOGOWYWIDGET_H

#include <QWidget>

namespace Ui {
class CzujnikAnalogowyWidget;
}

class CzujnikAnalogowyWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CzujnikAnalogowyWidget(QWidget *parent = 0);
    ~CzujnikAnalogowyWidget();

    void setParam(const QString & name, const double & ratio, const QString & unit);
    void setValue(const double & val);

    double getConvValue() const { return valConv; }
private:
    Ui::CzujnikAnalogowyWidget *ui;
    QString name;
    double ratio;
    QString unit;
    double valConv;
};

#endif // CZUJNIKANALOGOWYWIDGET_H
