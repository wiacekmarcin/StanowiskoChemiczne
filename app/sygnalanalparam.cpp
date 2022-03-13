#include "sygnalanalparam.h"

SygnalAnalParam::SygnalAnalParam()
{

}

QString SygnalAnalParam::getName() const
{
    return name;
}

void SygnalAnalParam::setName(const QString &value)
{
    name = value;
    emit changeName(name);
}

double SygnalAnalParam::getRatio() const
{
    return ratio;
}

void SygnalAnalParam::setRatio(double value)
{
    ratio = value;
    emit changeRatio(value);
}

QString SygnalAnalParam::getUnit() const
{
    return unit;
}

void SygnalAnalParam::setUnit(const QString &value)
{
    unit = value;
    emit changeUnit(value);
}
