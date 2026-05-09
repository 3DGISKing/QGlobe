/********************************************************************************
** Form generated from reading UI file 'licencedlg.ui'
**
** Created by: Qt User Interface Compiler version 5.15.18
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LICENCEDLG_H
#define UI_LICENCEDLG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_LicenceDlg
{
public:
    QVBoxLayout *verticalLayout;
    QGroupBox *groupBox_2;
    QHBoxLayout *horizontalLayout_3;
    QLineEdit *lineEditModelNumber;
    QSpacerItem *verticalSpacer_2;
    QGroupBox *groupBox;
    QHBoxLayout *horizontalLayout_2;
    QLineEdit *lineEditLicence;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButtonLicence;
    QPushButton *pushButtonClose;

    void setupUi(QDialog *LicenceDlg)
    {
        if (LicenceDlg->objectName().isEmpty())
            LicenceDlg->setObjectName(QString::fromUtf8("LicenceDlg"));
        LicenceDlg->resize(241, 125);
        LicenceDlg->setMaximumSize(QSize(241, 125));
        verticalLayout = new QVBoxLayout(LicenceDlg);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        groupBox_2 = new QGroupBox(LicenceDlg);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        horizontalLayout_3 = new QHBoxLayout(groupBox_2);
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setContentsMargins(2, 2, 2, 2);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        lineEditModelNumber = new QLineEdit(groupBox_2);
        lineEditModelNumber->setObjectName(QString::fromUtf8("lineEditModelNumber"));
        lineEditModelNumber->setReadOnly(true);

        horizontalLayout_3->addWidget(lineEditModelNumber);


        verticalLayout->addWidget(groupBox_2);

        verticalSpacer_2 = new QSpacerItem(223, 3, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_2);

        groupBox = new QGroupBox(LicenceDlg);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        horizontalLayout_2 = new QHBoxLayout(groupBox);
        horizontalLayout_2->setSpacing(2);
        horizontalLayout_2->setContentsMargins(2, 2, 2, 2);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        lineEditLicence = new QLineEdit(groupBox);
        lineEditLicence->setObjectName(QString::fromUtf8("lineEditLicence"));

        horizontalLayout_2->addWidget(lineEditLicence);


        verticalLayout->addWidget(groupBox);

        verticalSpacer = new QSpacerItem(20, 8, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        pushButtonLicence = new QPushButton(LicenceDlg);
        pushButtonLicence->setObjectName(QString::fromUtf8("pushButtonLicence"));

        horizontalLayout->addWidget(pushButtonLicence);

        pushButtonClose = new QPushButton(LicenceDlg);
        pushButtonClose->setObjectName(QString::fromUtf8("pushButtonClose"));

        horizontalLayout->addWidget(pushButtonClose);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(LicenceDlg);

        QMetaObject::connectSlotsByName(LicenceDlg);
    } // setupUi

    void retranslateUi(QDialog *LicenceDlg)
    {
        LicenceDlg->setWindowTitle(QCoreApplication::translate("LicenceDlg", "LicenceDlg", nullptr));
        groupBox_2->setTitle(QString());
        groupBox->setTitle(QString());
        pushButtonLicence->setText(QCoreApplication::translate("LicenceDlg", "Licence", nullptr));
        pushButtonClose->setText(QCoreApplication::translate("LicenceDlg", "Close", nullptr));
    } // retranslateUi

};

namespace Ui {
    class LicenceDlg: public Ui_LicenceDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LICENCEDLG_H
