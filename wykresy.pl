import os
import math
from PIL import Image, ImageDraw, ImageFont

def getOsX(i, ratio, minv, maxv):
    valx = maxv - minv
    realVal = i / ratio
    if valx < 120:
        return "%2.1f\"" % realVal
    else:
        realVal = int(realVal);

    if valx < 250:
        return "%d\"" % realVal

    realVal = realVal / 60.0

    if (valx < 12000):
        return "%2.1f'" % realVal
    
    return "%d'" % int(realVal);

def getOsY(i, imax, minv, maxv):
    valy = maxv - minv
    realVal = (i / imax)* valy + minv
    if valy < 1:
        return "%1.3f" % realVal
    if valy < 10:
        return "%2.2f" % realVal

    return "%2.0f" % realVal



width = 1300
height = 900
marginleft = 50
marginright = 50
margintop = 50
marginbottom = 50

minv = 15
maxv = 115
secsX = 12

img = Image.new("RGB", (width, height), "white")
draw = ImageDraw.Draw(img)
fnt = ImageFont.truetype('Pillow/Tests/fonts/FreeMono.ttf', 12)



points = []
for i in range(100000) :
    points.append(math.sin(i/math.pi))
    
valperYpx = (height - margintop - marginbottom) / (maxv - minv);
valperXpx = (width - marginleft - marginright) / secsX;

draw.rectangle((0, 0, width - 1, height - 1), outline="black")
draw.text((0.45*width, height-5), "Czas, [s]", font=fnt, fill="black")

pixelsper1s = 5
beforeOSY = 50

nrSec = 0
actWidth = 0
    
while actWidth <= (width - marginright - marginleft):
    if nrSec % 10 == 0:
        draw.line((marginleft + actWidth, margintop + 0, marginleft + actWidth, height - marginbottom), width=3, fill="darkgray")
        draw.text((marginleft + actWidth - 15, height - 25), getOsX(actWidth, valperXpx, 0, secsX), font=fnt, fill="black")
    else:
        draw.line((marginleft + actWidth, margintop + 0, marginleft + actWidth, height - marginbottom), width=1, fill="gray")
    nrSec += 2
    actWidth += 2*pixelsper1s



maxHeight = height - margintop - marginbottom;
i = 0;
while i <= 100:
    y = maxHeight - (maxHeight*i/100.0)
    if i % 10 == 0 : 
        draw.text((5, y+margintop), getOsY(i, 100, minv, maxv), font=fnt, fill="black" )
        draw.line((marginleft, margintop+y, width-marginright, margintop+y), width = 3, fill = "darkgray" )
    else:
        draw.line((marginleft, margintop+y, width-marginright, margintop+y), width = 1, fill = "gray")
    i+=2


    #draw.line((beforeOSY, 2, width, 2), width = 3, fill = "darkgray")
    #draw.line((width - 1, 0, width - 1, height-50), width = 3, fill = "darkgray")
    
    #draw.text((10, 20), "100", font=fnt, fill="black");

"""

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
"""
img.show()    