#ifndef CZUJNIKANALOGOWYUSTAWIENIAWIDGET_H
#define CZUJNIKANALOGOWYUSTAWIENIAWIDGET_H

#include <QWidget>

namespace Ui {
class CzujnikAnalogowyUstawieniaWidget;
}

class CzujnikAnalogowyUstawieniaWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CzujnikAnalogowyUstawieniaWidget(QWidget *parent = 0);
    ~CzujnikAnalogowyUstawieniaWidget();
    QString name() const;
    QString unit() const;
    double ratio() const;
    bool valid();
    void setData(const QString & name, const QString & unit, const double & ratio);
private slots:
    void textChange();
signals:
    void updateCzujnik();

private:
    Ui::CzujnikAnalogowyUstawieniaWidget *ui;
    double rr;
};

#endif // CZUJNIKANALOGOWYUSTAWIENIAWIDGET_H
