#include "pdfcreator.h"

#include <QPolygon>
#include <QPoint>
#include <QPixmap>
#include <QPainter>
#include <QByteArray>
#include <QBuffer>
#include <QDebug>
#include <QVector>

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
constexpr char PdfCreator::koncetracjaPar[];
constexpr char PdfCreator::zlaKoncetracjaPar[];
constexpr char PdfCreator::odczytyStezen[];
constexpr char PdfCreator::zdjeciaPrzeprBadania[];

PdfCreator::PdfCreator(const TestData &td_) :
    td(td_)
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
    result += "</div";

    for (int id = 0; id < td.getProby().size(); ++id) {
        if (id) {
            result += QString("<p>#######################################################################</p>");
        }
        result += getProba(td.getProby().at(id), td.getIloscCieczy(), id + 1);
    }
    result += getWarunkiPoUdanejProbie();
    analogIn idTab[8] = { a_voc1, a_voc2, a_o2, a_co2, a_8, a_cisn_komora, a_temp_komory, a_temp_parownik};
    for (short i = 0; i < 8; ++i) {
        visibleWykresType var =  visibleWykres[idTab[i]];
        if (!var.show)
            continue;
        result += getImageWykresPage(idTab[i], var.minVal, var.maxVal, var.opis, var.opis2, var.jedn);
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
    QPixmap p("LogoSGSP.png");
    QByteArray byteArray;
    QBuffer buffer(&byteArray);
    p.save(&buffer, "PNG");

    return QString("<img src=\"data:image/png;base64,") + byteArray.toBase64() + "\"/></p>";
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
    return QString("<p style=\"page-break-after:always;font-size:14px;\">%1 %2 %RH</p>").arg(QString(PdfCreator::wilgotnosc),
                                                                                             QString::number(td.getWilgotnosc(), 'f', 1));
}

QString PdfCreator::getProba(const ProbaType &proba, const QList<float> & dozowanie, unsigned int nrProba) const
{
    qDebug() << proba.cisnKomoryDozowanie << " " << proba.cisnKomoryZaplon << " " << proba.powtarzaneDozowanie << " "
             << proba.powtarzanyZaplon << " " << proba.success << " " << proba.co2 << " " << proba.cz8 << " "
             << proba.o2 << " " << proba.tempKomoryDozowanie << " " << proba.tempKomoryZaplon << " "
             << proba.tempParownikaDozowanie << " " << proba.voc1 << " " << proba.voc2 << " " << proba.zrodloZaplonu;
    QString ret("<div>");
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
    for (int i = 0; i < dozowanie.size(); ++i) {
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
        return QString("<p style=\"font-size:12px\">%1 %2 ml</p>").arg(QString(PdfCreator::iloscCieczy), QString::number(dozowanie.at(0),'f', 1));
    }
    return QString("<p style=\"font-size:12px\">%1 %2 ml</p>").arg(QString(PdfCreator::iloscCieczy), sumaStr);
}

QString PdfCreator::getWarunkiPoczatkowe(const ProbaType &proba) const
{
    QString ret = QString("<h3 style=\"font-size:16px\">%1</h3>").arg(QString(PdfCreator::warunkiPoczatkowe));
    ret += QString("<p style=\"font-size:12px\">%1 %2 &#x2103;").arg(QString(PdfCreator::tempParownika), QString::number(proba.tempParownikaDozowanie, 'f', 1));
    ret += QString("<p style=\"font-size:12px\">%1 %2 &#x2103;").arg(QString(PdfCreator::tempKomory), QString::number(proba.tempKomoryDozowanie, 'f', 1));
    ret += QString("<p style=\"font-size:12px\">%1 %2 kPa").arg(QString(PdfCreator::cisnKomory), QString::number(proba.cisnKomoryDozowanie, 'f', 1));
    return ret;
}

QString PdfCreator::getWarunkiPrzedZaplonem(const ProbaType &proba) const
{
    QString ret = QString("<h3 style=\"font-size:16px\">%1</h3>").arg(QString(PdfCreator::warunkiPrzedZaplonem));
    ret += QString("<p style=\"font-size:12px\">%1 %2 &#x2103;").arg(QString(PdfCreator::tempKomory), QString::number(proba.tempKomoryZaplon, 'f', 1));
    ret += QString("<p style=\"font-size:12px\">%1 %2 kPa").arg(QString(PdfCreator::cisnKomory),
                                                                 QString::number(proba.cisnKomoryZaplon, 'f', 1));
    ret += QString("<p style=\"font-size:12px\">%1 %2 %").arg(QString(PdfCreator::koncetracjaPar), QString::number(proba.koncentracjaPar, 'f', 1));
    if (proba.zlaKoncetracja) {
        ret += QString("<pstyle=\"font-size:12px\"><u>%1</u></p>").arg(PdfCreator::zlaKoncetracjaPar);
    }
    return ret;
}

QString PdfCreator::getOdczytyStezen(const ProbaType &proba) const
{
    QString ret = QString("<pstyle=\"font-size:12px\">%1").arg(PdfCreator::odczytyStezen);
    ret += QString("<ul>");
    ret += QString("<li>VOC1 = %1 %</li>").arg(QString::number(proba.voc1, 'f', 1));
    ret += QString("<li>VOC2 = %1 %</li>").arg(QString::number(proba.voc2, 'f', 1));
    ret += QString("<li>O2 = %1 %</li>").arg(QString::number(proba.o2, 'f', 1));
    ret += QString("<li>CO2 = %1 %</li>").arg(QString::number(proba.co2, 'f', 1));
    ret += QString("<li>Czujnik 8 = %1 %</li>").arg(QString::number(proba.cz8, 'f', 1));
    return ret + QString("</ul></p>");
}

QString PdfCreator::getWarunkiPoUdanejProbie() const
{
    QString ret = QString("<h3 style=\"font-size:14px\">%1</h3>").arg(QString(PdfCreator::warunkiPoUdanejProba));
    ret += QString("<p style=\"font-size:12px\">%1 %2 &#x2103;").arg(QString(PdfCreator::tempKomory),
                                                                     QString::number(td.getTempKomoraKoniec(), 'f', 1));
    ret += QString("<p style=\"font-size:12px\">%1 %2 kPa").arg(QString(PdfCreator::cisnKomory),
                                                                 QString::number(td.getCisnienieKoniec(), 'f', 1));
    ret += QString("<p style=\"font-size:12px\">%1").arg(PdfCreator::odczytyStezen);
    ret += QString("<ul>");
    ret += QString("<li>VOC1 = %1 %</li>").arg(QString::number(td.getVoc1(), 'f', 1));
    ret += QString("<li>VOC2 = %1 %</li>").arg(QString::number(td.getVoc2(), 'f', 1));
    ret += QString("<li>O2 = %1 %</li>").arg(QString::number(td.getO2(), 'f', 1));
    ret += QString("<li>CO2 = %1 %</li>").arg(QString::number(td.getCo2(), 'f', 1));
    ret += QString("<li>Czujnik 8 = %1 %</li>").arg(QString::number(td.getCz8(), 'f', 1));
    return ret + QString("</ul></p>");
}

QString PdfCreator::getImageWykresPage(analogIn id, float min, float max, const QString &title,
                                     const QString &subtite, const QString &jedn) const
{
    QString ret = QString("<div style=\"page-break-before:always\" ><p><b>%1</b></p><p>%2</p><p style=\"text-align:center\">").arg(title, subtite);
    ret += getImageWykres(id, min, max, jedn);
    ret += "</p></div>";
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


QString PdfCreator::getImageWykres(analogIn id, float min, float max, const QString &jedn) const
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
        float vals[8] = {val8.voc1, val8.voc2, val8.o2, val8.co2, val8.cisnKom, val8.tempKom, val8.tempPar, val8.a8 };
        float val = vals[(short)id];
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
    return QString("<p>")+comment+QString("</p>");
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

void PdfCreator::setWykresVisible(analogIn wykresId, bool show, float minV, float maxV,
                                const QString & opis, const QString &opis2, const QString & unit)
{
    visibleWykresType value;
    value.show = show;
    value.minVal = minV;
    value.maxVal = maxV;
    value.opis = opis;
    value.opis2 = opis2;
    value.jedn = unit;
    visibleWykres[wykresId] = value;
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


