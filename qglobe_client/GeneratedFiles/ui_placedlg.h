/********************************************************************************
** Form generated from reading UI file 'placedlg.ui'
**
** Created by: Qt User Interface Compiler version 5.15.18
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PLACEDLG_H
#define UI_PLACEDLG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>

QT_BEGIN_NAMESPACE

class Ui_PlaceDlg
{
public:

    void setupUi(QDialog *PlaceDlg)
    {
        if (PlaceDlg->objectName().isEmpty())
            PlaceDlg->setObjectName(QString::fromUtf8("PlaceDlg"));
        PlaceDlg->resize(280, 250);

        retranslateUi(PlaceDlg);

        QMetaObject::connectSlotsByName(PlaceDlg);
    } // setupUi

    void retranslateUi(QDialog *PlaceDlg)
    {
        PlaceDlg->setWindowTitle(QCoreApplication::translate("PlaceDlg", "Dialog", nullptr));
    } // retranslateUi

};

namespace Ui {
    class PlaceDlg: public Ui_PlaceDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PLACEDLG_H
