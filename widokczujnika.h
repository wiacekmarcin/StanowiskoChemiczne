#ifndef WIDOKCZUJNIKA_H
#define WIDOKCZUJNIKA_H

#include <QWidget>

namespace Ui {
class WidokCzujnika;
}

class WidokCzujnika : public QWidget
{
    Q_OBJECT

public:
    explicit WidokCzujnika(QWidget *parent = 0);
    ~WidokCzujnika();

    void setValue(const int &);
    void setValue(const long &);
    void setValue(const long long &);
    void setValue(const float &);
    void setValue(const double &);

    void setDescription(const QString & opis, const QString & jedn);

private:
    Ui::WidokCzujnika *ui;
};

#endif // WIDOKCZUJNIKA_H
