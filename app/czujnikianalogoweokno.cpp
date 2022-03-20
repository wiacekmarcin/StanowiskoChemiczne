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
        czujniki[i]->setParam(ust.getName(i), ust.getRatio(i), ust.getUnit(i));
    }
}

void CzujnikiAnalogoweOkno::updateValue(int id, const double & val)
{
    qDebug("%s:%d %d %f",__FILE__,__LINE__,id,val);
    if (id < 0 || id >= Ustawienia::maxCzujekAnal)
        return;
    czujniki[id]->setValue(val);
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
}


void CzujnikiAnalogoweOkno::setHorizontalSize(unsigned int w)
{
    for (int i = 0; i < Ustawienia::maxCzujekAnal; ++i) {
        QSize s = czujniki[i]->size();
        s.setWidth(2 * w / Ustawienia::maxCzujekAnal);
        czujniki[i]->setFixedSize(s);
    }
}
