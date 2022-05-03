#include "czujnikianalogoweustawieniaframe.h"
#include "czujnikianalogoweokno.h"
#include <QPushButton>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QDialogButtonBox>
#include <QCoreApplication>

#include "czujnikanalogowyustawieniawidget.h"
#include <QTimer>

CzujnikiAnalogoweUstawieniaFrame::CzujnikiAnalogoweUstawieniaFrame(QWidget *parent) :
    QFrame( parent )
{




}



CzujnikiAnalogoweUstawieniaFrame::~CzujnikiAnalogoweUstawieniaFrame()
{

}

void CzujnikiAnalogoweUstawieniaFrame::setUstawienia(const UserPrivilige & user, const Ustawienia &sett)
{
    gridLayout = new QGridLayout(this);
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));

    for (int i = 0; i < sett.maxCzujekAnal; i++) {
        createOneElement(i, QString("Czujnik %1").arg(i+1));
        czujniki[i]->setData(user, sett.getCzujnikAnalogowyUstawienia((analogIn)i));
        connect(czujniki[i], &CzujnikAnalogowyUstawieniaWidget::updateCzujnik,
                this, &CzujnikiAnalogoweUstawieniaFrame::updateCzujnik);
    }



    QMetaObject::connectSlotsByName(this);
    setWindowTitle(QCoreApplication::translate("CzujnikAnalogowyUstawieniaOkno", "Ustawienia parametr\303\263w sygna\305\202\303\263w", nullptr));
}

void CzujnikiAnalogoweUstawieniaFrame::saveData(Ustawienia &ust)
{
    for (int i = 0; i < ust.maxCzujekAnal; ++i) {
        ust.setCzujka((analogIn)i, czujniki[i]->getUstawienia());
    }
}

void CzujnikiAnalogoweUstawieniaFrame::updateCzujnik()
{
    for (int i = 0; i < Ustawienia::maxCzujekAnal; i++) {
        if (!czujniki[i]->valid()) {
            break;
        }
    }
}

void CzujnikiAnalogoweUstawieniaFrame::createOneElement(int id, const QString & titleBox)
{
    QGroupBox *groupBox = new QGroupBox(titleBox, this);
    groupBox->setObjectName(QString("groupBox_%1").arg(id));

    QHBoxLayout *horizontalLayout = new QHBoxLayout(groupBox);;
    horizontalLayout->setObjectName(QString("horizontalLayout_%1").arg(id));

    CzujnikAnalogowyUstawieniaWidget *widget = new CzujnikAnalogowyUstawieniaWidget(groupBox);
    widget->setObjectName(QString::fromUtf8("widget_%1").arg(id));
    horizontalLayout->addWidget(widget);

    czujniki[id] = widget;

    gridLayout->addWidget(groupBox, id / 4, id % 4, 1, 1);
}
