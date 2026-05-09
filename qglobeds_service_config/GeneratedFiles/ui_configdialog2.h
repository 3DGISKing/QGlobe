/********************************************************************************
** Form generated from reading UI file 'configdialog2.ui'
**
** Created by: Qt User Interface Compiler version 5.15.18
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CONFIGDIALOG2_H
#define UI_CONFIGDIALOG2_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>

QT_BEGIN_NAMESPACE

class Ui_ConfigDialog2
{
public:
    QDialogButtonBox *buttonBox;
    QGroupBox *groupBox;
    QLabel *label_8;
    QPushButton *browseButton_2;
    QPushButton *browseButton;
    QLineEdit *dbpathEdit;
    QLabel *label_6;
    QLabel *label_7;
    QSpinBox *srvportSpin;
    QLineEdit *dbpathEdit_2;
    QSpinBox *srvconnectSpin;
    QLabel *label_2;

    void setupUi(QDialog *ConfigDialog2)
    {
        if (ConfigDialog2->objectName().isEmpty())
            ConfigDialog2->setObjectName(QString::fromUtf8("ConfigDialog2"));
        ConfigDialog2->resize(440, 231);
        ConfigDialog2->setMinimumSize(QSize(440, 231));
        ConfigDialog2->setMaximumSize(QSize(440, 231));
        buttonBox = new QDialogButtonBox(ConfigDialog2);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setGeometry(QRect(180, 195, 231, 30));
        buttonBox->setStandardButtons(QDialogButtonBox::Apply|QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        groupBox = new QGroupBox(ConfigDialog2);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(15, 15, 411, 171));
        label_8 = new QLabel(groupBox);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setGeometry(QRect(15, 135, 98, 20));
        QFont font;
        font.setPointSize(10);
        label_8->setFont(font);
        browseButton_2 = new QPushButton(groupBox);
        browseButton_2->setObjectName(QString::fromUtf8("browseButton_2"));
        browseButton_2->setGeometry(QRect(370, 132, 26, 26));
        browseButton = new QPushButton(groupBox);
        browseButton->setObjectName(QString::fromUtf8("browseButton"));
        browseButton->setGeometry(QRect(370, 92, 26, 26));
        dbpathEdit = new QLineEdit(groupBox);
        dbpathEdit->setObjectName(QString::fromUtf8("dbpathEdit"));
        dbpathEdit->setGeometry(QRect(133, 95, 237, 20));
        label_6 = new QLabel(groupBox);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(15, 95, 98, 20));
        label_6->setFont(font);
        label_7 = new QLabel(groupBox);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setGeometry(QRect(15, 55, 101, 20));
        label_7->setFont(font);
        srvportSpin = new QSpinBox(groupBox);
        srvportSpin->setObjectName(QString::fromUtf8("srvportSpin"));
        srvportSpin->setGeometry(QRect(133, 14, 262, 20));
        srvportSpin->setMaximum(65535);
        dbpathEdit_2 = new QLineEdit(groupBox);
        dbpathEdit_2->setObjectName(QString::fromUtf8("dbpathEdit_2"));
        dbpathEdit_2->setGeometry(QRect(133, 135, 237, 20));
        srvconnectSpin = new QSpinBox(groupBox);
        srvconnectSpin->setObjectName(QString::fromUtf8("srvconnectSpin"));
        srvconnectSpin->setGeometry(QRect(133, 55, 262, 20));
        srvconnectSpin->setMaximum(65535);
        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(15, 13, 101, 17));
        label_2->setFont(font);

        retranslateUi(ConfigDialog2);

        QMetaObject::connectSlotsByName(ConfigDialog2);
    } // setupUi

    void retranslateUi(QDialog *ConfigDialog2)
    {
        ConfigDialog2->setWindowTitle(QCoreApplication::translate("ConfigDialog2", "Service Options", nullptr));
        groupBox->setTitle(QString());
        label_8->setText(QCoreApplication::translate("ConfigDialog2", "Database path 2:", nullptr));
        browseButton_2->setText(QCoreApplication::translate("ConfigDialog2", "...", nullptr));
        browseButton->setText(QCoreApplication::translate("ConfigDialog2", "...", nullptr));
        label_6->setText(QCoreApplication::translate("ConfigDialog2", "Database path 1:", nullptr));
        label_7->setText(QCoreApplication::translate("ConfigDialog2", "Max Connections:", nullptr));
        label_2->setText(QCoreApplication::translate("ConfigDialog2", "Server Port :", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ConfigDialog2: public Ui_ConfigDialog2 {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONFIGDIALOG2_H
