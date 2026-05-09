/********************************************************************************
** Form generated from reading UI file 'hopedbtogdem.ui'
**
** Created by: Qt User Interface Compiler version 5.15.18
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_HOPEDBTOQGLOBE_H
#define UI_HOPEDBTOQGLOBE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_HopeDBToGDemClass
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *sourcePathlabel;
    QLineEdit *textEditInputIEEIDBPath;
    QPushButton *pushButtonBrowserInput;
    QHBoxLayout *horizontalLayout_2;
    QLabel *destPathlabel;
    QLineEdit *textsaveGTPath;
    QPushButton *pushButtonBrowserSave;
    QProgressBar *progressBarGtCount;
    QHBoxLayout *horizontalLayout_3;
    QSpacerItem *horizontalSpacer;
    QPushButton *ConvertBtn;

    void setupUi(QDialog *HopeDBToGDemClass)
    {
        if (HopeDBToGDemClass->objectName().isEmpty())
            HopeDBToGDemClass->setObjectName(QString::fromUtf8("HopeDBToGDemClass"));
        HopeDBToGDemClass->resize(503, 138);
        verticalLayout = new QVBoxLayout(HopeDBToGDemClass);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        sourcePathlabel = new QLabel(HopeDBToGDemClass);
        sourcePathlabel->setObjectName(QString::fromUtf8("sourcePathlabel"));

        horizontalLayout->addWidget(sourcePathlabel);

        textEditInputIEEIDBPath = new QLineEdit(HopeDBToGDemClass);
        textEditInputIEEIDBPath->setObjectName(QString::fromUtf8("textEditInputIEEIDBPath"));
        textEditInputIEEIDBPath->setReadOnly(true);

        horizontalLayout->addWidget(textEditInputIEEIDBPath);

        pushButtonBrowserInput = new QPushButton(HopeDBToGDemClass);
        pushButtonBrowserInput->setObjectName(QString::fromUtf8("pushButtonBrowserInput"));

        horizontalLayout->addWidget(pushButtonBrowserInput);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        destPathlabel = new QLabel(HopeDBToGDemClass);
        destPathlabel->setObjectName(QString::fromUtf8("destPathlabel"));

        horizontalLayout_2->addWidget(destPathlabel);

        textsaveGTPath = new QLineEdit(HopeDBToGDemClass);
        textsaveGTPath->setObjectName(QString::fromUtf8("textsaveGTPath"));
        textsaveGTPath->setReadOnly(true);

        horizontalLayout_2->addWidget(textsaveGTPath);

        pushButtonBrowserSave = new QPushButton(HopeDBToGDemClass);
        pushButtonBrowserSave->setObjectName(QString::fromUtf8("pushButtonBrowserSave"));

        horizontalLayout_2->addWidget(pushButtonBrowserSave);


        verticalLayout->addLayout(horizontalLayout_2);

        progressBarGtCount = new QProgressBar(HopeDBToGDemClass);
        progressBarGtCount->setObjectName(QString::fromUtf8("progressBarGtCount"));
        progressBarGtCount->setValue(24);

        verticalLayout->addWidget(progressBarGtCount);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer);

        ConvertBtn = new QPushButton(HopeDBToGDemClass);
        ConvertBtn->setObjectName(QString::fromUtf8("ConvertBtn"));

        horizontalLayout_3->addWidget(ConvertBtn);


        verticalLayout->addLayout(horizontalLayout_3);


        retranslateUi(HopeDBToGDemClass);

        QMetaObject::connectSlotsByName(HopeDBToGDemClass);
    } // setupUi

    void retranslateUi(QDialog *HopeDBToGDemClass)
    {
        HopeDBToGDemClass->setWindowTitle(QCoreApplication::translate("HopeDBToGDemClass", "HopeDBToGDem", nullptr));
        sourcePathlabel->setText(QCoreApplication::translate("HopeDBToGDemClass", "Jpg Path", nullptr));
        pushButtonBrowserInput->setText(QCoreApplication::translate("HopeDBToGDemClass", "Browser...", nullptr));
        destPathlabel->setText(QCoreApplication::translate("HopeDBToGDemClass", "Save Path", nullptr));
        pushButtonBrowserSave->setText(QCoreApplication::translate("HopeDBToGDemClass", "Browser...", nullptr));
        ConvertBtn->setText(QCoreApplication::translate("HopeDBToGDemClass", "Make Gt Files", nullptr));
    } // retranslateUi

};

namespace Ui {
    class HopeDBToGDemClass: public Ui_HopeDBToGDemClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_HOPEDBTOQGLOBE_H
