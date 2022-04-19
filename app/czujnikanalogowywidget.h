#ifndef CZUJNIKANALOGOWYWIDGET_H
#define CZUJNIKANALOGOWYWIDGET_H

#include <QWidget>
#include <QMutex>
#include <QTimer>
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

    void setParam(const QString & name, const double & ratio, const QString & unit);
    void setValue(const double & val);

    void setPrec(unsigned short newPrec);

private slots:
    void timeout();

private:
    Ui::CzujnikAnalogowyWidget *ui;
    QString name;
    double ratio;
    QString unit;
    static constexpr unsigned short maxPoz = 5;
    double values[maxPoz];
    unsigned  short idx;
    unsigned short prec;
    QMutex mutex;
    QTimer timer;
};

#endif // CZUJNIKANALOGOWYWIDGET_H
