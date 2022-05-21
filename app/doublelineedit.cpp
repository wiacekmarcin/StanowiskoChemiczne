#include "doublelineedit.h"
#include <QDoubleValidator>
#include <QStyle>

DoubleLineEdit::DoubleLineEdit(QWidget * parent)
    : QLineEdit(parent),
      prec(0), min(0), max(100),
      validator(new MyValidator(min, max, prec))
{
    this->setValidator(validator);
    connect(this, &QLineEdit::textChanged, this, &DoubleLineEdit::valueChangedSlot);
    p_ok = this->palette();
    p_error = this->palette();
    p_error.setColor(QPalette::Base, Qt::red);
}

double DoubleLineEdit::value() const
{
    return val;
}

void DoubleLineEdit::setValue(const double &newVal)
{
    val = newVal;
    setText(QString::number(val));
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
    if (ok) {
        if (this->val < this->min) {
            this->val = this->min;
            this->setPalette(p_error);
            return;
        } else if (this->val > this->max) {
            this->val = max;
            this->setPalette(p_error);
            return;
        } else {
            this->setPalette(p_ok);
            emit valueChanged(this->val);
        }
     } else {
         this->setPalette(p_error);
    }
}
