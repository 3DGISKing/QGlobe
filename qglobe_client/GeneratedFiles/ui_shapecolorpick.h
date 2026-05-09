/********************************************************************************
** Form generated from reading UI file 'shapecolorpick.ui'
**
** Created by: Qt User Interface Compiler version 5.15.18
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SHAPECOLORPICK_H
#define UI_SHAPECOLORPICK_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>

QT_BEGIN_NAMESPACE

class Ui_shapeColorPickClass
{
public:
    QComboBox *shpColorTypeCmb;
    QLabel *label;
    QPushButton *shpColorDialogBtn;
    QPushButton *shpColorSelectBtn;
    QLabel *label_2;
    QTextEdit *shpColorWidthTxt;

    void setupUi(QDialog *shapeColorPickClass)
    {
        if (shapeColorPickClass->objectName().isEmpty())
            shapeColorPickClass->setObjectName(QString::fromUtf8("shapeColorPickClass"));
        shapeColorPickClass->setWindowModality(Qt::ApplicationModal);
        shapeColorPickClass->resize(227, 155);
        shpColorTypeCmb = new QComboBox(shapeColorPickClass);
        shpColorTypeCmb->setObjectName(QString::fromUtf8("shpColorTypeCmb"));
        shpColorTypeCmb->setGeometry(QRect(20, 10, 181, 22));
        label = new QLabel(shapeColorPickClass);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(20, 40, 131, 21));
        shpColorDialogBtn = new QPushButton(shapeColorPickClass);
        shpColorDialogBtn->setObjectName(QString::fromUtf8("shpColorDialogBtn"));
        shpColorDialogBtn->setGeometry(QRect(170, 34, 31, 31));
        shpColorSelectBtn = new QPushButton(shapeColorPickClass);
        shpColorSelectBtn->setObjectName(QString::fromUtf8("shpColorSelectBtn"));
        shpColorSelectBtn->setGeometry(QRect(80, 110, 75, 24));
        label_2 = new QLabel(shapeColorPickClass);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(20, 76, 111, 21));
        shpColorWidthTxt = new QTextEdit(shapeColorPickClass);
        shpColorWidthTxt->setObjectName(QString::fromUtf8("shpColorWidthTxt"));
        shpColorWidthTxt->setGeometry(QRect(120, 70, 81, 27));
        QFont font;
        font.setPointSize(6);
        shpColorWidthTxt->setFont(font);

        retranslateUi(shapeColorPickClass);

        QMetaObject::connectSlotsByName(shapeColorPickClass);
    } // setupUi

    void retranslateUi(QDialog *shapeColorPickClass)
    {
        shapeColorPickClass->setWindowTitle(QCoreApplication::translate("shapeColorPickClass", "shapeColorPick", nullptr));
        label->setText(QCoreApplication::translate("shapeColorPickClass", "Choose a color!", nullptr));
        shpColorDialogBtn->setText(QString());
        shpColorSelectBtn->setText(QCoreApplication::translate("shapeColorPickClass", "PushButton", nullptr));
        label_2->setText(QCoreApplication::translate("shapeColorPickClass", "Enter line width!", nullptr));
    } // retranslateUi

};

namespace Ui {
    class shapeColorPickClass: public Ui_shapeColorPickClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SHAPECOLORPICK_H
