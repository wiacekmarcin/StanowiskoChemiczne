#include "doublelineedit.h"
#include <QDoubleValidator>
DoubleLineEdit::DoubleLineEdit(QWidget * parent)
    : QLineEdit(parent),
      prec(0), min(0), max(100),
      validator(new MyValidator(min, max, prec))
{
    this->setValidator(validator);
    connect(this, &QLineEdit::textChanged, this, &DoubleLineEdit::valueChangedSlot);
}

double DoubleLineEdit::value() const
{
    return val;
}

void DoubleLineEdit::setValue(const double &newVal)
{
    val = newVal;
}

int DoubleLineEdit::getPrec() const
{
    return prec;
}

void DoubleLineEdit::setPrec(const int &newVal)
{
    prec = newVal;
    validator->setDecimals(newVal);
}

double DoubleLineEdit::getMin() const
{
    return min;
}

void DoubleLineEdit::setMin(const double &newVal)
{
    min = newVal;
    validator->setBottom(newVal);
}

double DoubleLineEdit::getMax() const
{
    return max;
}

void DoubleLineEdit::setMax(const double &newVal)
{
    max = newVal;
    validator->setTop(newVal);
}

void DoubleLineEdit::valueChangedSlot(const QString &val)
{
    bool ok;
    this->val = val.toDouble(&ok);
    if (ok)
        emit valueChanged(this->val);
}
