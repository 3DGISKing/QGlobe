/********************************************************************************
** Form generated from reading ui file 'printdlg.ui'
**
** Created: Tue Sep 22 13:57:01 2009
**      by: Qt User Interface Compiler version 4.5.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_PRINTDLG_H
#define UI_PRINTDLG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QFrame>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSpinBox>

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
        QFont font;
        font.setFamily(QString::fromUtf8("PRK P Gothic"));
        font.setStyleStrategy(QFont::NoAntialias);
        PrintDlg->setFont(font);
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
        cancelButton = new QPushButton(PrintDlg);
        cancelButton->setObjectName(QString::fromUtf8("cancelButton"));
        cancelButton->setGeometry(QRect(380, 270, 75, 23));

        retranslateUi(PrintDlg);

        QMetaObject::connectSlotsByName(PrintDlg);
    } // setupUi

    void retranslateUi(QDialog *PrintDlg)
    {
        PrintDlg->setWindowTitle(QApplication::translate("PrintDlg", "\354\235\270\354\207\204", 0, QApplication::UnicodeUTF8));
        printerGroup->setTitle(QApplication::translate("PrintDlg", "\354\235\270\354\207\204\352\270\260", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("PrintDlg", "\354\235\270\354\207\204\352\270\260\354\235\264\353\246\204:", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("PrintDlg", "\354\235\270\354\207\204\352\270\260\354\203\201\355\203\234:", 0, QApplication::UnicodeUTF8));
        printerStateLabel->setText(QString());
        printToFileCheck->setText(QApplication::translate("PrintDlg", "\355\214\214\354\235\274\353\241\234 \354\235\270\354\207\204", 0, QApplication::UnicodeUTF8));
        otherGroup->setTitle(QApplication::translate("PrintDlg", "\354\235\270\354\207\204\353\266\200\354\210\230 \353\260\217 \352\270\260\355\203\200", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("PrintDlg", "\354\235\270\354\207\204\353\266\200\354\210\230:", 0, QApplication::UnicodeUTF8));
        colorCheck->setText(QApplication::translate("PrintDlg", "\352\260\200\353\212\245\355\225\230\353\251\264 \354\203\211\354\235\270\354\207\204", 0, QApplication::UnicodeUTF8));
        bothSideCheck->setText(QApplication::translate("PrintDlg", "\353\237\211\353\251\264 \354\235\270\354\207\204", 0, QApplication::UnicodeUTF8));
        paperGroup->setTitle(QApplication::translate("PrintDlg", "\354\235\270\354\207\204\354\232\251\354\247\200\355\230\225\355\203\234", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("PrintDlg", "\353\260\251\355\226\245:", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("PrintDlg", "\355\201\254\352\270\260:", 0, QApplication::UnicodeUTF8));
        directionCombo->clear();
        directionCombo->insertItems(0, QStringList()
         << QApplication::translate("PrintDlg", "\352\260\200\353\241\234", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("PrintDlg", "\354\204\270\353\241\234", 0, QApplication::UnicodeUTF8)
        );
        label_4->setText(QApplication::translate("PrintDlg", "\353\204\210\353\271\204:", 0, QApplication::UnicodeUTF8));
        label_8->setText(QApplication::translate("PrintDlg", "\353\206\222\354\235\264:", 0, QApplication::UnicodeUTF8));
        OKButton->setText(QApplication::translate("PrintDlg", "\354\235\270\354\207\204", 0, QApplication::UnicodeUTF8));
        cancelButton->setText(QApplication::translate("PrintDlg", "\354\267\250\354\206\214", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(PrintDlg);
    } // retranslateUi

};

namespace Ui {
    class PrintDlg: public Ui_PrintDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PRINTDLG_H
