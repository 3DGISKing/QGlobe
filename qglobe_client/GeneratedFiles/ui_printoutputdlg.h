/********************************************************************************
** Form generated from reading UI file 'printoutputdlg.ui'
**
** Created by: Qt User Interface Compiler version 5.15.18
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PRINTOUTPUTDLG_H
#define UI_PRINTOUTPUTDLG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_PrintOutputDlg
{
public:
    QLabel *label;
    QLineEdit *lineEdit;
    QPushButton *OKButton;
    QPushButton *cancelButton;

    void setupUi(QDialog *PrintOutputDlg)
    {
        if (PrintOutputDlg->objectName().isEmpty())
            PrintOutputDlg->setObjectName(QString::fromUtf8("PrintOutputDlg"));
        PrintOutputDlg->resize(372, 71);
        label = new QLabel(PrintOutputDlg);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(20, 20, 111, 16));
        lineEdit = new QLineEdit(PrintOutputDlg);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));
        lineEdit->setGeometry(QRect(20, 40, 251, 20));
        OKButton = new QPushButton(PrintOutputDlg);
        OKButton->setObjectName(QString::fromUtf8("OKButton"));
        OKButton->setGeometry(QRect(280, 10, 75, 23));
        OKButton->setAutoDefault(false);
        cancelButton = new QPushButton(PrintOutputDlg);
        cancelButton->setObjectName(QString::fromUtf8("cancelButton"));
        cancelButton->setGeometry(QRect(280, 40, 75, 23));
        cancelButton->setAutoDefault(false);

        retranslateUi(PrintOutputDlg);

        OKButton->setDefault(true);


        QMetaObject::connectSlotsByName(PrintOutputDlg);
    } // setupUi

    void retranslateUi(QDialog *PrintOutputDlg)
    {
        PrintOutputDlg->setWindowTitle(QCoreApplication::translate("PrintOutputDlg", "Output To File", nullptr));
        label->setText(QCoreApplication::translate("PrintOutputDlg", "Output File Name:", nullptr));
        OKButton->setText(QCoreApplication::translate("PrintOutputDlg", "Ok", nullptr));
        cancelButton->setText(QCoreApplication::translate("PrintOutputDlg", "Cancel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class PrintOutputDlg: public Ui_PrintOutputDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PRINTOUTPUTDLG_H
