/********************************************************************************
** Form generated from reading UI file 'printdlg.ui'
**
** Created by: Qt User Interface Compiler version 5.15.18
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PRINTDLG_H
#define UI_PRINTDLG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>

QT_BEGIN_NAMESPACE

class Ui_PrintDlg
{
public:
    QGroupBox *printerGroup;
    QComboBox *printerCombo;
    QLabel *label;
    QLabel *label_2;
    QLabel *printerStateLabel;
    QFrame *line;
    QCheckBox *printToFileCheck;
    QGroupBox *otherGroup;
    QLabel *label_5;
    QSpinBox *copyNumSpin;
    QCheckBox *colorCheck;
    QCheckBox *bothSideCheck;
    QFrame *line_2;
    QGroupBox *paperGroup;
    QLabel *label_6;
    QLabel *label_7;
    QComboBox *directionCombo;
    QComboBox *sizeCombo;
    QLabel *label_4;
    QLabel *label_8;
    QLineEdit *widthEdit;
    QLineEdit *heightEdit;
    QPushButton *OKButton;
    QPushButton *cancelButton;

    void setupUi(QDialog *PrintDlg)
    {
        if (PrintDlg->objectName().isEmpty())
            PrintDlg->setObjectName(QString::fromUtf8("PrintDlg"));
        PrintDlg->resize(462, 304);
        PrintDlg->setMinimumSize(QSize(462, 304));
        PrintDlg->setMaximumSize(QSize(462, 304));
        printerGroup = new QGroupBox(PrintDlg);
        printerGroup->setObjectName(QString::fromUtf8("printerGroup"));
        printerGroup->setGeometry(QRect(10, 10, 441, 111));
        printerGroup->setMinimumSize(QSize(441, 111));
        printerGroup->setMaximumSize(QSize(441, 111));
        printerCombo = new QComboBox(printerGroup);
        printerCombo->setObjectName(QString::fromUtf8("printerCombo"));
        printerCombo->setGeometry(QRect(100, 21, 321, 21));
        label = new QLabel(printerGroup);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(20, 23, 71, 16));
        label_2 = new QLabel(printerGroup);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(20, 48, 71, 20));
        printerStateLabel = new QLabel(printerGroup);
        printerStateLabel->setObjectName(QString::fromUtf8("printerStateLabel"));
        printerStateLabel->setGeometry(QRect(100, 48, 321, 20));
        line = new QFrame(printerGroup);
        line->setObjectName(QString::fromUtf8("line"));
        line->setGeometry(QRect(10, 64, 421, 16));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);
        printToFileCheck = new QCheckBox(printerGroup);
        printToFileCheck->setObjectName(QString::fromUtf8("printToFileCheck"));
        printToFileCheck->setGeometry(QRect(20, 80, 151, 18));
        otherGroup = new QGroupBox(PrintDlg);
        otherGroup->setObjectName(QString::fromUtf8("otherGroup"));
        otherGroup->setGeometry(QRect(270, 140, 181, 121));
        label_5 = new QLabel(otherGroup);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(20, 23, 61, 16));
        copyNumSpin = new QSpinBox(otherGroup);
        copyNumSpin->setObjectName(QString::fromUtf8("copyNumSpin"));
        copyNumSpin->setGeometry(QRect(80, 20, 81, 22));
        copyNumSpin->setMaximum(999);
        copyNumSpin->setValue(1);
        colorCheck = new QCheckBox(otherGroup);
        colorCheck->setObjectName(QString::fromUtf8("colorCheck"));
        colorCheck->setGeometry(QRect(20, 60, 141, 18));
        colorCheck->setChecked(true);
        bothSideCheck = new QCheckBox(otherGroup);
        bothSideCheck->setObjectName(QString::fromUtf8("bothSideCheck"));
        bothSideCheck->setGeometry(QRect(20, 90, 131, 18));
        line_2 = new QFrame(otherGroup);
        line_2->setObjectName(QString::fromUtf8("line_2"));
        line_2->setGeometry(QRect(10, 41, 161, 20));
        line_2->setFrameShape(QFrame::HLine);
        line_2->setFrameShadow(QFrame::Sunken);
        paperGroup = new QGroupBox(PrintDlg);
        paperGroup->setObjectName(QString::fromUtf8("paperGroup"));
        paperGroup->setGeometry(QRect(10, 140, 251, 121));
        label_6 = new QLabel(paperGroup);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(20, 30, 41, 16));
        label_7 = new QLabel(paperGroup);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setGeometry(QRect(20, 60, 41, 16));
        directionCombo = new QComboBox(paperGroup);
        directionCombo->addItem(QString());
        directionCombo->addItem(QString());
        directionCombo->setObjectName(QString::fromUtf8("directionCombo"));
        directionCombo->setGeometry(QRect(60, 27, 171, 21));
        sizeCombo = new QComboBox(paperGroup);
        sizeCombo->setObjectName(QString::fromUtf8("sizeCombo"));
        sizeCombo->setGeometry(QRect(60, 58, 81, 21));
        label_4 = new QLabel(paperGroup);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(150, 60, 31, 16));
        label_8 = new QLabel(paperGroup);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setGeometry(QRect(150, 89, 31, 20));
        widthEdit = new QLineEdit(paperGroup);
        widthEdit->setObjectName(QString::fromUtf8("widthEdit"));
        widthEdit->setGeometry(QRect(190, 58, 41, 20));
        widthEdit->setReadOnly(true);
        heightEdit = new QLineEdit(paperGroup);
        heightEdit->setObjectName(QString::fromUtf8("heightEdit"));
        heightEdit->setGeometry(QRect(190, 88, 41, 21));
        heightEdit->setReadOnly(true);
        OKButton = new QPushButton(PrintDlg);
        OKButton->setObjectName(QString::fromUtf8("OKButton"));
        OKButton->setGeometry(QRect(300, 270, 75, 23));
        OKButton->setAutoDefault(false);
        cancelButton = new QPushButton(PrintDlg);
        cancelButton->setObjectName(QString::fromUtf8("cancelButton"));
        cancelButton->setGeometry(QRect(380, 270, 75, 23));
        cancelButton->setAutoDefault(false);

        retranslateUi(PrintDlg);

        OKButton->setDefault(true);


        QMetaObject::connectSlotsByName(PrintDlg);
    } // setupUi

    void retranslateUi(QDialog *PrintDlg)
    {
        PrintDlg->setWindowTitle(QCoreApplication::translate("PrintDlg", "Print", nullptr));
        printerGroup->setTitle(QCoreApplication::translate("PrintDlg", "Printer", nullptr));
        label->setText(QCoreApplication::translate("PrintDlg", "Printer Name:", nullptr));
        label_2->setText(QCoreApplication::translate("PrintDlg", "Printer Status:", nullptr));
        printerStateLabel->setText(QString());
        printToFileCheck->setText(QCoreApplication::translate("PrintDlg", "Print To File", nullptr));
        otherGroup->setTitle(QCoreApplication::translate("PrintDlg", "Print Copy And Other", nullptr));
        label_5->setText(QCoreApplication::translate("PrintDlg", "Print Copy:", nullptr));
        colorCheck->setText(QCoreApplication::translate("PrintDlg", "Color Print", nullptr));
        bothSideCheck->setText(QCoreApplication::translate("PrintDlg", "Both Side Print", nullptr));
        paperGroup->setTitle(QCoreApplication::translate("PrintDlg", "Paper Formt", nullptr));
        label_6->setText(QCoreApplication::translate("PrintDlg", "Direction:", nullptr));
        label_7->setText(QCoreApplication::translate("PrintDlg", "Size:", nullptr));
        directionCombo->setItemText(0, QCoreApplication::translate("PrintDlg", "Horizon", nullptr));
        directionCombo->setItemText(1, QCoreApplication::translate("PrintDlg", "Vertical", nullptr));

        label_4->setText(QCoreApplication::translate("PrintDlg", "Width:", nullptr));
        label_8->setText(QCoreApplication::translate("PrintDlg", "pHeight:", nullptr));
        OKButton->setText(QCoreApplication::translate("PrintDlg", "Print", nullptr));
        cancelButton->setText(QCoreApplication::translate("PrintDlg", "Cancel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class PrintDlg: public Ui_PrintDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PRINTDLG_H
