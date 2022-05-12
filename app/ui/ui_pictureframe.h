/********************************************************************************
** Form generated from reading UI file 'pictureframe.ui'
**
** Created by: Qt User Interface Compiler version 5.12.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PICTUREFRAME_H
#define UI_PICTUREFRAME_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_PictureFrame
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *filename;
    QLabel *image;
    QSpacerItem *verticalSpacer;

    void setupUi(QFrame *PictureFrame)
    {
        if (PictureFrame->objectName().isEmpty())
            PictureFrame->setObjectName(QString::fromUtf8("PictureFrame"));
        PictureFrame->resize(400, 300);
        verticalLayout = new QVBoxLayout(PictureFrame);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        filename = new QLabel(PictureFrame);
        filename->setObjectName(QString::fromUtf8("filename"));

        verticalLayout->addWidget(filename);

        image = new QLabel(PictureFrame);
        image->setObjectName(QString::fromUtf8("image"));

        verticalLayout->addWidget(image);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);


        retranslateUi(PictureFrame);

        QMetaObject::connectSlotsByName(PictureFrame);
    } // setupUi

    void retranslateUi(QFrame *PictureFrame)
    {
        PictureFrame->setWindowTitle(QApplication::translate("PictureFrame", "Frame", nullptr));
        filename->setText(QString());
        image->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class PictureFrame: public Ui_PictureFrame {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PICTUREFRAME_H
