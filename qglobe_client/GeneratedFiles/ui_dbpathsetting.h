/********************************************************************************
** Form generated from reading UI file 'dbpathsetting.ui'
**
** Created by: Qt User Interface Compiler version 5.15.18
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DBPATHSETTING_H
#define UI_DBPATHSETTING_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QToolButton>

QT_BEGIN_NAMESPACE

class Ui_SetDBPathDlg
{
public:
    QLabel *label;
    QLineEdit *pathEdit;
    QToolButton *toolButton;
    QPushButton *OKButton;
    QPushButton *cancelButton;

    void setupUi(QDialog *SetDBPathDlg)
    {
        if (SetDBPathDlg->objectName().isEmpty())
            SetDBPathDlg->setObjectName(QString::fromUtf8("SetDBPathDlg"));
        SetDBPathDlg->resize(438, 116);
        label = new QLabel(SetDBPathDlg);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(18, 30, 81, 20));
        pathEdit = new QLineEdit(SetDBPathDlg);
        pathEdit->setObjectName(QString::fromUtf8("pathEdit"));
        pathEdit->setGeometry(QRect(100, 30, 291, 20));
        pathEdit->setReadOnly(true);
        toolButton = new QToolButton(SetDBPathDlg);
        toolButton->setObjectName(QString::fromUtf8("toolButton"));
        toolButton->setGeometry(QRect(390, 30, 25, 20));
        OKButton = new QPushButton(SetDBPathDlg);
        OKButton->setObjectName(QString::fromUtf8("OKButton"));
        OKButton->setGeometry(QRect(260, 70, 75, 23));
        OKButton->setAutoDefault(false);
        cancelButton = new QPushButton(SetDBPathDlg);
        cancelButton->setObjectName(QString::fromUtf8("cancelButton"));
        cancelButton->setGeometry(QRect(340, 70, 75, 23));
        cancelButton->setAutoDefault(false);

        retranslateUi(SetDBPathDlg);

        OKButton->setDefault(true);


        QMetaObject::connectSlotsByName(SetDBPathDlg);
    } // setupUi

    void retranslateUi(QDialog *SetDBPathDlg)
    {
        SetDBPathDlg->setWindowTitle(QCoreApplication::translate("SetDBPathDlg", "DataBase Path Setting", nullptr));
        label->setText(QCoreApplication::translate("SetDBPathDlg", "DataBase Path:", nullptr));
        toolButton->setText(QCoreApplication::translate("SetDBPathDlg", "...", nullptr));
        OKButton->setText(QCoreApplication::translate("SetDBPathDlg", "OK", nullptr));
        cancelButton->setText(QCoreApplication::translate("SetDBPathDlg", "Cancel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SetDBPathDlg: public Ui_SetDBPathDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DBPATHSETTING_H
