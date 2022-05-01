#include "czujnikianalogoweokno.h"
#include "czujnikanalogowywidget.h"

#include <QFrame>
#include <QHBoxLayout>
#include "czujnikanalogowywidget.h"
#include <QResizeEvent>

CzujnikiAnalogoweOkno::CzujnikiAnalogoweOkno(QWidget *parent) :
    QWidget(parent)
{
    setObjectName(QString::fromUtf8("CzujnikiAnalogoweOkno"));
    resize(1540, 130);
    setMinimumSize(QSize(1540, 130));
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
    gridLayout = new QGridLayout(this);
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
    QMetaObject::connectSlotsByName(this);
    gridLayout->setMargin(10);

    for (int i=0; i< Ustawienia::maxCzujekAnal; ++i) {
        createOne(i);
    }
}

CzujnikiAnalogoweOkno::~CzujnikiAnalogoweOkno()
{

}

void CzujnikiAnalogoweOkno::setParams(const Ustawienia &ust)
{
    for (int i=0; i<ust.maxCzujekAnal; ++i) {
        czujniki[i]->setParam(ust.getCzujnikAnalogowyUstawienia((analogIn)i));
    }
}

void CzujnikiAnalogoweOkno::updateValue(double val1, double val2, double val3, double val4, double val5, double val6, double val7, double val8)

{
    //
    czujniki[0]->setValue(val1);
    czujniki[1]->setValue(val2);
    czujniki[2]->setValue(val3);
    czujniki[3]->setValue(val4);
    czujniki[4]->setValue(val5);
    czujniki[5]->setValue(val6);
    czujniki[6]->setValue(val7);
    czujniki[7]->setValue(val8);
}


void CzujnikiAnalogoweOkno::createOne(int id)
{
    QFrame *frame = new QFrame(this);
    frame->setObjectName(QString("frame_%1").arg(id));
    frame->setFrameShape(QFrame::StyledPanel);
    frame->setFrameShadow(QFrame::Raised);

    QHBoxLayout *horizontalLayout = new QHBoxLayout(frame);
    horizontalLayout->setSpacing(0);
    horizontalLayout->setObjectName(QString("horizontalLayout_%1").arg(id));
    horizontalLayout->setContentsMargins(1,1,1,1);
    frame->setFrameShape(QFrame::Box);

    CzujnikAnalogowyWidget *widget = new CzujnikAnalogowyWidget(frame);
    widget->setObjectName(QString::fromUtf8("widget_%1").arg(id));

    horizontalLayout->addWidget(widget);

    gridLayout->addWidget(frame, id/4, id%4, 1, 1);

    czujniki[id] = widget;

    czujniki[id]->setPrec(1);
}


void CzujnikiAnalogoweOkno::setHorizontalSize(unsigned int w)
{
    for (int i = 0; i < Ustawienia::maxCzujekAnal; ++i) {
        QSize s = czujniki[i]->size();
        s.setWidth(2 * w / Ustawienia::maxCzujekAnal);
        czujniki[i]->setFixedSize(s);
    }
}
