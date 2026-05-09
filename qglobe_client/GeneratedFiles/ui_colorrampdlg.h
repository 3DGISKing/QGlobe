/********************************************************************************
** Form generated from reading UI file 'colorrampdlg.ui'
**
** Created by: Qt User Interface Compiler version 5.15.18
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_COLORRAMPDLG_H
#define UI_COLORRAMPDLG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_ColorRampDlg
{
public:
    QVBoxLayout *verticalLayout;
    QComboBox *comboBox;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButtonOk;
    QPushButton *pushButtonCancel;

    void setupUi(QDialog *ColorRampDlg)
    {
        if (ColorRampDlg->objectName().isEmpty())
            ColorRampDlg->setObjectName(QString::fromUtf8("ColorRampDlg"));
        ColorRampDlg->resize(256, 71);
        ColorRampDlg->setMaximumSize(QSize(256, 71));
        verticalLayout = new QVBoxLayout(ColorRampDlg);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        comboBox = new QComboBox(ColorRampDlg);
        comboBox->setObjectName(QString::fromUtf8("comboBox"));

        verticalLayout->addWidget(comboBox);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        pushButtonOk = new QPushButton(ColorRampDlg);
        pushButtonOk->setObjectName(QString::fromUtf8("pushButtonOk"));

        horizontalLayout->addWidget(pushButtonOk);

        pushButtonCancel = new QPushButton(ColorRampDlg);
        pushButtonCancel->setObjectName(QString::fromUtf8("pushButtonCancel"));

        horizontalLayout->addWidget(pushButtonCancel);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(ColorRampDlg);

        QMetaObject::connectSlotsByName(ColorRampDlg);
    } // setupUi

    void retranslateUi(QDialog *ColorRampDlg)
    {
        ColorRampDlg->setWindowTitle(QCoreApplication::translate("ColorRampDlg", "ColorRampDlg", nullptr));
        pushButtonOk->setText(QCoreApplication::translate("ColorRampDlg", "OK", nullptr));
        pushButtonCancel->setText(QCoreApplication::translate("ColorRampDlg", "Cancel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ColorRampDlg: public Ui_ColorRampDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_COLORRAMPDLG_H
