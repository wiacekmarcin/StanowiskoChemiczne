#ifndef PROJEKTWIDGET_H
#define PROJEKTWIDGET_H

#include <QWidget>

namespace Ui {
class ProjektWidget;
}

class ProjektWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ProjektWidget(QWidget *parent = 0);
    ~ProjektWidget();
    void setDane(const QString & nazwa, const QString &uczestnicy, const QString &katalog, const QString &komentarz, const QString &data);
private:
    Ui::ProjektWidget *ui;
};

#endif // PROJEKTWIDGET_H
