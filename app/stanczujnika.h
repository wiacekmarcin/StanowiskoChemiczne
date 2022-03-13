#ifndef STANCZUJNIKA_H
#define STANCZUJNIKA_H

#include <QWidget>
#include <QString>
#include <QTimer>

namespace Ui {
class StanCzujnika;
}

class StanCzujnika : public QWidget
{
    Q_OBJECT

public:
    explicit StanCzujnika(QWidget *parent = 0);
    ~StanCzujnika();

    void setText(const QString & text);
    void setOk(bool ok);

    void update();
private:
    Ui::StanCzujnika *ui;
    bool isOk;
    bool red;
};

#endif // STANCZUJNIKA_H