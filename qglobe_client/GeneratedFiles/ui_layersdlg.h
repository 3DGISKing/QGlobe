/********************************************************************************
** Form generated from reading UI file 'layersdlg.ui'
**
** Created by: Qt User Interface Compiler version 5.15.18
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LAYERSDLG_H
#define UI_LAYERSDLG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>

QT_BEGIN_NAMESPACE

class Ui_LayersDlg
{
public:

    void setupUi(QDialog *LayersDlg)
    {
        if (LayersDlg->objectName().isEmpty())
            LayersDlg->setObjectName(QString::fromUtf8("LayersDlg"));
        LayersDlg->resize(276, 214);

        retranslateUi(LayersDlg);

        QMetaObject::connectSlotsByName(LayersDlg);
    } // setupUi

    void retranslateUi(QDialog *LayersDlg)
    {
        LayersDlg->setWindowTitle(QCoreApplication::translate("LayersDlg", "Dialog", nullptr));
    } // retranslateUi

};

namespace Ui {
    class LayersDlg: public Ui_LayersDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LAYERSDLG_H
