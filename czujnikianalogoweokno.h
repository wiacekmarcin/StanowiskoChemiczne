#ifndef CZUJNIKIANALOGOWEOKNO_H
#define CZUJNIKIANALOGOWEOKNO_H

#include <QWidget>

namespace Ui {
class CzujnikiAnalogoweOkno;
}
class CzujnikAnalogowyWidget;

class CzujnikiAnalogoweOkno : public QWidget
{
    Q_OBJECT

public:
    explicit CzujnikiAnalogoweOkno(QWidget *parent = 0);
    ~CzujnikiAnalogoweOkno();
public slots:
    void updateValue(int id, const double & val);
signals:
    void valueChange(int id, const double & val);
private:
    Ui::CzujnikiAnalogoweOkno *ui;
    CzujnikAnalogowyWidget * czujniki[6];
};

#endif // CZUJNIKIANALOGOWEOKNO_H
