#include "pdfcreator.h"

#include <QPolygon>
#include <QPoint>
#include <QPixmap>
#include <QPainter>
#include <QByteArray>
#include <QBuffer>
#include <QDebug>
#include <QVector>
#include <math.h>

#include "testdata.h"


constexpr char PdfCreator::title1[];
constexpr char PdfCreator::title2[];
constexpr char PdfCreator::temat[];
constexpr char PdfCreator::tematValue[];
constexpr char PdfCreator::dataWykonania[];
constexpr char PdfCreator::skladzespolu[];
constexpr char PdfCreator::nazwacieczy[];
constexpr char PdfCreator::tempKomoraStartTest[];
constexpr char PdfCreator::wilgotnosc[];
constexpr char PdfCreator::proba[];
constexpr char PdfCreator::probaWynik[];
constexpr char PdfCreator::probaPozytywna[];
constexpr char PdfCreator::probaNegatywna[];
constexpr char PdfCreator::zrodloZaplonu[];
constexpr char PdfCreator::iloscCieczy[];
constexpr char PdfCreator::warunkiPoczatkowe[];
constexpr char PdfCreator::warunkiPrzedZaplonem[];
constexpr char PdfCreator::warunkiPoUdanejProba[];
constexpr char PdfCreator::tempParownika[];
constexpr char PdfCreator::tempKomory[];
constexpr char PdfCreator::cisnKomory[];
constexpr char PdfCreator::cisnKomoryPoOdpCieczy[];
constexpr char PdfCreator::koncetracjaPar[];
constexpr char PdfCreator::zlaKoncetracjaPar[];
constexpr char PdfCreator::odczytyStezen[];
constexpr char PdfCreator::zdjeciaPrzeprBadania[];

PdfCreator::PdfCreator(const TestData &td_, const Ustawienia &ust_) :
    td(td_), ust(ust_)
{

}

QString PdfCreator::getData() const
{
    return QString("<p style=\"font-size:14px;\">%1%2</p>").arg(dataWykonania, td.getDataTestu().toString("dd.MM.yyyy hh:mm"));
}

QString PdfCreator::getBody() const
{
    QString result = QString("<html><body>") + getTitle();

    result += getTemat();
    result += QString("<div style=\"margin-top:200px\">%1 %2</div>").arg(getData(), getUczestnicy());
    result += getNazwaCieczy();
    result += "<div style=\"margin-top:100px\">";
    result += getTempKomoraStart();
    result += getWilgotnosc();
    result += "</div>";

    for (int id = 0; id < td.getProby().size(); ++id) {
        result += getProba(td.getProby().at(id), td.getIloscCieczy(), id + 1);
    }
    result += getWarunkiPoUdanejProbie();
    foreach (visibleWykresType var, visibleWykres) {
        result += getImageWykresPage(var);
    }
    result += getImages();
    result += getComment();
    result += QString("</body></html>");
    return result;
}

QString PdfCreator::getTitle() const
{
    return QString("<div align=\"center\">%1%2</div>").arg(getTitle1(), getTitle2());
}

QString PdfCreator::getTemat() const
{
    return QString("<div style=\"margin-top:150px\"><table><tr><td><p style=\"font-size:18px;\">") +
           QString("<b>%1</b></p><p style=\" font-size:18px;\">%2</p></td><td>%3</td></tr></table></div>").arg(temat, tematValue, getLogo());
}

QString PdfCreator::getLogo() const
{
    QPixmap p("LOGOSGSPKolor.png");
    QByteArray byteArray;
    QBuffer buffer(&byteArray);
    p.save(&buffer, "PNG");

    return QString("<img src=\"data:image/png;base64,") + byteArray.toBase64() + "\"/>";
}

QString PdfCreator::getUczestnicy() const
{
    QString result = "";
    result += QString("<p style=\"font-size:14px;\">%1<ol>").arg(PdfCreator::skladzespolu);

    foreach(auto u , td.getUczestnicy()) {
        result += QString("<li>%1</li>").arg(u);
    }

    result += "</ol></p>";
    return result;
}

QString PdfCreator::getNazwaCieczy() const
{
    return QString("<div style=\"margin-top:150px;\"><p style=\"font-size:14px;\">%1 %2</p></div>").arg(PdfCreator::nazwacieczy,
                                                                                                        td.getNazwaCieczy());
}

QString PdfCreator::getTempKomoraStart() const
{
    return QString("<p style=\"font-size:14px;\">%1 %2 &#x2103;</p>").arg(QString(PdfCreator::tempKomoraStartTest),
                                                                          QString::number(td.getTempKomoraPoczatek(), 'f', 1));
}

QString PdfCreator::getWilgotnosc() const
{
    return QString("<p style=\"font-size:14px;\">%1 %2 %RH</p>").arg(QString(PdfCreator::wilgotnosc),
                                                                                             QString::number(td.getWilgotnosc(), 'f', 1));
}

QString PdfCreator::getProba(const ProbaType &proba, const QList<float> & dozowanie, unsigned int nrProba) const
{
    /*
    qDebug() << proba.cisnKomoryDozowanie << " " << proba.cisnKomoryZaplon << " " << proba.powtarzaneDozowanie << " "
             << proba.powtarzanyZaplon << " " << proba.success << " " << proba.co2 << " " << proba.cz8 << " "
             << proba.o2 << " " << proba.tempKomoryDozowanie << " " << proba.tempKomoryZaplon << " "
             << proba.tempParownikaDozowanie << " " << proba.voc1 << " " << proba.voc2 << " " << proba.zrodloZaplonu;
    */
    QString ret;
    if (nrProba % 2 == 1)
        ret += ("<div style=\"page-break-before:always\">");
    else
        ret += "<div>";
    ret += QString("<h2 style=\"font-size:16px\">%1 %2</h2>").arg(QString(PdfCreator::proba), QString::number(nrProba));
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

QString PdfCreator::getProbaWynik(bool wynik) const
{
    return QString("<p style=\"font-size:12px\">%1 %2</p>").arg(QString(PdfCreator::probaWynik),
                                       (wynik ? QString(PdfCreator::probaPozytywna) : QString(PdfCreator::probaNegatywna)));
}

QString PdfCreator::getZrodloZaplonu(const QString &zaplon) const
{
    return QString("<p style=\"font-size:12px\">%1 %2</p>").arg(QString(PdfCreator::zrodloZaplonu), zaplon);
}

QString PdfCreator::getIloscCieczy(const QList<float> &dozowanie, unsigned int nrProba) const
{
    if (nrProba == 1 || dozowanie.size() == 1) {
        return QString("<p style=\"font-size:12px\">%1 %2 ml</p>").arg(QString(PdfCreator::iloscCieczy), QString::number(dozowanie.at(0),'f', 1));
    }

    float suma = 0.0;
    QString sumaStr;
    unsigned int sumCnt = 0;
    float prev = 0.0;
    for (unsigned int i = 0; i < dozowanie.size() && i < nrProba; ++i) {
        float d = dozowanie.at(i) - prev;
        if (d == 0.0)
            continue;
        suma += d;
        sumaStr += QString("+ %1 ").arg(QString::number(d, 'f', 1));
        ++sumCnt;
    }
    sumaStr.remove(0,1);
    sumaStr += QString("= %1").arg(QString::number(suma, 'f', 1));
    if (sumCnt == 1) {
        return QString("<p style=\"font-size:12px\">%1 %2 ml</p>").arg(QString(PdfCreator::iloscCieczy), QString::number(dozowanie.at(0),'f', 1));
    }
    return QString("<p style=\"font-size:12px\">%1 %2 ml</p>").arg(QString(PdfCreator::iloscCieczy), sumaStr);
}

QString PdfCreator::getWarunkiPoczatkowe(const ProbaType &proba) const
{
    Ustawienia::CzujnikAnalogowy an = ust.getCzujnikAnalogowyUstawienia(a_cisn_komora);
    QString unit = an.unit;
    float conv = an.convert;

    QString ret = QString("<h3 style=\"font-size:16px\">%1</h3>").arg(QString(PdfCreator::warunkiPoczatkowe));
    ret += QString("<p style=\"font-size:12px\">%1 %2 &#x2103;</p>").arg(QString(PdfCreator::tempParownika), QString::number(
                                                        proba.tempParownikaDozowanie, 'f', 1));

    ret += QString("<p style=\"font-size:12px\">%1 %2 &#x2103;</p>").arg(QString(PdfCreator::tempKomory), QString::number(
                                                        proba.tempKomoryDozowanie, 'f', 1));

    ret += QString("<p style=\"font-size:12px\">%1 %2 %3</p>").arg(QString(PdfCreator::cisnKomory), QString::number(
                                                        conv*proba.P1, 'f', 1),
                                                        unit);
    ret += QString("<p style=\"font-size:12px\">%1 %2 %3</p>").arg(QString(PdfCreator::cisnKomoryPoOdpCieczy), QString::number(
                                                        conv*proba.P2, 'f', 1),
                                                        unit);
    return ret;
}

QString PdfCreator::getWarunkiPrzedZaplonem(const ProbaType &proba) const
{
    Ustawienia::CzujnikAnalogowy an = ust.getCzujnikAnalogowyUstawienia(a_cisn_komora);
    QString unit = an.unit;
    float conv = an.convert;
    float val1 = (proba.podcisnienie) ? proba.P3 : proba.P2;


    QString ret = QString("<h3 style=\"font-size:16px\">%1</h3>").arg(QString(PdfCreator::warunkiPrzedZaplonem));
    ret += QString("<p style=\"font-size:12px\">%1 %2 &#x2103;</p>").arg(QString(PdfCreator::tempKomory), QString::number(
                                                                             proba.tempKomoryPrzedZaplon, 'f', 1));

    ret += QString("<p style=\"font-size:12px\">%1 %2 %3</p>").arg(QString(PdfCreator::cisnKomory),
                                                                 QString::number(conv*val1, 'f', 1),
                                                                  unit);

    ret += QString("<p style=\"font-size:12px\">%1 %2 %</p>").arg(QString(PdfCreator::koncetracjaPar), QString::number(proba.calkowitaKoncetracjaPar, 'f', 1));
    if (proba.zlaKoncetracja) {
        ret += QString("<p style=\"font-size:12px\"><u>%1</u></p>").arg(PdfCreator::zlaKoncetracjaPar);
    }
    return ret;
}

QString PdfCreator::getOdczytyStezen(const ProbaType &proba) const
{
    QString ret = QString("<p style=\"font-size:12px\">%1").arg(PdfCreator::odczytyStezen);
    ret += QString("<ul>");
    ret += QString("<li>VOC1 = %1 %</li>").arg(QString::number(proba.voc1, 'f', 2));
    ret += QString("<li>VOC2 = %1 %</li>").arg(QString::number(proba.voc2, 'f', 2));
    ret += QString("<li>O2 = %1 %</li>").arg(QString::number(proba.o2, 'f', 2));
    ret += QString("<li>CO2 = %1 %</li>").arg(QString::number(proba.co2, 'f', 2));
    ret += QString("<li>Czujnik 8 = %1 %</li>").arg(QString::number(proba.cz8, 'f', 2));
    return ret + QString("</ul></p>");
}

QString PdfCreator::getWarunkiPoUdanejProbie() const
{
    Ustawienia::CzujnikAnalogowy an = ust.getCzujnikAnalogowyUstawienia(a_cisn_komora);
    QString unit = an.unit;
    float conv = an.convert;
    QString ret = QString("<h3 style=\"font-size:14px\">%1</h3>").arg(QString(PdfCreator::warunkiPoUdanejProba));
    ret += QString("<p style=\"font-size:12px\">%1 %2 &#x2103;</p>").arg(QString(PdfCreator::tempKomory),
                                                                     QString::number(td.getTempKomoraKoniec(), 'f', 1));
    ret += QString("<p style=\"font-size:12px\">%1 %2 %3</p>").arg(QString(PdfCreator::cisnKomory),
                                                                 QString::number(conv*td.getCisnienieKoniec(), 'f', 1),
                                                                   unit);
    ret += QString("<p style=\"font-size:12px\">%1").arg(PdfCreator::odczytyStezen);
    ret += QString("<ul>");
    ret += QString("<li>VOC1 = %1 %</li>").arg(QString::number(td.getVoc1(), 'f', 1));
    ret += QString("<li>VOC2 = %1 %</li>").arg(QString::number(td.getVoc2(), 'f', 1));
    ret += QString("<li>O2 = %1 %</li>").arg(QString::number(td.getO2(), 'f', 1));
    ret += QString("<li>CO2 = %1 %</li>").arg(QString::number(td.getCo2(), 'f', 1));
    ret += QString("<li>Czujnik 8 = %1 %</li>").arg(QString::number(td.getCz8(), 'f', 1));
    return ret + QString("</ul></p>");
}

QString PdfCreator::getImageWykresPage(const visibleWykresType & var) const
{
    QString ret = QString("<div style=\"page-break-before:always\" ><p><b>%1</b></p><p>%2</p><p style=\"text-align:center\">")
            .arg(var.opis, var.opis2);
    ret += getImageWykres(var);
    ret += "</div>";
    return ret;

}

QString PdfCreator::getTitle1() const
{
    return QString("<h1>%1</h1>").arg(title1);
}

QString PdfCreator::getTitle2() const
{
    return QString("<h1>%1</h1>").arg(title2);
}

QString PdfCreator::getPicture(int num, const QPair<QString,QString> &filename) const
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

QString PdfCreator::getComment() const
{
    return QString("<h2 style=\"font-size: 14px;\">Komentarz</h2><p>")+comment+QString("</p>");
}

QString PdfCreator::getImages() const
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

void PdfCreator::addImage(const QString &file, const QString &descr)
{
    fileList << QPair<QString,QString>(file, descr);
}

void PdfCreator::clearImage()
{
    fileList.clear();
}

void PdfCreator::setComment(const QString & comm)
{
    comment = comm;
}

QString PdfCreator::getOsX(unsigned int i, unsigned int max, const float & minx, const float & maxx) const
{
    //qInfo() << "getOsX i=" << i << " ratio=" << max << " minx=" << minx << " maxx=" << maxx;
    float valx = maxx - minx;
    float realVal = (minx + 1.0 * i * valx / max);

    valx /= 10;
    realVal /= 10;

    if (realVal < 24)
        return QString::number(realVal, 'f', 1)+QString("\"");

    if (realVal < 250)
        return QString::number(realVal, 'f', 0)+QString("\"");

    realVal /= 60.0;
    valx /= 60.0;

    if (realVal < 250)
        return QString::number(realVal, 'f', 1)+QString("\'");

    if (realVal < 250)
        return QString::number(realVal, 'f', 0)+QString("\'");

    realVal /= 60.0;
    valx /= 60.0;

    if (realVal < 24)
        return QString::number(realVal, 'f', 2)+QString("h");

    if (realVal < 250)
        return QString::number(realVal, 'f', 1)+QString("h");

    return QString::number(realVal, 'f', 0)+QString("h");
}

QString PdfCreator::getOsY(unsigned int i, const float & imax, const float & minv, const float & maxv) const
{
    //qInfo() << "getOsY i=" << i << " imax=" << imax << " minv=" << minv << " maxv=" << maxv;
    float valy = maxv - minv;
    float realVal = minv + (1.0 * i / imax)* valy;
    qInfo() << "realVal=" << realVal;
    if (valy < 1)
        return QString::number(realVal, 'f', 3);
    if (valy < 10)
        return QString::number(realVal, 'f', 2);
    if (valy < 100)
        return QString::number(realVal, 'f', 1);
    return QString::number(realVal, 'f', 0);
}

void PdfCreator::setWykresVisible(analogIn wykresId, bool show, float minV, float maxV, double ratio, unsigned int pages,
                                  const QString &opis, const QString &opis2, const QString &unit)
{
    if (!show || pages == 0)
        return;
    visibleWykresType v;
    v.id = wykresId;
    v.minVal = minV;
    v.maxVal = maxV;
    v.ratio = ratio;
    v.opis = opis;
    v.opis2 = opis2;
    v.jedn = unit;
    //unsigned long maxTimeSec = td.getValues().size()/10;
    unsigned long maxTimeSec = td.getValues().size();
    if (pages == 1) {
        v.minTime = 0;
        v.maxTime = maxTimeSec;
        visibleWykres.append(v);
        qInfo() << v.id << " min=" << v.minVal << " max=" << v.maxVal << " ratio="
                << v.ratio << " opis=" << v.opis << " opis2=" << v.opis2 << " unit=" << v.jedn
                << " minT=" << v.minTime << " maxT=" << v.maxTime;
        qInfo() << "values=" << td.getValues().size();
        return;
    }

    for (uint ip = 0; ip < pages; ip++) {
        visibleWykresType t = v;
        t.minTime = maxTimeSec*ip/pages;
        t.maxTime = maxTimeSec*(ip+1)/pages;
        visibleWykres.append(t);
    }
}

QString PdfCreator::getImageWykres(const visibleWykresType & var) const
{
    qInfo() << var.id << " min=" << var.minVal << " max=" << var.maxVal << " ratio=" << var.ratio
            << " opis=" << var.opis << " opis2=" << var.opis2 << " unit=" << var.jedn
               << " minT=" << var.minTime << " maxT=" << var.maxTime;
    analogIn id = var.id;
    float minV = var.minVal;
    float maxV = var.maxVal;
    float minT = var.minTime;
    float maxT = var.maxTime;
    double ratio = var.ratio;
    qInfo() << "minV" << minV << "maxV" << maxV << "minT" << minT << "maxT" << maxT;

    const QString & title = var.opis;
    const QString & subtitle = var.opis2;
    const QString & jedn = var.jedn;

    constexpr unsigned int width = 1375;
    constexpr unsigned int height = 1000;
    constexpr unsigned int marginleft = 75;
    constexpr unsigned int marginright = 50;
    constexpr unsigned int margintop = 50;
    constexpr unsigned int marginbottom = 50;

    constexpr unsigned int maxHeight = height - margintop - marginbottom;
    constexpr unsigned int maxWidth = width - marginleft - marginright;

    constexpr unsigned int pixelxperGray = 5;

    QVector<float> points;
    for (unsigned int pos = 0; pos < (unsigned int)td.getValues().size(); ++pos) {
        if (pos < minT)
            continue;
        if (pos > maxT)
            break;

        AnalValType a = td.getValues().at(pos);
        /*
        a_voc1              = 0,
        a_voc2              = 1,
        a_o2                = 2,
        a_co2               = 3,
        a_cisn_komora       = 4,
        a_temp_komory       = 5,
        a_temp_parownik     = 6,
        a_8                 = 7
        */
        float vals[8] = { a.voc1 , a.voc2, a.o2, a.co2, a.cisnKom, a.tempKom, a.tempPar, a.a8 };
        points.push_back(ratio * vals[(short)id]);
        //qInfo() << vals[(short)id] << " * " << ratio << " = " << (vals[(short)id] * ratio);
    }
    qInfo() << "Punktow do rysowania" << points.size();

    float valperYpx = (height - margintop - marginbottom) / (maxV - minV);
    float valperXpx = (width - marginleft - marginright) / (maxT - minT);
    qInfo() << valperXpx;

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

    QFontMetrics fm(paint->font());
    QString czasX("Czas, [ s \" / min ']");
    QRect rczas = fm.boundingRect(czasX);
    paint->drawText(QPoint((width+rczas.width())/2-rczas.width(), height-5), czasX);

    QRect rtitle = fm.boundingRect(title);
    paint->drawText(QPoint((width+rtitle.width())/2-rtitle.width(), 20), title);

    QRect rbtitle = fm.boundingRect(subtitle);
    paint->drawText(QPoint((width+rbtitle.width())/2-rbtitle.width(), 40), subtitle);

    float actWidth = 0;
    unsigned int nrSec = 0;
    QPen darkgrayPen = QPen(Qt::darkGray);
    darkgrayPen.setWidth(3);
    QPen grayPen = QPen(Qt::gray);
    grayPen.setWidth(1);
    QPen blackPen = QPen(Qt::black);
    QString sizeText;
    while (actWidth <= maxWidth)
    {
        if (nrSec % 10 == 0) {
            paint->setPen(darkgrayPen);
            paint->drawLine(marginleft + actWidth, margintop + 0, marginleft + actWidth, height - marginbottom);
            paint->setPen(blackPen);
            sizeText = getOsX(actWidth, maxWidth, minT, maxT);
            qInfo () << sizeText;
            if ((sizeText.size() > 4 && nrSec % 20 == 0) || sizeText.size() <= 4)
                paint->drawText(marginleft + actWidth - 15, height - 25, sizeText);
        } else {
            paint->setPen(grayPen);
            paint->drawLine(marginleft + actWidth, margintop + 0, marginleft + actWidth, height - marginbottom);
        }
        nrSec += 2;
        actWidth += 2*pixelxperGray;
    }

    unsigned int actProm = 0;

    while (actProm <= 100)
    {
        unsigned int y = maxHeight - int(maxHeight*actProm/100.0);
        if (actProm % 10 == 0) {
            paint->setPen(darkgrayPen);
            paint->drawLine(marginleft, margintop+y, width-marginright, margintop+y);
            paint->setPen(blackPen);
            paint->drawText(5, y+margintop, getOsY(actProm, 100, minV, maxV));
        } else {
            paint->setPen(grayPen);
            paint->drawLine(marginleft, margintop+y, width-marginright, margintop+y);
        }
        actProm += 2;
    }

    unsigned int valXP = 0;
    unsigned int valX = 0;
    unsigned int nsum = 0;
    float fsum = 0;
    unsigned int idp = 0;
    float realValx = 0;
    float valY = 0;
    float valYP = points[0];
    QPen pen = QPen(Qt::green);
    pen.setWidth(5);
    paint->setPen(pen);
    while (valX <= maxWidth && idp < (unsigned int)points.size())
    {
        fsum += points[idp];
        idp += 1;
        nsum += 1;
        realValx += valperXpx;
        valX = round(realValx);
        if (valX != valXP) {
            valY = 1.0* fsum / nsum;
            fsum = 0.0;
            nsum = 0;
            if (valYP < maxV && valY > maxV)
                valY = maxV;

            if (valYP > minV && valY < minV)
                valY = minV;

            if (valYP > maxV && valY < maxV)
                valYP = maxV;

            if (valYP < minV && valY > minV)
                valYP = minV;

            if (valYP <= maxV && valYP >= minV && valY <= maxV && valY >= minV) {
                unsigned int v1 = maxHeight - int(1.0 * (valYP - minV)*valperYpx);
                unsigned int v2 = maxHeight - int(1.0 * (valY - minV)*valperYpx);
                paint->drawLine(marginleft + valXP, margintop + v1, marginleft + valX, margintop + v2);
            }
            valXP = valX;
            valYP = valY;
        }
    }

    QTransform transform;
    QTransform trans = transform.rotate(270);
    QPixmap p1(pix->transformed(trans));

    QPainter *paint270 = new QPainter(&p1);
    paint270->setFont(f);
    paint270->drawText((height-20)/2, 35, QString("%1").arg(jedn));

    QByteArray byteArray;
    QBuffer buffer(&byteArray);

    p1.save(&buffer, "PNG");
    //pix->save(&buffer, "PNG");

    return QString("<img src=\"data:image/png;base64,") + byteArray.toBase64() + "\"/>";
}




