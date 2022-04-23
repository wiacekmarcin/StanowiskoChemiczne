#include "testdata.h"

#include <QPolygon>
#include <QPoint>
#include <QPixmap>
#include <QPainter>
#include <QByteArray>
#include <QBuffer>

constexpr char TestData::title1[];
constexpr char TestData::title2[];
constexpr char TestData::temat[];
constexpr char TestData::tematValue[];
constexpr char TestData::dataWykonania[];
constexpr char TestData::skladzespolu[];
constexpr char TestData::nazwacieczy[];
constexpr char TestData::tempKomoraStartTest[];
constexpr char TestData::wilgotnosc[];
constexpr char TestData::proba[];
constexpr char TestData::probaWynik[];
constexpr char TestData::probaPozytywna[];
constexpr char TestData::probaNegatywna[];
constexpr char TestData::zrodloZaplonu[];
constexpr char TestData::iloscCieczy[];
constexpr char TestData::warunkiPoczatkowe[];
constexpr char TestData::warunkiPrzedZaplonem[];
constexpr char TestData::warunkiPoUdanejProba[];
constexpr char TestData::tempParownika[];
constexpr char TestData::tempKomory[];
constexpr char TestData::cisnKomory[];
constexpr char TestData::koncetracjaPar[];
constexpr char TestData::zlaKoncetracjaPar[];
constexpr char TestData::odczytyStezen[];

TestData::TestData()
{
    d.dataTestu = QDateTime::currentDateTime();
    d.uczestnicy << "Jan Kowalski" << "Leon Zimowiec" << "Klaudia Ładna";
    d.nazwaCieczy = "Woda";
    d.wilgotnosc = 75.5;
    d.tempKomoraPoczatek = 25.3;

    d.iloscCieczy << 21.4 << 0 << 15.0;

    ProbaType p1;
    p1.success = false;
    p1.zrodloZaplonu = "Iskra mechaniczna";
    p1.powtarzanyZaplon = false;
    p1.powtarzaneDozowanie = false;
    p1.cisnKomoryDozowanie = 1.0;
    p1.tempKomoryDozowanie = 31.0;
    p1.tempParownikaDozowanie = 34.0;
    p1.tempKomoryZaplon = 40.9;
    p1.cisnKomoryZaplon = 1.4;
    p1.koncentracjaPar = 32.7;
    p1.cisnKomoryDozowanie = 1.2;
    p1.tempKomoryDozowanie = 30.0;
    p1.zlaKoncetracja = false;
    p1.voc1 = 15.6;
    p1.voc2 = 16.7;
    p1.o2 = 26.7;
    p1.co2 = 32.0;
    p1.cz8 = 21;

    ProbaType p2;
    p2.success = false;
    p2.zrodloZaplonu = "Iskra elektryczna";
    p2.powtarzanyZaplon = true;
    p2.powtarzaneDozowanie = false;
    p2.cisnKomoryDozowanie = 1.0;
    p2.tempKomoryDozowanie = 31.0;
    p2.tempParownikaDozowanie = 34.0;
    p2.cisnKomoryDozowanie = 0.9;
    p2.tempKomoryDozowanie = 31.1;
    p2.tempKomoryZaplon = 10.9;
    p2.cisnKomoryZaplon = 2.4;
    p2.koncentracjaPar = 42.7;
    p2.zlaKoncetracja = false;
    p2.voc1 = 11.6;
    p2.voc2 = 12.7;
    p2.o2 = 22.7;
    p2.co2 = 22.0;
    p2.cz8 = 22;

    ProbaType p3;
    p3.success = true;
    p3.zrodloZaplonu = "Płomień";
    p3.powtarzanyZaplon = false;
    p3.powtarzaneDozowanie = false;
    p3.cisnKomoryDozowanie = 1.1;
    p3.tempKomoryDozowanie = 33.0;
    p3.tempParownikaDozowanie = 35.0;
    p3.cisnKomoryDozowanie = 1.3;
    p3.tempKomoryDozowanie = 31.1;
    p3.zlaKoncetracja = true;
    p3.tempKomoryZaplon = 34.9;
    p3.cisnKomoryZaplon = 1.0;
    p3.koncentracjaPar = 22.7;
    p3.voc1 = 31.6;
    p3.voc2 = 32.7;
    p3.o2 = 32.7;
    p3.co2 = 32.0;
    p3.cz8 = 32;

    d.proby << p1 << p2 << p3;
}

QString TestData::getBody() const
{
    QString result = QString("<html><body>") + getTitle();

    result += getTemat();
    result += QString("<div>%1 %2</div>").arg(getData(), getUczestnicy());
    result += getNazwaCieczy();
    result += getTempKomoraStart();
    result += getWilgotnosc();

    for (int id = 0; id < d.proby.size(); ++id) {
        if (id) {
            result += QString("<p>#######################################################################</p>");
        }
        result += getProba(d.proby.at(id), d.iloscCieczy, id + 1);
    }
    result += QString("</body></html>");
    return result;
}

void TestData::setTemperaturaKomory(FazaTestu ft, const float &temp)
{
    switch(ft) {
    case FT_poczatek : d.tempKomoraPoczatek = temp; break;
    case FT_koniec: d.tempKomoraKoniec = temp; break;
    case FT_dozowanie: d.proby.last().tempParownikaDozowanie = temp; break;
    case FT_zaplon: d.proby.last().tempKomoryZaplon = temp; break;
    default : break ;
    }
}

void TestData::setCisnienieKomory(FazaTestu ft, const float &cisn)
{
    switch(ft) {
    case FT_koniec: d.cisnienieKoniec = cisn; break;
    case FT_dozowanie: d.proby.last().cisnKomoryDozowanie = cisn; break;
    case FT_zaplon: d.proby.last().cisnKomoryZaplon = cisn; break;
    default : break ;
    }
}

void TestData::setStezenia(FazaTestu ft, const float &voc1, const float &voc2, const float &o2, const float &co2, const float &cz8)
{
    switch(ft) {
    case FT_koniec: {
        d.voc1 = voc1;
        d.voc2 = voc2;
        d.o2 = o2;
        d.co2 = co2;
        d.cz8 = cz8;
        break;
    }
    case FT_przedZaplon:
    {
        d.proby.last().voc1 = voc1;
        d.proby.last().voc2 = voc2;
        d.proby.last().o2 = o2;
        d.proby.last().co2 = co2;
        d.proby.last().cz8 = cz8;
    }
    default : break ;
    }
}

void TestData::setUdanaProba(bool success)
{
    d.proby.last().success = success;
    if (success) {
        setPowtarzanieDozowanie(false);
        setPowtarzanieZaplonu(false);
    }
    else
        d.proby.append(ProbaType());

}

QString TestData::getTitle()
{
    return QString("<div align=\"center\">%1%2</div>").arg(getTitle1(), getTitle2());
}

QString TestData::getUczestnicy() const
{
    QString result = "";
    result += QString("<p>%1<ol>").arg(TestData::skladzespolu);

    foreach(auto u , d.uczestnicy) {
        result += QString("<li>%1</li>").arg(u);
    }

    result += "</ol></p>";
    return result;
}

QString TestData::getNazwaCieczy() const
{
    return QString("<p>%1 %2</p>").arg(TestData::nazwacieczy, d.nazwaCieczy);
}

QString TestData::getTempKomoraStart() const
{
    return QString("<p>%1 %2 &#x2103;</p>").arg(QString(TestData::tempKomoraStartTest), QString::number(d.tempKomoraPoczatek, 'f', 1));
}

QString TestData::getWilgotnosc() const
{
    return QString("<p>%1 %2 %RH</p>").arg(QString(TestData::wilgotnosc), QString::number(d.wilgotnosc, 'f', 1));
}

QString TestData::getProba(const ProbaType &proba, const QList<float> & dozowanie, unsigned int nrProba) const
{
    QString ret("<div style=\"page-break-before:always\">");
    ret += QString("<h2>%1 %2</h2>").arg(QString(TestData::proba), QString::number(nrProba));
    ret += getProbaWynik(proba.success);
    ret += getZrodloZaplonu(proba.zrodloZaplonu);
    if (proba.powtarzanyZaplon && !proba.success)
        return ret + QString("</div>");
    ret += getIloscCieczy(dozowanie, nrProba);
    ret += getWarunkiPoczatkowe(proba);
    ret += getWarunkiPrzedZaplonem(proba);
    ret += getOdczytyStezen(proba);
    return ret + QString("</div>");
}

QString TestData::getProbaWynik(bool wynik) const
{
    return QString("<p>%1 %2</p>").arg(QString(TestData::probaWynik),
                                       (wynik ? QString(TestData::probaPozytywna) : QString(TestData::probaNegatywna)));
}

QString TestData::getZrodloZaplonu(const QString &zaplon) const
{
    return QString("<p>%1 %2</p>").arg(QString(TestData::zrodloZaplonu), zaplon);
}

QString TestData::getIloscCieczy(const QList<float> &dozowanie, unsigned int nrProba) const
{
    if (nrProba == 1) {
        return QString("<p>%1 %2 ml</p>").arg(QString(TestData::iloscCieczy), QString::number(dozowanie.at(0),'f', 1));
    }

    float suma = 0.0;
    QString sumaStr;
    unsigned int sumCnt = 0;
    for (unsigned int i = 0; i < nrProba; ++i) {
        float d = dozowanie.at(i);
        suma += d;
        if (d == 0.0)
            continue;
        sumaStr += QString("+ %1 ").arg(QString::number(d, 'f', 1));
        ++sumCnt;
    }
    sumaStr.remove(0,1);
    sumaStr += QString("= %1").arg(QString::number(suma, 'f', 1));
    if (sumCnt == 1) {
        return QString("<p>%1 %2 ml</p>").arg(QString(TestData::iloscCieczy), QString::number(dozowanie.at(0),'f', 1));
    }
    return QString("<p>%1 %2 ml</p>").arg(QString(TestData::iloscCieczy), sumaStr);
}

QString TestData::getWarunkiPoczatkowe(const ProbaType &proba) const
{
    QString ret = QString("<h3>%1</h3>").arg(QString(TestData::warunkiPoczatkowe));
    ret += QString("<p>%1 %2 &#x2103;").arg(QString(TestData::tempParownika), QString::number(proba.tempParownikaDozowanie, 'f', 1));
    ret += QString("<p>%1 %2 &#x2103;").arg(QString(TestData::tempKomory), QString::number(proba.tempKomoryDozowanie, 'f', 1));
    ret += QString("<p>%1 %2 mBar").arg(QString(TestData::cisnKomory), QString::number(10*proba.cisnKomoryDozowanie, 'f', 1));
    return ret;
}

QString TestData::getWarunkiPrzedZaplonem(const ProbaType &proba) const
{
    QString ret = QString("<h3>%1</h3>").arg(QString(TestData::warunkiPrzedZaplonem));
    ret += QString("<p>%1 %2 &#x2103;").arg(QString(TestData::tempKomory), QString::number(proba.tempKomoryZaplon, 'f', 1));
    ret += QString("<p>%1 %2 mBar").arg(QString(TestData::cisnKomory), QString::number(10*proba.cisnKomoryZaplon, 'f', 1));
    ret += QString("<p>%1 %2 %").arg(QString(TestData::koncetracjaPar), QString::number(proba.koncentracjaPar, 'f', 1));
    if (proba.zlaKoncetracja) {
        ret += QString("<p><u>%1</u></p>").arg(TestData::zlaKoncetracjaPar);
    }
    return ret;
}

QString TestData::getOdczytyStezen(const ProbaType &proba) const
{
    QString ret = QString("<p>%1").arg(TestData::odczytyStezen);
    ret += QString("<ul>");
    ret += QString("<li>VOC1 = %1 %</li>").arg(QString::number(proba.voc1, 'f', 1));
    ret += QString("<li>VOC2 = %1 %</li>").arg(QString::number(proba.voc2, 'f', 1));
    ret += QString("<li>O2 = %1 %</li>").arg(QString::number(proba.o2, 'f', 1));
    ret += QString("<li>CO2 = %1 %</li>").arg(QString::number(proba.co2, 'f', 1));
    ret += QString("<li>Czujnik 8 = %1 %</li>").arg(QString::number(proba.cz8, 'f', 1));
    return ret + QString("</ul></p>");
}

QString TestData::getWarunkiPoUdanejProbie() const
{
    QString ret = QString("<h3>%1</h3>").arg(QString(TestData::warunkiPoUdanejProba));
    ret += QString("<p>%1 %2 &#x2103;").arg(QString(TestData::tempKomory), QString::number(d.tempKomoraKoniec, 'f', 1));
    ret += QString("<p>%1 %2 mBar").arg(QString(TestData::cisnKomory), QString::number(10*d.cisnienieKoniec, 'f', 1));
    ret += QString("<p>%1").arg(TestData::odczytyStezen);
    ret += QString("<ul>");
    ret += QString("<li>VOC1 = %1 %</li>").arg(QString::number(d.voc1, 'f', 1));
    ret += QString("<li>VOC2 = %1 %</li>").arg(QString::number(d.voc2, 'f', 1));
    ret += QString("<li>O2 = %1 %</li>").arg(QString::number(d.o2, 'f', 1));
    ret += QString("<li>CO2 = %1 %</li>").arg(QString::number(d.co2, 'f', 1));
    ret += QString("<li>Czujnik 8 = %1 %</li>").arg(QString::number(d.cz8, 'f', 1));
    return ret + QString("</ul></p>");
}

QString TestData::getImageUrl(int id)
{
    QPoint * points = new QPoint[values.size()];
    unsigned int px = 0;
    foreach(auto value, values) {
        points[px]= (QPoint(px, (unsigned int)10*(value[id])));
        ++px;
    }

    QPixmap *pix = new QPixmap(px, 1000);
    QPainter *paint = new QPainter(pix);
    paint->setPen(*(new QColor(255,34,255,255)));
    paint->setBrush(Qt::white);
    QRect rect = QRect(0, 0, px, 1000);
    paint->fillRect(rect, QBrush(Qt::white));
    paint->drawRect(rect);//5 radius apiece
    paint->setBrush(Qt::black);
    paint->drawPoints(points, px);

    QByteArray byteArray;
    QBuffer buffer(&byteArray);
    pix->save(&buffer, "PNG");

    return QString("<img src=\"data:image/png;base64,") + byteArray.toBase64() + "\"/>";
}

const QString &TestData::getNazwaTestu() const
{
    return nazwaTestu;
}

void TestData::setNazwaTestu(const QString &newNazwaTestu)
{
    nazwaTestu = newNazwaTestu;
}

void TestData::setListValues(const QList<QVector<float> > &values)
{
    this->values = values;
}
