/********************************************************************************
** Form generated from reading UI file 'directorydialog.ui'
**
** Created by: Qt User Interface Compiler version 5.15.18
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIRECTORYDIALOG_H
#define UI_DIRECTORYDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QTreeView>

QT_BEGIN_NAMESPACE

class Ui_DirectoryDialog
{
public:
    QDialogButtonBox *buttonBox;
    QTreeView *directoryView;
    QLabel *directorylabel;

    void setupUi(QDialog *DirectoryDialog)
    {
        if (DirectoryDialog->objectName().isEmpty())
            DirectoryDialog->setObjectName(QString::fromUtf8("DirectoryDialog"));
        DirectoryDialog->resize(402, 341);
        buttonBox = new QDialogButtonBox(DirectoryDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setGeometry(QRect(40, 300, 341, 32));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        directoryView = new QTreeView(DirectoryDialog);
        directoryView->setObjectName(QString::fromUtf8("directoryView"));
        directoryView->setGeometry(QRect(20, 40, 361, 251));
        directorylabel = new QLabel(DirectoryDialog);
        directorylabel->setObjectName(QString::fromUtf8("directorylabel"));
        directorylabel->setGeometry(QRect(20, 10, 71, 16));

        retranslateUi(DirectoryDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), DirectoryDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), DirectoryDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(DirectoryDialog);
    } // setupUi

    void retranslateUi(QDialog *DirectoryDialog)
    {
        DirectoryDialog->setWindowTitle(QCoreApplication::translate("DirectoryDialog", "Dir View", nullptr));
        directorylabel->setText(QCoreApplication::translate("DirectoryDialog", "Find Files", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DirectoryDialog: public Ui_DirectoryDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIRECTORYDIALOG_H
