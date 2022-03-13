#include "ustawienia.h"

#include <QVariant>
#include <QTime>
#include <QCoreApplication>

void delay(unsigned int time)
{
    QTime dieTime= QTime::currentTime().addSecs(time);
    while (QTime::currentTime() < dieTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

Ustawienia::Ustawienia()
{
    qDebug("%s %d",__FILE__,__LINE__);

    for (int i = 1; i < maxCzujekAnal; ++i) {
        czujki[i-1].name = settings.value(QString("czujnikianalogowe/%1/name").arg(i), QString("Czujnik analogowy %1").arg(i)).toString();
        czujki[i-1].unit = settings.value(QString("czujnikianalogowe/%1/unit").arg(i), QString("jedn.")).toString();
        czujki[i-1].ratio = settings.value(QString("czujnikianalogowe/%1/unit").arg(i), 1.0).toDouble();
    }
    qDebug("%s %d",__FILE__,__LINE__);

    for (int i = 1; i <= maxCzujekCyfrIn; ++i) {
        wejscia[i-1] = settings.value(QString("wejsciacyfrowe/%1/name").arg(i), QString("Wejscie cyfrowe %1").arg(i)).toString();
    }
    qDebug("%s %d",__FILE__,__LINE__);

    for (int i = 1; i <= maxCzujekCyfrOut; ++i) {
        wyjscia[i-1] = settings.value(QString("wyjsciacyfrowe/%1/name").arg(i), QString("Wyjscie cyfrowe %1").arg(i)).toString();
    }
    qDebug("%s %d",__FILE__,__LINE__);

    reverseMotors = settings.value(QString("dozownik/rerverseMotor")).toBool();
    maxImp = settings.value(QString("dozownik/maxImpMotor")).toInt();

    qDebug("%s %d",__FILE__,__LINE__);

}

void Ustawienia::setCzujka(short id, const QString &name, const QString &unit, const double &ratio)
{
    if (id > maxCzujekAnal || id <= 0)
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
    if (id > maxCzujekAnal || id <= 0)
        return Ustawienia::CzujnikAnalogowy();
    return czujki[id-1];
}

QString Ustawienia::getName(short id) const
{
    if (id > maxCzujekAnal || id <= 0)
        return QString();
    return czujki[id-1].name;
}

QString Ustawienia::getUnit(short id) const
{
    if (id > maxCzujekAnal || id <= 0)
        return QString();
    return czujki[id-1].unit;
}

double Ustawienia::getRatio(short id) const
{
    if (id > maxCzujekAnal || id <= 0)
        return 0.0;
    return czujki[id-1].ratio;
}

void Ustawienia::setWejscie(int id, const QString &name)
{
    if (id > maxCzujekCyfrIn || id <= 0)
        return;
    wejscia[id-1] = name;
    settings.setValue(QString("wejsciacyfrowe/%1/name").arg(id), QVariant::fromValue(name));
}

QString Ustawienia::wejscie(int id) const
{
    if (id > maxCzujekCyfrIn || id <= 0)
        return QString();
    return wejscia[id-1];
}

void Ustawienia::setWyjscie(int id, const QString &name)
{
    if (id > maxCzujekCyfrOut || id <= 0)
        return;
    wyjscia[id-1] = name;
    settings.setValue(QString("wyjsciacyfrowe/%1/name").arg(id), QVariant::fromValue(name));
}

QString Ustawienia::wyjscie(int id) const
{
    if (id > maxCzujekCyfrOut || id <= 0)
        return QString();
    return wyjscia[id-1];
}

bool Ustawienia::getReverseMotors() const
{
    return reverseMotors;
}

void Ustawienia::setReverseMotors(bool newReverseMotors)
{
    reverseMotors = newReverseMotors;
    settings.setValue(QString("dozownik/rerverseMotor"), QVariant::fromValue(newReverseMotors));
}

int Ustawienia::getMaxImp() const
{
    return maxImp;
}

void Ustawienia::setMaxImp(int newMaxImp)
{
    maxImp = newMaxImp;
    settings.setValue(QString("dozownik/maxImpMotor"), QVariant::fromValue(newMaxImp));
}
