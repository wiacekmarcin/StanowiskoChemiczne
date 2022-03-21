#ifndef CZUJNIKIANALOGOWEOKNO_H
#define CZUJNIKIANALOGOWEOKNO_H

#include <QWidget>
#include "ustawienia.h"
class QGridLayout;
class QFrame;
class QHBoxLayout;

class CzujnikAnalogowyWidget;

class CzujnikiAnalogoweOkno : public QWidget
{
    Q_OBJECT

public:
    explicit CzujnikiAnalogoweOkno(QWidget *parent = 0);
    ~CzujnikiAnalogoweOkno();
    void setParams(const Ustawienia & ust);
    void setHorizontalSize(unsigned int w);
public slots:
    void updateValue(double val1, double val2, double val3, double val4, double val5, double val6, double val7, double val8);

protected:
    void createOne(int id);
private:
    CzujnikAnalogowyWidget * czujniki[Ustawienia::maxCzujekAnal];
    QGridLayout *gridLayout;

};

#endif // CZUJNIKIANALOGOWEOKNO_H
