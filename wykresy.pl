import os
import math
from PIL import Image, ImageDraw, ImageFont

def getOsX(i, ratio, minv, maxv):
    valx = maxv - minv
    realVal = minv + 1.0 * i / ratio
    print valx
    if valx < 24:
        return "%2.1f\"" % realVal
    else:
        realVal = int(realVal);

    if valx < 250:
        return "%d\"" % realVal

    realVal = realVal / 60.0

    if (valx < 24000):
        return "%2.1f'" % realVal
    
    return "%d'" % int(realVal);

def getOsY(i, imax, minv, maxv):
    valy = maxv - minv
    realVal = (1.0 * i / imax)* valy + minv
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

minv = 10
maxv = 35
secsX = 360
minSecX = 240

img = Image.new("RGB", (width, height), "white")
draw = ImageDraw.Draw(img)
fnt = ImageFont.truetype('Pillow/Tests/fonts/FreeMono.ttf', 12)



points = []
for i in range(10000) :
    points.append(math.sqrt(1*i)*math.sin(0.1*i/math.pi)*math.sin(0.1*i/math.pi))
    print "point=",points[-1]
    #print math.sqrt(i)*math.sin(0.1*i/math.pi)*math.sin(0.1*i/math.pi)
    #points.append(0.1*i)
    
valperYpx = (height - margintop - marginbottom) / (maxv - minv);
valperXpx = (width - marginleft - marginright) / (secsX - minSecX);


print 0.1*valperXpx
draw.rectangle((0, 0, width - 1, height - 1), outline="black")
draw.text((0.45*width, height-5), "Czas, [s]", font=fnt, fill="black")

pixelxperGray = 5

nrSec = 0
actWidth = 0
    
while actWidth <= (width - marginright - marginleft):
    if nrSec % 10 == 0:
        draw.line((marginleft + actWidth, margintop + 0, marginleft + actWidth, height - marginbottom), width=3, fill="darkgray")
        draw.text((marginleft + actWidth - 15, height - 25), getOsX(actWidth, valperXpx, minSecX, secsX), font=fnt, fill="black")
    else:
        draw.line((marginleft + actWidth, margintop + 0, marginleft + actWidth, height - marginbottom), width=1, fill="gray")
    nrSec += 2
    actWidth += 2*pixelxperGray



maxHeight = height - margintop - marginbottom
maxWidth = width - marginleft - marginright

i = 0;
while i <= 100:
    y = maxHeight - (maxHeight*i/100.0)
    if i % 10 == 0 : 
        draw.text((5, y+margintop), getOsY(i, 100, minv, maxv), font=fnt, fill="black" )
        draw.line((marginleft, margintop+y, width-marginright, margintop+y), width = 3, fill = "darkgray" )
    else:
        draw.line((marginleft, margintop+y, width-marginright, margintop+y), width = 1, fill = "gray")
    i+=2

#przy zalozeniu ze mamy punktow 10*s  a musimy zmiescie do secX
nrdecSec = 0;
idP = 0;
##while nrdecSec < 10*secX and idP < points.size():
#

valXP = 0;
valX = 0;
nsum = 0;
fsum = 0
idp = 0
realValx = 0
valY = 0
valYP = 0
while valX <= maxWidth and idp < len(points):
    #print nsum, points[idp]
    fsum += points[idp]
    idp += 1
    nsum += 1
    realValx += 0.1*valperXpx
    valX = round(realValx)
    if valX != valXP :
        valY = fsum / nsum
        fsum = 0.0
        nsum = 0
        if valYP < maxv and valY > maxv:
            valY = maxv
        
        if valYP > minv and valY < minv:
            valY = minv

        if valYP > maxv and valY < maxv:
            valYP = maxv
        
        if valYP < minv and valY > minv:
            valYP = minv

        if valYP <= maxv and valYP >= minv and valY <= maxv and valY >= minv:
            v1 = maxHeight - 1.0 * (valYP - minv)*valperYpx
            v2 = maxHeight - 1.0 * (valY - minv)*valperYpx
            draw.line((marginleft + valXP, margintop + v1, marginleft + valX, margintop + v2), width=4, fill="blue")
            #print valY, v2
        valXP = valX
        valYP = valY
        
      

    
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