/********************************************************************************
** Form generated from reading UI file 'wersjadlg.ui'
**
** Created by: Qt User Interface Compiler version 5.12.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WERSJADLG_H
#define UI_WERSJADLG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QLabel>

QT_BEGIN_NAMESPACE

class Ui_WersjaDlg
{
public:
    QFormLayout *formLayout;
    QLabel *label;
    QLabel *compile_version;
    QLabel *label_3;
    QLabel *compile_data;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *WersjaDlg)
    {
        if (WersjaDlg->objectName().isEmpty())
            WersjaDlg->setObjectName(QString::fromUtf8("WersjaDlg"));
        WersjaDlg->resize(640, 89);
        formLayout = new QFormLayout(WersjaDlg);
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        label = new QLabel(WersjaDlg);
        label->setObjectName(QString::fromUtf8("label"));

        formLayout->setWidget(0, QFormLayout::LabelRole, label);

        compile_version = new QLabel(WersjaDlg);
        compile_version->setObjectName(QString::fromUtf8("compile_version"));

        formLayout->setWidget(0, QFormLayout::FieldRole, compile_version);

        label_3 = new QLabel(WersjaDlg);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        formLayout->setWidget(1, QFormLayout::LabelRole, label_3);

        compile_data = new QLabel(WersjaDlg);
        compile_data->setObjectName(QString::fromUtf8("compile_data"));

        formLayout->setWidget(1, QFormLayout::FieldRole, compile_data);

        buttonBox = new QDialogButtonBox(WersjaDlg);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setLayoutDirection(Qt::LeftToRight);
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Ok);
        buttonBox->setCenterButtons(true);

        formLayout->setWidget(2, QFormLayout::SpanningRole, buttonBox);


        retranslateUi(WersjaDlg);
        QObject::connect(buttonBox, SIGNAL(accepted()), WersjaDlg, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), WersjaDlg, SLOT(reject()));

        QMetaObject::connectSlotsByName(WersjaDlg);
    } // setupUi

    void retranslateUi(QDialog *WersjaDlg)
    {
        WersjaDlg->setWindowTitle(QApplication::translate("WersjaDlg", "Wersja", nullptr));
        label->setText(QApplication::translate("WersjaDlg", "Wersja kompilacji", nullptr));
        compile_version->setText(QApplication::translate("WersjaDlg", "0.0.0", nullptr));
        label_3->setText(QApplication::translate("WersjaDlg", "Data kompilacji", nullptr));
        compile_data->setText(QApplication::translate("WersjaDlg", "0", nullptr));
    } // retranslateUi

};

namespace Ui {
    class WersjaDlg: public Ui_WersjaDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WERSJADLG_H
