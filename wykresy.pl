import os
import math
from PIL import Image, ImageDraw, ImageFont


width = 1250
height = 950

minf = 0
maxf = 100

img = Image.new("RGB", (width, height), "white")
draw = ImageDraw.Draw(img)
fnt = ImageFont.truetype('Pillow/Tests/fonts/FreeMono.ttf', 12)



points = []
for i in range(100000) :
    points.append(math.sin(i/math.pi))
    
valperYpx = (height - 50) / (maxf - minf);
valperXpx = 10
draw.rectangle((0, 0, width - 1, height - 1), outline="black")
draw.text((0.45*width, height-5), "Czas, [s]", font=fnt, fill="black")

pixelsper1s = 5
beforeOSY = 50

nrSec = 0
actWidth = beforeOSY
    
while actWidth < width:
    if nrSec % 10 == 0:
        draw.line((actWidth, 0, actWidth, height-50), width=3, fill="darkgray")
        if nrSec != 240:
            draw.text((actWidth-15, height-25), "%d" %nrSec, font=fnt, fill="black")
    else:
        draw.line((actWidth, 0, actWidth, height-50), width=1, fill="gray")
    nrSec += 2
    actWidth += 2*pixelsper1s



maxHeight = height - 50
i = 0;
while i < 100:
    y = maxHeight - (maxHeight*i/100.0)
    if i % 10 == 0 : 
        draw.text((25, y), "%d" % i, font=fnt, fill="black" )
        draw.line((beforeOSY, y, width, y), width = 3, fill = "darkgray" )
    else:
        draw.line((beforeOSY, y, width, y), width = 1, fill = "gray")
    i+=2


    draw.line((beforeOSY, 2, width, 2), width = 3, fill = "darkgray")
    draw.line((width - 1, 0, width - 1, height-50), width = 3, fill = "darkgray")
    
    draw.text((10, 20), "100", font=fnt, fill="black");

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