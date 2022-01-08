#ifndef SYGNALANALPARAM_H
#define SYGNALANALPARAM_H

#include <QObject>

class SygnalAnalParam : public QObject
{
    Q_OBJECT

public:
    SygnalAnalParam();

    QString getName() const;
    void setName(const QString &value);

    double getRatio() const;
    void setRatio(double value);

    QString getUnit() const;
    void setUnit(const QString &value);

signals:
    void changeName(const QString & name);
    void changeUnit(const QString & unit);
    void changeRatio(const double & ratio);

private:
    QString name;
    double ratio;
    QString unit;
};

#endif // SYGNALANALPARAM_H
