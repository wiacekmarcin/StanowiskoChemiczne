#ifndef CZUJNIKANALOGOWYWIDGET_H
#define CZUJNIKANALOGOWYWIDGET_H

#include <QWidget>
#include <QMutex>
#include <QTimer>

#include "ustawienia.h"


namespace Ui {
class CzujnikAnalogowyWidget;
}

/**
 * @brief The CzujnikAnalogowyWidget class
 * Wyświetla pojedyńczą wartość czujnika analogowego
 * @param name - nazwa wyświetlana
 * @param ratio - współczynnik (na razie nie potrzebny,
 * bo przychodząca wartość jest już przeliczona)
 * @param unit - jednostka wyświetlana
 * @param valuses - tablica wartości z których liczona jest średnia
 * @param actIndex - aktualny index
 */
class CzujnikAnalogowyWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CzujnikAnalogowyWidget(QWidget *parent = 0);
    ~CzujnikAnalogowyWidget();

    void setParam(const Ustawienia::CzujnikAnalogowy & czA);
    void setValue(const double & val);

    void setPrec(unsigned short newPrec);

private slots:
    void timeout();

private:
    Ui::CzujnikAnalogowyWidget *ui;
    QString name;
    double convert;
    QString unit;
    static constexpr unsigned short cntAvgDisp = 5;
    double values[cntAvgDisp];
    unsigned  short idx;
    unsigned short prec;
    QMutex mutex;
    QTimer timer;

    double prev[10];
    double prevAvg;
    int actId;
    bool stab;
};

#endif // CZUJNIKANALOGOWYWIDGET_H
