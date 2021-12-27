#include "ustawienia.h"

#include <QVariant>

Ustawienia::Ustawienia()
{
    for (int i = 1; i <= maxCzujek; ++i) {
        czujki[i-1].name = settings.value(QString("czujnikianalogowe/%1/name").arg(i), QString("Czujnik analogowy %1").arg(i)).toString();
        czujki[i-1].unit = settings.value(QString("czujnikianalogowe/%1/unit").arg(i), QString("jedn.")).toString();
        czujki[i-1].ratio = settings.value(QString("czujnikianalogowe/%1/unit").arg(i), 1.0).toDouble();
    }
}

void Ustawienia::setCzujka(short id, const QString &name, const QString &unit, const double &ratio)
{
    if (id > 8 || id <= 0)
        return;
    czujki[id-1].name = name;
    czujki[id-1].unit = unit;
    czujki[id-1].ratio = ratio;
    settings.setValue(QString("czujnikianalogowe/%1/name").arg(id), QVariant::fromValue(name));
    settings.setValue(QString("czujnikianalogowe/%1/unit").arg(id), QVariant::fromValue(name));
    settings.setValue(QString("czujnikianalogowe/%1/ratio").arg(id), QVariant::fromValue(name));
}

Ustawienia::CzujnikAnalogowy Ustawienia::getCzujka(short id) const
{
    if (id > 8 || id <= 0)
        return Ustawienia::CzujnikAnalogowy();
    return czujki[id-1];
}

QString Ustawienia::getName(short id) const
{
    if (id > 8 || id <= 0)
        return QString();
    return czujki[id-1].name;
}

QString Ustawienia::getUnit(short id) const
{
    if (id > 8 || id <= 0)
        return QString();
    return czujki[id-1].unit;
}

double Ustawienia::getRatio(short id) const
{
    if (id > 8 || id <= 0)
        return 0.0;
    return czujki[id-1].ratio;
}

