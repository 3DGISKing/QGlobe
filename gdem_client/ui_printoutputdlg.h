/********************************************************************************
** Form generated from reading ui file 'printoutputdlg.ui'
**
** Created: Tue Sep 22 13:57:00 2009
**      by: Qt User Interface Compiler version 4.5.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_PRINTOUTPUTDLG_H
#define UI_PRINTOUTPUTDLG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>

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
        QFont font;
        font.setFamily(QString::fromUtf8("PRK P Gothic"));
        PrintOutputDlg->setFont(font);
        label = new QLabel(PrintOutputDlg);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(20, 20, 111, 16));
        lineEdit = new QLineEdit(PrintOutputDlg);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));
        lineEdit->setGeometry(QRect(20, 40, 251, 20));
        OKButton = new QPushButton(PrintOutputDlg);
        OKButton->setObjectName(QString::fromUtf8("OKButton"));
        OKButton->setGeometry(QRect(280, 10, 75, 23));
        cancelButton = new QPushButton(PrintOutputDlg);
        cancelButton->setObjectName(QString::fromUtf8("cancelButton"));
        cancelButton->setGeometry(QRect(280, 40, 75, 23));

        retranslateUi(PrintOutputDlg);

        QMetaObject::connectSlotsByName(PrintOutputDlg);
    } // setupUi

    void retranslateUi(QDialog *PrintOutputDlg)
    {
        PrintOutputDlg->setWindowTitle(QApplication::translate("PrintOutputDlg", "\355\214\214\354\235\274\353\241\234 \354\266\234\353\240\245", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("PrintOutputDlg", "\354\266\234\353\240\245\355\214\214\354\235\274\354\235\264\353\246\204:", 0, QApplication::UnicodeUTF8));
        OKButton->setText(QApplication::translate("PrintOutputDlg", "\355\231\225\354\235\270", 0, QApplication::UnicodeUTF8));
        cancelButton->setText(QApplication::translate("PrintOutputDlg", "\354\267\250\354\206\214", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(PrintOutputDlg);
    } // retranslateUi

};

namespace Ui {
    class PrintOutputDlg: public Ui_PrintOutputDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PRINTOUTPUTDLG_H
