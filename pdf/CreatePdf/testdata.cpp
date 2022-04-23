#include "testdata.h"

#include <QPoint>
#include <QPixmap>
#include <QByteArray>
#include <QBuffer>
#include <QPainter>

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
    result += QString("<p>") + getImageUrl() + QString("</p>");
    result += QString("<p>") + getImageUrl() + QString("</p>");
    result += QString("<p>") + getImageUrl() + QString("</p>");
    result += QString("</body></html>");
    return result;
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

QString TestData::getImageUrl() const
{
    QPoint * points = new QPoint[6000];
    unsigned int px = 0;
    for(px = 0; px < 6000; ++px) {
        points[px]= QPoint(px, 10* (px % 100));
    }
    QSize A4Size = QSize(600, 850);
    //QPixmap *pix = new QPixmap(px, 1000);
    QPixmap *pix = new QPixmap(A4Size.width(), A4Size.height()/2);
    QPainter *paint = new QPainter(pix);

    paint->setBrush(Qt::white);
    QRect rect = QRect(0, 0, A4Size.width() - 1, A4Size.height()/2 - 1);
    paint->fillRect(rect, QBrush(Qt::white));
    paint->drawRect(rect);//5 radius apiece

    QPen grayPen = QPen(Qt::gray);
    grayPen.setWidth(1);
    paint->setPen(grayPen);
    paint->drawLine(20, 0, 20, A4Size.height()/2);
    paint->drawLine(40, 0, 40, A4Size.height()/2);
    paint->drawLine(60, 0, 60, A4Size.height()/2);
    paint->drawLine(80, 0, 80, A4Size.height()/2);
    paint->drawLine(120, 0, 120, A4Size.height()/2);
    paint->drawLine(140, 0, 140, A4Size.height()/2);
    paint->drawLine(160, 0, 160, A4Size.height()/2);
    paint->drawLine(180, 0, 180, A4Size.height()/2);
    paint->drawLine(220, 0, 220, A4Size.height()/2);
    paint->drawLine(240, 0, 240, A4Size.height()/2);
    paint->drawLine(260, 0, 260, A4Size.height()/2);
    paint->drawLine(280, 0, 280, A4Size.height()/2);
    paint->drawLine(320, 0, 320, A4Size.height()/2);
    paint->drawLine(340, 0, 340, A4Size.height()/2);
    paint->drawLine(360, 0, 360, A4Size.height()/2);
    paint->drawLine(380, 0, 380, A4Size.height()/2);
    paint->drawLine(420, 0, 420, A4Size.height()/2);
    paint->drawLine(440, 0, 440, A4Size.height()/2);
    paint->drawLine(460, 0, 460, A4Size.height()/2);
    paint->drawLine(480, 0, 480, A4Size.height()/2);
    paint->drawLine(520, 0, 520, A4Size.height()/2);
    paint->drawLine(540, 0, 540, A4Size.height()/2);
    paint->drawLine(560, 0, 560, A4Size.height()/2);
    paint->drawLine(580, 0, 580, A4Size.height()/2);

    QPen darkgrayPen = QPen(Qt::darkGray);
    darkgrayPen.setWidth(3);
    paint->setPen(darkgrayPen);
    paint->drawLine(0, 0, 0, A4Size.height()/2);
    paint->drawLine(100, 0, 100, A4Size.height()/2);
    paint->drawLine(200, 0, 200, A4Size.height()/2);
    paint->drawLine(300, 0, 300, A4Size.height()/2);
    paint->drawLine(400, 0, 400, A4Size.height()/2);
    paint->drawLine(500, 0, 500, A4Size.height()/2);
    paint->drawLine(600, 0, 600, A4Size.height()/2);


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

    return QString("<img src=\"data:image/png;base64,") + byteArray.toBase64() + "\"/>";
}
