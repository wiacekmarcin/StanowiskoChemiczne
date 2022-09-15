#ifndef DOUBLELINEEDIT_H
#define DOUBLELINEEDIT_H

#include <QObject>
#include <QWidget>
#include <QLineEdit>
#include <QDoubleValidator>
#include <QValidator>
#include <QDebug>
#include <QPalette>

class MyValidator : public QDoubleValidator
{
    public:
    MyValidator(int prec, double min, double max) :
        QDoubleValidator(min, max, prec)
    { }

    QValidator::State validate(QString& input, int& pos) const
    {
        if (input.count(QChar(',')) > 1 || input.count(QChar('.')))
            return QValidator::Invalid;
        if( input.contains( QChar(',') ) ) {
            input.replace(QChar(','), QChar('.'));
            return QValidator::Intermediate;
        }
        if (input.contains(QChar('.'))) {
            return QValidator::Intermediate;
        }
        auto state = QDoubleValidator::validate(input,pos);
        return state;
    }

    void fixup(QString & input) const
    {
         QDoubleValidator::fixup(input);
         input.replace(QChar(','), QChar('.'));
    }
};

class DoubleLineEdit : public QLineEdit
{
    Q_OBJECT

    Q_PROPERTY(int prec READ getPrec WRITE setPrec NOTIFY precChange)
    Q_PROPERTY(double max READ getMax WRITE setMax NOTIFY maxChange)
    Q_PROPERTY(double min READ getMin WRITE setMin NOTIFY minChange)


signals:
    void precChange(int);
    void maxChange(double max);
    void minChange(double min);
    void valueChanged(double val);

public:
    DoubleLineEdit(QWidget * parent = nullptr);

    double value() const;
    void setValue(const double& newVal);

    int getPrec() const;
    void setPrec(const int& newVal);

    double getMin() const;
    void setMin(const double& newVal);

    double getMax() const;
    void setMax(const double& newVal);

    bool getValid() const;
    void setValid(bool newValid);

private slots:
    void valueChangedSlot(const QString & val);
private:
    double val;
    int prec;
    double min;
    double max;
    MyValidator *validator;
    QPalette p_ok;
    QPalette p_error;

    bool valid;
};

#endif // DOUBLELINEEDIT_H
