/********************************************************************************
** Form generated from reading UI file 'writeremovejgw.ui'
**
** Created by: Qt User Interface Compiler version 5.15.18
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WRITEREMOVEJGW_H
#define UI_WRITEREMOVEJGW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_WriteRemoveJGWClass
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label;
    QLineEdit *Open_Folder;
    QPushButton *Open_Folder_pushButton;
    QCheckBox *actionJGWRemove;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *Run_pushButton;

    void setupUi(QDialog *WriteRemoveJGWClass)
    {
        if (WriteRemoveJGWClass->objectName().isEmpty())
            WriteRemoveJGWClass->setObjectName(QString::fromUtf8("WriteRemoveJGWClass"));
        WriteRemoveJGWClass->resize(494, 102);
        verticalLayout = new QVBoxLayout(WriteRemoveJGWClass);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label = new QLabel(WriteRemoveJGWClass);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout_2->addWidget(label);

        Open_Folder = new QLineEdit(WriteRemoveJGWClass);
        Open_Folder->setObjectName(QString::fromUtf8("Open_Folder"));

        horizontalLayout_2->addWidget(Open_Folder);

        Open_Folder_pushButton = new QPushButton(WriteRemoveJGWClass);
        Open_Folder_pushButton->setObjectName(QString::fromUtf8("Open_Folder_pushButton"));

        horizontalLayout_2->addWidget(Open_Folder_pushButton);


        verticalLayout->addLayout(horizontalLayout_2);

        actionJGWRemove = new QCheckBox(WriteRemoveJGWClass);
        actionJGWRemove->setObjectName(QString::fromUtf8("actionJGWRemove"));

        verticalLayout->addWidget(actionJGWRemove);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        Run_pushButton = new QPushButton(WriteRemoveJGWClass);
        Run_pushButton->setObjectName(QString::fromUtf8("Run_pushButton"));

        horizontalLayout->addWidget(Run_pushButton);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(WriteRemoveJGWClass);

        QMetaObject::connectSlotsByName(WriteRemoveJGWClass);
    } // setupUi

    void retranslateUi(QDialog *WriteRemoveJGWClass)
    {
        WriteRemoveJGWClass->setWindowTitle(QCoreApplication::translate("WriteRemoveJGWClass", "WriteRemoveJGW", nullptr));
        label->setText(QCoreApplication::translate("WriteRemoveJGWClass", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Folder exisiting jpg files</p></body></html>", nullptr));
        Open_Folder_pushButton->setText(QCoreApplication::translate("WriteRemoveJGWClass", "Browser...", nullptr));
        actionJGWRemove->setText(QCoreApplication::translate("WriteRemoveJGWClass", "JGW File Remove", nullptr));
        Run_pushButton->setText(QCoreApplication::translate("WriteRemoveJGWClass", "Run", nullptr));
    } // retranslateUi

};

namespace Ui {
    class WriteRemoveJGWClass: public Ui_WriteRemoveJGWClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WRITEREMOVEJGW_H
