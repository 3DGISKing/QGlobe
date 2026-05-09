/********************************************************************************
** Form generated from reading UI file 'calcprogressdlg.ui'
**
** Created by: Qt User Interface Compiler version 5.15.18
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CALCPROGRESSDLG_H
#define UI_CALCPROGRESSDLG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_CalcProgressDlg
{
public:
    QProgressBar *calcProgressBar;
    QLabel *calcLabel;
    QPushButton *stopButton;

    void setupUi(QDialog *CalcProgressDlg)
    {
        if (CalcProgressDlg->objectName().isEmpty())
            CalcProgressDlg->setObjectName(QString::fromUtf8("CalcProgressDlg"));
        CalcProgressDlg->setWindowModality(Qt::ApplicationModal);
        CalcProgressDlg->resize(400, 124);
        calcProgressBar = new QProgressBar(CalcProgressDlg);
        calcProgressBar->setObjectName(QString::fromUtf8("calcProgressBar"));
        calcProgressBar->setGeometry(QRect(10, 50, 380, 23));
        calcProgressBar->setValue(24);
        calcProgressBar->setAlignment(Qt::AlignCenter);
        calcProgressBar->setInvertedAppearance(false);
        calcProgressBar->setTextDirection(QProgressBar::TopToBottom);
        calcLabel = new QLabel(CalcProgressDlg);
        calcLabel->setObjectName(QString::fromUtf8("calcLabel"));
        calcLabel->setGeometry(QRect(10, 20, 120, 16));
        stopButton = new QPushButton(CalcProgressDlg);
        stopButton->setObjectName(QString::fromUtf8("stopButton"));
        stopButton->setGeometry(QRect(162, 90, 75, 24));
        stopButton->setAutoDefault(false);

        retranslateUi(CalcProgressDlg);

        stopButton->setDefault(true);


        QMetaObject::connectSlotsByName(CalcProgressDlg);
    } // setupUi

    void retranslateUi(QDialog *CalcProgressDlg)
    {
        CalcProgressDlg->setWindowTitle(QCoreApplication::translate("CalcProgressDlg", "Calculate", nullptr));
        calcProgressBar->setFormat(QCoreApplication::translate("CalcProgressDlg", "%p", nullptr));
        calcLabel->setText(QCoreApplication::translate("CalcProgressDlg", "Calculating", nullptr));
        stopButton->setText(QCoreApplication::translate("CalcProgressDlg", "Stop", nullptr));
    } // retranslateUi

};

namespace Ui {
    class CalcProgressDlg: public Ui_CalcProgressDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CALCPROGRESSDLG_H
