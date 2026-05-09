/********************************************************************************
** Form generated from reading UI file 'gotodialog.ui'
**
** Created by: Qt User Interface Compiler version 5.15.18
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GOTODIALOG_H
#define UI_GOTODIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_GoToDialog
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QSpacerItem *horizontalSpacer_4;
    QLineEdit *lineEditLongitude;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_2;
    QSpacerItem *horizontalSpacer_3;
    QLineEdit *lineEditLatitude;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_3;
    QSpacerItem *horizontalSpacer_2;
    QLineEdit *lineEditHeight;
    QHBoxLayout *horizontalLayout_4;
    QComboBox *comboBoxUnit;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButtonGoto;
    QPushButton *pushButtonClose;

    void setupUi(QDialog *GoToDialog)
    {
        if (GoToDialog->objectName().isEmpty())
            GoToDialog->setObjectName(QString::fromUtf8("GoToDialog"));
        GoToDialog->resize(266, 129);
        GoToDialog->setMaximumSize(QSize(266, 129));
        verticalLayout = new QVBoxLayout(GoToDialog);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label = new QLabel(GoToDialog);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout->addWidget(label);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_4);

        lineEditLongitude = new QLineEdit(GoToDialog);
        lineEditLongitude->setObjectName(QString::fromUtf8("lineEditLongitude"));

        horizontalLayout->addWidget(lineEditLongitude);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label_2 = new QLabel(GoToDialog);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        horizontalLayout_2->addWidget(label_2);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_3);

        lineEditLatitude = new QLineEdit(GoToDialog);
        lineEditLatitude->setObjectName(QString::fromUtf8("lineEditLatitude"));

        horizontalLayout_2->addWidget(lineEditLatitude);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        label_3 = new QLabel(GoToDialog);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        horizontalLayout_3->addWidget(label_3);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_2);

        lineEditHeight = new QLineEdit(GoToDialog);
        lineEditHeight->setObjectName(QString::fromUtf8("lineEditHeight"));

        horizontalLayout_3->addWidget(lineEditHeight);


        verticalLayout->addLayout(horizontalLayout_3);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        comboBoxUnit = new QComboBox(GoToDialog);
        comboBoxUnit->setObjectName(QString::fromUtf8("comboBoxUnit"));

        horizontalLayout_4->addWidget(comboBoxUnit);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer);

        pushButtonGoto = new QPushButton(GoToDialog);
        pushButtonGoto->setObjectName(QString::fromUtf8("pushButtonGoto"));

        horizontalLayout_4->addWidget(pushButtonGoto);

        pushButtonClose = new QPushButton(GoToDialog);
        pushButtonClose->setObjectName(QString::fromUtf8("pushButtonClose"));

        horizontalLayout_4->addWidget(pushButtonClose);


        verticalLayout->addLayout(horizontalLayout_4);


        retranslateUi(GoToDialog);

        QMetaObject::connectSlotsByName(GoToDialog);
    } // setupUi

    void retranslateUi(QDialog *GoToDialog)
    {
        GoToDialog->setWindowTitle(QCoreApplication::translate("GoToDialog", "GoToDialog", nullptr));
        label->setText(QCoreApplication::translate("GoToDialog", "Longitude", nullptr));
        label_2->setText(QCoreApplication::translate("GoToDialog", "Latitude", nullptr));
        label_3->setText(QCoreApplication::translate("GoToDialog", "Height", nullptr));
        pushButtonGoto->setText(QCoreApplication::translate("GoToDialog", "Goto", nullptr));
        pushButtonClose->setText(QCoreApplication::translate("GoToDialog", "Close", nullptr));
    } // retranslateUi

};

namespace Ui {
    class GoToDialog: public Ui_GoToDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GOTODIALOG_H
