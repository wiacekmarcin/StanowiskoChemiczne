#include "testdata.h"

#include <QPolygon>
#include <QPoint>
#include <QPixmap>
#include <QPainter>
#include <QByteArray>
#include <QBuffer>
#include <QDebug>


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

TestData::TestData() : QObject(nullptr)
{

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
    result += getWarunkiPoUdanejProbie();
    result += getImageUrl(1);
    result += getImageUrl(2);
    result += getImageUrl(3);
    result += getImageUrl(4);
    result += getImageUrl(5);
    result += getImageUrl(6);
    result += getImageUrl(7);
    result += getImageUrl(8);
    result += QString("</body></html>");
    return result;
}

void TestData::setTemperaturaKomory(FazaTestu ft, const float &temp)
{
    emit debug(QString("Ustaw Temp ft = %1 temp = %2").arg(ft).arg(temp));
    switch(ft) {
    case FT_poczatek :  d.tempKomoraPoczatek = temp; break;
    case FT_koniec:     d.tempKomoraKoniec = temp; break;
    case FT_dozowanie:  d.proby.last().tempKomoryDozowanie = temp; break;
    case FT_przedZaplon:  d.proby.last().tempKomoryZaplon = temp; break;
    case FT_zaplon: d.proby.last().tempKomoryZaplon = temp; break;
    default :  qInfo () << "Invalid ft=" << ft ; break ;
    }
}

void TestData::setCisnienieKomory(FazaTestu ft, const float &cisn)
{
    emit debug(QString("Ustaw ciśnienie ft = %1 cisn = %2").arg(ft).arg(cisn));
    switch(ft) {
    case FT_koniec: d.cisnienieKoniec = cisn; break;
    case FT_dozowanie: d.proby.last().cisnKomoryDozowanie = cisn; break;
    case FT_przedZaplon: d.proby.last().cisnKomoryZaplon = cisn; break;
    case FT_zaplon: d.proby.last().cisnKomoryZaplon = cisn; break;
    default :  qInfo () << "Invalid ft=" << ft ; break ;
    }
}

void TestData::setStezenia(FazaTestu ft, const float &voc1, const float &voc2, const float &o2, const float &co2, const float &cz8)
{
    emit debug(QString("Ustaw stezenia ft = %1 %2,%3,%4,%5,%6").arg(ft).arg(voc1).arg(voc2).arg(o2).arg(co2).arg(cz8));
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
    default : qInfo () << "Invalid ft=" << ft ;  break ;
    
    }
}

void TestData::setUdanaProba(bool success, bool powtarzaneDozowanie, bool powtarzanyZaplon)
{
    emit debug(QString("Udana proba = %1").arg(success));
    d.proby.last().success = success;
    if (!success) {
        d.proby.append(ProbaType());
        d.proby.last().powtarzaneDozowanie = powtarzaneDozowanie;
        d.proby.last().powtarzanyZaplon =  powtarzanyZaplon;
    }
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
    return QString("<p style=\"page-break-after:always\">%1 %2 %RH</p>").arg(QString(TestData::wilgotnosc), QString::number(d.wilgotnosc, 'f', 1));
}

QString TestData::getProba(const ProbaType &proba, const QList<float> & dozowanie, unsigned int nrProba) const
{
    qDebug() << proba.cisnKomoryDozowanie << " " << proba.cisnKomoryZaplon << " " << proba.powtarzaneDozowanie << " "
             << proba.powtarzanyZaplon << " " << proba.success << " " << proba.co2 << " " << proba.cz8 << " "
             << proba.o2 << " " << proba.tempKomoryDozowanie << " " << proba.tempKomoryZaplon << " "
             << proba.tempParownikaDozowanie << " " << proba.voc1 << " " << proba.voc2 << " " << proba.zrodloZaplonu;
    QString ret("<div>");
    ret += QString("<h2>%1 %2</h2>").arg(QString(TestData::proba), QString::number(nrProba));
    ret += getProbaWynik(proba.success);
    ret += getZrodloZaplonu(proba.zrodloZaplonu);
    if (proba.powtarzanyZaplon)
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
    if (nrProba == 1 || dozowanie.size() == 1) {
        return QString("<p>%1 %2 ml</p>").arg(QString(TestData::iloscCieczy), QString::number(dozowanie.at(0),'f', 1));
    }

    float suma = 0.0;
    QString sumaStr;
    unsigned int sumCnt = 0;
    for (unsigned int i = 0; i < dozowanie.size(); ++i) {
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


QString TestData::getImageUrl(int id) const
{
    QPoint * points = new QPoint[6000];
    unsigned int px = 0;
    //for(px = 0; px < 6000; ++px) {
    //    points[px]= QPoint(px, 10* (px % 100));
    //}
    QSize A4Size = QSize(600, 850);
    //QPixmap *pix = new QPixmap(px, 1000);
    QPixmap *pix = new QPixmap(A4Size.width(), A4Size.height());
    QPainter *paint = new QPainter(pix);

    paint->setBrush(Qt::white);
    QRect rect = QRect(0, 0, A4Size.width() - 1, A4Size.height() - 1);
    paint->fillRect(rect, QBrush(Qt::white));
    paint->drawRect(rect);//5 radius apiece

    QPen grayPen = QPen(Qt::gray);
    grayPen.setWidth(1);
    paint->setPen(grayPen);
    for (int i = 0; i <= 850; i+=20) {
        if (i % 100 == 0)
            continue;
        paint->drawLine(0, i, 600, i);
        if (i < 600)
            paint->drawLine(i, 0, i, 850);
    }


    QPen darkgrayPen = QPen(Qt::darkGray);
    darkgrayPen.setWidth(3);
    paint->setPen(darkgrayPen);
    for (int i = 0; i <= 850; i+=100) {
        paint->drawLine(0, i, 600, i);
        if (i <= 600)
            paint->drawLine(i, 0, i, 850);
    }



    paint->setBrush(Qt::black);
    QPen pen = QPen(Qt::black);
    //pen.setWidth(10);
    //paint->setPen(pen);
    //paint->drawPoints(points, px);
    //QTransform transform;
    //QTransform trans = transform.rotate(90);
    //QPixmap p1(pix->transformed(trans));

    QByteArray byteArray;
    QBuffer buffer(&byteArray);

    //QPixmap p2(p1.scaledToHeight(840));
    //p2.save(&buffer, "PNG");
    //p1.save(&buffer, "PNG");
    pix->save(&buffer, "PNG");

    return QString("<p><img  style=\"page-break-before:always\" src=\"data:image/png;base64,") + byteArray.toBase64() + "\"/></p>";
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


