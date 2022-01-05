#include "czujnikanalogowyustawieniaokno.h"
#include "ui_czujnikanalogowyustawieniaokno.h"
#include "czujnikianalogoweokno.h"
#include <QPushButton>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QDialogButtonBox>

#include "czujnikanalogowyustawieniawidget.h"


CzujnikAnalogowyUstawieniaOkno::CzujnikAnalogowyUstawieniaOkno(const Ustawienia &sett, QWidget *parent) :
    QDialog(parent)
{

    gridLayout = new QGridLayout(this);
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));

    buttonBox = new QDialogButtonBox(this);
    buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
    buttonBox->setOrientation(Qt::Horizontal);
    buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
    buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);

    for (int i = 0; i < sett.maxCzujek; i++) {
        createOneElement(i, QString("Czujnik %1").arg(i+1));
        connect(czujniki[i], &CzujnikAnalogowyUstawieniaWidget::updateCzujnik,
                this, &CzujnikAnalogowyUstawieniaOkno::updateCzujnik);
    }

    for (int i=0; i < sett.maxCzujek; ++i) {
        auto r = sett.getCzujka(i+1);
        czujniki[i]->setData(r.name, r.unit, r.ratio);
    }


    gridLayout->addWidget(buttonBox, sett.maxCzujek/2+1, 0, 1, 2);
    updateCzujnik();

    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    QMetaObject::connectSlotsByName(this);
    setWindowTitle(QCoreApplication::translate("CzujnikAnalogowyUstawieniaOkno", "Ustawienia parametr\303\263w sygna\305\202\303\263w", nullptr));
}



CzujnikAnalogowyUstawieniaOkno::~CzujnikAnalogowyUstawieniaOkno()
{

}

void CzujnikAnalogowyUstawieniaOkno::saveData(Ustawienia &ust)
{
    for (int i = 0; i < ust.maxCzujek; ++i) {
        ust.setCzujka(i, czujniki[i]->name(), czujniki[i]->unit(), czujniki[i]->ratio());
    }
}

void CzujnikAnalogowyUstawieniaOkno::updateCzujnik()
{
    bool valid = true;
    for (int i = 0; i < Ustawienia::maxCzujek; i++) {
        if (!czujniki[i]->valid()) {
            valid = false;
            break;
        }
    }
    buttonBox->button(QDialogButtonBox::Ok)->setEnabled(valid);
}

void CzujnikAnalogowyUstawieniaOkno::createOneElement(int id, const QString & titleBox)
{
    QGroupBox *groupBox = new QGroupBox(titleBox, this);
    groupBox->setObjectName(QString("groupBox_%1").arg(id));

    QHBoxLayout *horizontalLayout = new QHBoxLayout(groupBox);;
    horizontalLayout->setObjectName(QString("horizontalLayout_%1").arg(id));

    CzujnikAnalogowyUstawieniaWidget *widget = new CzujnikAnalogowyUstawieniaWidget(groupBox);
    widget->setObjectName(QString::fromUtf8("widget_%1").arg(id));
    horizontalLayout->addWidget(widget);

    czujniki[id] = widget;

    gridLayout->addWidget(groupBox, id / 2, id % 2, 1, 1);
}
