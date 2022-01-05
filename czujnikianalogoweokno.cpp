#include "czujnikianalogoweokno.h"
#include "ui_czujnikianalogoweokno.h"
#include "czujnikanalogowywidget.h"

#include <QFrame>
#include <QHBoxLayout>
#include "czujnikanalogowywidget.h"

CzujnikiAnalogoweOkno::CzujnikiAnalogoweOkno(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CzujnikiAnalogoweOkno)
{
    setObjectName(QString::fromUtf8("CzujnikiAnalogoweOkno"));
    resize(990, 130);
    setMinimumSize(QSize(960, 130));
    gridLayout = new QGridLayout(this);
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
    QMetaObject::connectSlotsByName(this);

    ui->setupUi(this);

    for (int i=0; i< Ustawienia::maxCzujek; ++i) {
        createOne(i);
    }
}

CzujnikiAnalogoweOkno::~CzujnikiAnalogoweOkno()
{
    delete ui;
}

void CzujnikiAnalogoweOkno::setParams(const Ustawienia &ust)
{
    for (int i=0; i<ust.maxCzujek; ++i) {
        czujniki[i]->setParam(ust.getName(i+1), ust.getRatio(i+1), ust.getUnit(i+1));
    }
}

void CzujnikiAnalogoweOkno::updateValue(int id, const double & val)
{
    if (id <= 0 || id > 6)
        return;
    czujniki[id]->setValue(val);
    emit valueChange(id, czujniki[id]->getConvValue());
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
    horizontalLayout->setContentsMargins(0, 0, 0, 0);

    CzujnikAnalogowyWidget *widget = new CzujnikAnalogowyWidget(frame);
    widget->setObjectName(QString::fromUtf8("widget_%1").arg(id));

    horizontalLayout->addWidget(widget);

    gridLayout->addWidget(frame, id/4, id%4, 1, 1);
    czujniki[id] = widget;
}
