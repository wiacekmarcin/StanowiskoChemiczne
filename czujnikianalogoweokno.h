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
public slots:
    void updateValue(int id, const double & val);
signals:
    void valueChange(int id, const double & val);

protected:
    void createOne(int id);
private:
    CzujnikAnalogowyWidget * czujniki[Ustawienia::maxCzujek];
    QGridLayout *gridLayout;

};

#endif // CZUJNIKIANALOGOWEOKNO_H
