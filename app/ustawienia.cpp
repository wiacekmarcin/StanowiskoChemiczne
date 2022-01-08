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
    if (id > maxCzujek || id <= 0)
        return;
    czujki[id-1].name = name;
    czujki[id-1].unit = unit;
    czujki[id-1].ratio = ratio;
    settings.setValue(QString("czujnikianalogowe/%1/name").arg(id), QVariant::fromValue(name));
    settings.setValue(QString("czujnikianalogowe/%1/unit").arg(id), QVariant::fromValue(unit));
    settings.setValue(QString("czujnikianalogowe/%1/ratio").arg(id), QVariant::fromValue(ratio));
}

Ustawienia::CzujnikAnalogowy Ustawienia::getCzujka(short id) const
{
    if (id > maxCzujek || id <= 0)
        return Ustawienia::CzujnikAnalogowy();
    return czujki[id-1];
}

QString Ustawienia::getName(short id) const
{
    if (id > maxCzujek || id <= 0)
        return QString();
    return czujki[id-1].name;
}

QString Ustawienia::getUnit(short id) const
{
    if (id > maxCzujek || id <= 0)
        return QString();
    return czujki[id-1].unit;
}

double Ustawienia::getRatio(short id) const
{
    if (id > maxCzujek || id <= 0)
        return 0.0;
    return czujki[id-1].ratio;
}

void Ustawienia::setWejscie(int id, const QString &name)
{
    if (id > 10 || id <= 0)
        return;
    wejscia[id-1] = name;
    settings.setValue(QString("wejsciacyfrowe/%1/name").arg(id), QVariant::fromValue(name));
}

QString Ustawienia::wejscie(int id) const
{
    if (id > 10 || id <= 0)
        return QString();
    return wejscia[id-1];
}

void Ustawienia::setWyjscie(int id, const QString &name)
{
    if (id > 11 || id <= 0)
        return;
    wyjscia[id-1] = name;
    settings.setValue(QString("wyjsciacyfrowe/%1/name").arg(id), QVariant::fromValue(name));
}

QString Ustawienia::wyjscie(int id) const
{
    if (id > 11 || id <= 0)
        return QString();
    return wyjscia[id-1];
}
