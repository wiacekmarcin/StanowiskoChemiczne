#include "testdata.h"

#include <QPolygon>
#include <QPoint>
#include <QPixmap>
#include <QPainter>
#include <QByteArray>
#include <QBuffer>
#include <QDebug>
#include <QVector>

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
    result += QString("<div style=\"margin-top:200px\">%1 %2</div>").arg(getData(), getUczestnicy());
    result += getNazwaCieczy();
    result += "<div style=\"margin-top:100px\">";
    result += getTempKomoraStart();
    result += getWilgotnosc();
    result += "</div";

    for (int id = 0; id < d.proby.size(); ++id) {
        if (id) {
            result += QString("<p>#######################################################################</p>");
        }
        result += getProba(d.proby.at(id), d.iloscCieczy, id + 1);
    }
    result += getWarunkiPoUdanejProbie();
    analogIn idTab[8] = { a_vol1, a_vol2, a_o2, a_co2, a_8, a_cisn_komora, a_temp_komory, a_temp_parownik};
    for (short i = 0; i < 8; ++i) {
        visibleWykresType var =  visibleWykres[idTab[i]];
        if (!var.show)
            continue;
        result += getImageWykresPage(idTab[i], var.min, var.max, var.opis, var.opis2, var.jedn);
    }
    result += getImages();
    result += getComment();
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

QString TestData::getTemat() const
{
    return QString("<div style=\"margin-top:150px\"><table><tr><td><p style=\"font-size:18px;\">") +
           QString("<b>%1</b></p><p style=\" font-size:18px;\">%2</p></td><td>%3</td></tr></table></div>").arg(temat, tematValue, getLogo());
}

QString TestData::getLogo() const
{
    QPixmap p("LogoSGSP.png");
    QByteArray byteArray;
    QBuffer buffer(&byteArray);
    p.save(&buffer, "PNG");

    return QString("<img src=\"data:image/png;base64,") + byteArray.toBase64() + "\"/></p>";
}

QString TestData::getUczestnicy() const
{
    QString result = "";
    result += QString("<p style=\"font-size:14px;\">%1<ol>").arg(TestData::skladzespolu);

    foreach(auto u , d.uczestnicy) {
        result += QString("<li>%1</li>").arg(u);
    }

    result += "</ol></p>";
    return result;
}

QString TestData::getNazwaCieczy() const
{
    return QString("<div style=\"margin-top:150px;\"><p style=\"font-size:14px;\">%1 %2</p></div>").arg(TestData::nazwacieczy, d.nazwaCieczy);
}

QString TestData::getTempKomoraStart() const
{
    return QString("<p style=\"font-size:14px;\">%1 %2 &#x2103;</p>").arg(QString(TestData::tempKomoraStartTest), QString::number(d.tempKomoraPoczatek, 'f', 1));
}

QString TestData::getWilgotnosc() const
{
    return QString("<p style=\"page-break-after:always;font-size:14px;\">%1 %2 %RH</p>").arg(QString(TestData::wilgotnosc), QString::number(d.wilgotnosc, 'f', 1));
}

QString TestData::getProba(const ProbaType &proba, const QList<float> & dozowanie, unsigned int nrProba) const
{
    qDebug() << proba.cisnKomoryDozowanie << " " << proba.cisnKomoryZaplon << " " << proba.powtarzaneDozowanie << " "
             << proba.powtarzanyZaplon << " " << proba.success << " " << proba.co2 << " " << proba.cz8 << " "
             << proba.o2 << " " << proba.tempKomoryDozowanie << " " << proba.tempKomoryZaplon << " "
             << proba.tempParownikaDozowanie << " " << proba.voc1 << " " << proba.voc2 << " " << proba.zrodloZaplonu;
    QString ret("<div>");
    ret += QString("<h2 style=\"font-size:16px\">%1 %2</h2>").arg(QString(TestData::proba), QString::number(nrProba));
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
    return QString("<p style=\"font-size:12px\">%1 %2</p>").arg(QString(TestData::probaWynik),
                                       (wynik ? QString(TestData::probaPozytywna) : QString(TestData::probaNegatywna)));
}

QString TestData::getZrodloZaplonu(const QString &zaplon) const
{
    return QString("<p style=\"font-size:12px\">%1 %2</p>").arg(QString(TestData::zrodloZaplonu), zaplon);
}

QString TestData::getIloscCieczy(const QList<float> &dozowanie, unsigned int nrProba) const
{
    if (nrProba == 1 || dozowanie.size() == 1) {
        return QString("<p style=\"font-size:12px\">%1 %2 ml</p>").arg(QString(TestData::iloscCieczy), QString::number(dozowanie.at(0),'f', 1));
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
        return QString("<p style=\"font-size:12px\">%1 %2 ml</p>").arg(QString(TestData::iloscCieczy), QString::number(dozowanie.at(0),'f', 1));
    }
    return QString("<p style=\"font-size:12px\">%1 %2 ml</p>").arg(QString(TestData::iloscCieczy), sumaStr);
}

QString TestData::getWarunkiPoczatkowe(const ProbaType &proba) const
{
    QString ret = QString("<h3 style=\"font-size:16px\">%1</h3>").arg(QString(TestData::warunkiPoczatkowe));
    ret += QString("<p style=\"font-size:12px\">%1 %2 &#x2103;").arg(QString(TestData::tempParownika), QString::number(proba.tempParownikaDozowanie, 'f', 1));
    ret += QString("<p style=\"font-size:12px\">%1 %2 &#x2103;").arg(QString(TestData::tempKomory), QString::number(proba.tempKomoryDozowanie, 'f', 1));
    ret += QString("<p style=\"font-size:12px\">%1 %2 mBar").arg(QString(TestData::cisnKomory), QString::number(10*proba.cisnKomoryDozowanie, 'f', 1));
    return ret;
}

QString TestData::getWarunkiPrzedZaplonem(const ProbaType &proba) const
{
    QString ret = QString("<h3 style=\"font-size:16px\">%1</h3>").arg(QString(TestData::warunkiPrzedZaplonem));
    ret += QString("<p style=\"font-size:12px\">%1 %2 &#x2103;").arg(QString(TestData::tempKomory), QString::number(proba.tempKomoryZaplon, 'f', 1));
    ret += QString("<p style=\"font-size:12px\">%1 %2 mBar").arg(QString(TestData::cisnKomory), QString::number(10*proba.cisnKomoryZaplon, 'f', 1));
    ret += QString("<p style=\"font-size:12px\">%1 %2 %").arg(QString(TestData::koncetracjaPar), QString::number(proba.koncentracjaPar, 'f', 1));
    if (proba.zlaKoncetracja) {
        ret += QString("<pstyle=\"font-size:12px\"><u>%1</u></p>").arg(TestData::zlaKoncetracjaPar);
    }
    return ret;
}

QString TestData::getOdczytyStezen(const ProbaType &proba) const
{
    QString ret = QString("<pstyle=\"font-size:12px\">%1").arg(TestData::odczytyStezen);
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
    QString ret = QString("<h3 style=\"font-size:14px\">%1</h3>").arg(QString(TestData::warunkiPoUdanejProba));
    ret += QString("<p style=\"font-size:12px\">%1 %2 &#x2103;").arg(QString(TestData::tempKomory), QString::number(d.tempKomoraKoniec, 'f', 1));
    ret += QString("<p style=\"font-size:12px\">%1 %2 mBar").arg(QString(TestData::cisnKomory), QString::number(10*d.cisnienieKoniec, 'f', 1));
    ret += QString("<p style=\"font-size:12px\">%1").arg(TestData::odczytyStezen);
    ret += QString("<ul>");
    ret += QString("<li>VOC1 = %1 %</li>").arg(QString::number(d.voc1, 'f', 1));
    ret += QString("<li>VOC2 = %1 %</li>").arg(QString::number(d.voc2, 'f', 1));
    ret += QString("<li>O2 = %1 %</li>").arg(QString::number(d.o2, 'f', 1));
    ret += QString("<li>CO2 = %1 %</li>").arg(QString::number(d.co2, 'f', 1));
    ret += QString("<li>Czujnik 8 = %1 %</li>").arg(QString::number(d.cz8, 'f', 1));
    return ret + QString("</ul></p>");
}

QString TestData::getImageWykresPage(analogIn id, float min, float max, const QString &title,
                                     const QString &subtite, const QString &jedn) const
{
    QString ret = QString("<div style=\"page-break-before:always\" ><p><b>%1</b></p><p>%2</p><p style=\"text-align:center\">").arg(title, subtite);
    ret += getImageWykres(id, min, max, jedn);
    ret += "</p></div>";
    return ret;

}


QString TestData::getImageWykres(analogIn id, float min, float max, const QString &jedn) const
{
    /*
    QVector<float> allvalue;

    QVector<float> singleVals;

    for (int i = 0 ; i < 500; i++) {
        allvalue << (20 + (i/15)*0.3);
    }

    float prevvalue = allvalue.last();
    for (int i = 0; i < 50; i++) {
        allvalue << prevvalue;
    }

    for (int i = 0 ; i < 400; i++) {
        allvalue << (prevvalue - (i/15)*0.3);
    }
    float prevvalue2 = allvalue.last();

    for (int i = 0 ; i < 150; i++) {
        allvalue << prevvalue2;
    }

    prevvalue = allvalue.last();
    for (int i = 0 ; i < 800; i++) {
        allvalue << (prevvalue + (i/5)*0.3);
    }
    prevvalue2 = allvalue.last();
    for (int i = 0 ; i < 50; i++) {
        allvalue << prevvalue2;
    }

    for (int i = 0 ; i < 500; i++) {
        allvalue << prevvalue2 - (i/10)*0.2;
    }

    //allvalue[id] = singleVals;

    short decVal = 0;
    float sumaDec = 0.0;
    float value;
    //foreach(auto allvalue, values) {
    //    sumaDec += allvalue[(short)id];
    foreach (auto val, allvalue) {
        sumaDec += val;
        ++decVal;
        if (decVal == 10) {
            decVal = 0;
            value = sumaDec / 10;
            sumaDec = 0.0;
            if (value < min)
                value = min;
            if (value > max)
                value = max;
            singleVals.append(value-min);
        }
    }
    */
    QVector<float> singleVals;
    short decVal = 0;
    float sumaDec = 0.0;
    float value;
    foreach (auto val8, values) {
        float val = val8[id];
        sumaDec += val;
        ++decVal;
        if (decVal == 10) {
            decVal = 0;
            value = sumaDec / 10.0;
            sumaDec = 0.0;
            if (value < min)
                value = min;
            if (value > max)
                value = max;
            singleVals.append(value-min);
        }
    }

    const int width = 1250;
    const int height = 950;

    float valperYpx = (height - 50) / (max - min);
    QPixmap *pix = new QPixmap(width, height);
    QPainter *paint = new QPainter(pix);

    paint->setPen(Qt::black);
    paint->setBrush(Qt::white);
    QFont f = paint->font();
    f.setPixelSize(20);
    f.setBold(true);
    paint->setFont(f);
    QRect rect = QRect(0, 0, width - 1, height - 1);
    paint->fillRect(rect, QBrush(Qt::white));
    paint->drawRect(rect);//5 radius apiece

    paint->drawText(QPoint(0.45*width, height-5), "Czas, s");

    short pixelsper1s = 5;
    int beforeOSY = 50;

    int nrSec = 0;
    int actWidth = beforeOSY;
    QPen darkgrayPen = QPen(Qt::darkGray);
    darkgrayPen.setWidth(3);
    QPen grayPen = QPen(Qt::gray);
    grayPen.setWidth(1);
    QPen blackPen = QPen(Qt::black);
    while (actWidth < width) {

        if (nrSec % 10 == 0) {
            paint->setPen(darkgrayPen);
            paint->drawLine(actWidth, 0, actWidth, height-50);
            paint->setPen(blackPen);
            if (nrSec != 240)
                paint->drawText(actWidth-15, height-25, QString::number(nrSec));
        } else {
            paint->setPen(grayPen);
            paint->drawLine(actWidth, 0, actWidth, height-50);
        }
        nrSec += 2;
        actWidth += 2*pixelsper1s;
    }

    int maxHeight = height - 50;
    for (short id = 0; id < 100; id+=2) {
        int y = maxHeight - (maxHeight*id/100.0);
        if (id % 10 == 0) {
            paint->setPen(darkgrayPen);
            paint->drawLine(beforeOSY, y, width, y);
            paint->setPen(blackPen);
            paint->drawText(25, y, QString::number(id));
        } else {
            paint->setPen(grayPen);
            paint->drawLine(beforeOSY, y, width, y);
        }
    }
    paint->setPen(darkgrayPen);
    paint->drawLine(beforeOSY, 2, width, 2);
    paint->drawLine(width - 1, 0, width - 1, height-50);
    paint->setPen(blackPen);
    paint->drawText(10, 20, "100");


    //paint->setPen(darkgrayPen);
    //paint->drawLine(beforeOSY, height-50, actWidth, height-50);
    //paint->setPen(blackPen);
    //paint->drawText(actWidth-15, height-25, QString::number(nrSec));

    QPoint * points = new QPoint[singleVals.size()];
    nrSec = 0;
    actWidth = beforeOSY;
    qDebug() << "Tworzenie punktow " << singleVals.size();
    while (actWidth < width && nrSec < singleVals.size()) {
        qDebug() << "Punkt " << nrSec << " " << actWidth << ", " << (max - singleVals.at(nrSec))*valperYpx;
        points[nrSec] = QPoint(actWidth, (max - singleVals.at(nrSec))*valperYpx);
        actWidth += pixelsper1s;
        ++nrSec;
    }

    QPen pen = QPen(Qt::green);
    pen.setWidth(5);
    paint->setPen(pen);
    int ind = 1;
    while (ind < nrSec) {
        paint->drawLine(points[ind-1], points[ind]);
        ++ind;
    }
    //

    QTransform transform;
    QTransform trans = transform.rotate(270);
    QPixmap p1(pix->transformed(trans));

    QPainter *paint270 = new QPainter(&p1);
    paint270->setFont(f);
    paint270->drawText((height-20)/2, width-5, QString("%1").arg(jedn));

    QByteArray byteArray;
    QBuffer buffer(&byteArray);

    //QPixmap p2(p1.scaledToHeight(840));
    //p2.save(&buffer, "PNG");
    p1.save(&buffer, "PNG");
    //pix->save(&buffer, "PNG");

    return QString("<img  src=\"data:image/png;base64,") + byteArray.toBase64() + "\"/>";


#if 0
    const float ratioX = (height - 50)/(max-min);

    QPoint * points = new QPoint[singleVals.size()];
    unsigned int px = 0;
    for(px = 0; px < singleVals.size(); ++px) {
        points[px]= QPoint(px, (int)((max-singleVals.at(px))*ratioX));
    }
    QSize A4Size = QSize(width, height);
    //QPixmap *pix = new QPixmap(px, 1000);


    paint->setBrush(Qt::white);
    QRect rect = QRect(0, 0, A4Size.width() - 1, A4Size.height() - 1);
    paint->fillRect(rect, QBrush(Qt::white));
    paint->drawRect(rect);//5 radius apiece

    QPen grayPen = QPen(Qt::gray);
    grayPen.setWidth(1);
    paint->setPen(grayPen);
    for (int i = 0; i <= width; i+=20) {
        if (i % 100 == 0)
            continue;
        //paint->drawLine(0, i, 600, i);
        //if (i < 600)
        //    paint->drawLine(i, 0, i, 850);
    }


    QPen darkgrayPen = QPen(Qt::darkGray);
    darkgrayPen.setWidth(3);
    paint->setPen(darkgrayPen);
    for (int i = 0; i <= width; i+=100) {
        paint->drawLine(0, i, 600, i);
        //if (i <= 600)
        //    paint->drawLine(i, 0, i, 850);
    }



    paint->setBrush(Qt::black);
    QPen pen = QPen(Qt::black);
    pen.setWidth(10);
    paint->setPen(pen);
    paint->drawPoints(points, px);
    QTransform transform;
    QTransform trans = transform.rotate(90);
    QPixmap p1(pix->transformed(trans));

    QByteArray byteArray;
    QBuffer buffer(&byteArray);

    //QPixmap p2(p1.scaledToHeight(840));
    //p2.save(&buffer, "PNG");
    p1.save(&buffer, "PNG");
    //pix->save(&buffer, "PNG");

    return QString("<p><img src=\"data:image/png;base64,") + byteArray.toBase64() + "\"/></p>";
#endif
}

QString TestData::getPicture(int num, const QPair<QString,QString> &filename) const
{
    QPixmap p(filename.first);
    QByteArray byteArray;
    QBuffer buffer(&byteArray);
    p.save(&buffer, "PNG");
    QString ret = QString("<div style=\"\">");
    ret += QString("<p style=\"text-align: center;\"><img src=\"data:image/png;base64,") + byteArray.toBase64() + "\"/></p>";
    ret += QString("<p style=\"text-align: center; font-size: 14px; margin-bottom: 75px;\">Fot %1. %2</p>").arg(num).arg(filename.second);
    ret += QString("</div>");
    return ret;
}

QString TestData::getComment() const
{
    return QString("<p>")+comment+QString("</p>");
}

QString TestData::getImages() const
{
    QString ret;
    if (fileList.size() == 0)
        return "";

    for (int i =0; i < fileList.size(); i+= 2) {
        ret += "<div style=\"page-break-before:always\">";
        if (i == 0)
            ret += QString("<p style=\"text-align: center; font-size: 20px; margin-bottom: 25px;\"><b>%1</b></p>").arg(zdjeciaPrzeprBadania);
        ret += getPicture(i+1, fileList.at(i));
        if (i + 1 < fileList.size()) {
            ret += getPicture(i+2, fileList.at(i+1));
        }
        ret += "</div>";
    }
    return ret;
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

void TestData::setWykresVisible(analogIn wykresId, bool show, float minV, float maxV,
                                const QString & opis, const QString &opis2, const QString & unit)
{
    visibleWykresType value;
    value.show = show;
    value.min = minV;
    value.max = maxV;
    value.opis = opis;
    value.opis2 = opis2;
    value.jedn = unit;
    visibleWykres[wykresId] = value;
}

void TestData::addImage(const QString &file, const QString &descr)
{
    fileList << QPair<QString,QString>(file, descr);
}

void TestData::clearImage()
{
    fileList.clear();
}

void TestData::setComment(const QString & comm)
{
    comment = comm;
}


