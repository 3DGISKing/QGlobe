/********************************************************************************
** Form generated from reading UI file 'srvconfdlg.ui'
**
** Created by: Qt User Interface Compiler version 5.15.18
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SRVCONFDLG_H
#define UI_SRVCONFDLG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_SrvConfDlg
{
public:
    QPushButton *closeButton;
    QPushButton *okButton;
    QLabel *label;
    QLineEdit *ipEdit;
    QLabel *label_2;
    QLineEdit *portEdit;

    void setupUi(QDialog *SrvConfDlg)
    {
        if (SrvConfDlg->objectName().isEmpty())
            SrvConfDlg->setObjectName(QString::fromUtf8("SrvConfDlg"));
        SrvConfDlg->setWindowModality(Qt::WindowModal);
        SrvConfDlg->resize(343, 115);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(SrvConfDlg->sizePolicy().hasHeightForWidth());
        SrvConfDlg->setSizePolicy(sizePolicy);
        SrvConfDlg->setModal(true);
        closeButton = new QPushButton(SrvConfDlg);
        closeButton->setObjectName(QString::fromUtf8("closeButton"));
        closeButton->setGeometry(QRect(256, 81, 75, 24));
        closeButton->setAutoDefault(false);
        okButton = new QPushButton(SrvConfDlg);
        okButton->setObjectName(QString::fromUtf8("okButton"));
        okButton->setEnabled(false);
        okButton->setGeometry(QRect(175, 81, 75, 24));
        okButton->setAutoDefault(false);
        label = new QLabel(SrvConfDlg);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(11, 10, 60, 21));
        sizePolicy.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy);
        QFont font;
        font.setPointSize(10);
        label->setFont(font);
        ipEdit = new QLineEdit(SrvConfDlg);
        ipEdit->setObjectName(QString::fromUtf8("ipEdit"));
        ipEdit->setGeometry(QRect(80, 10, 250, 21));
        sizePolicy.setHeightForWidth(ipEdit->sizePolicy().hasHeightForWidth());
        ipEdit->setSizePolicy(sizePolicy);
        label_2 = new QLabel(SrvConfDlg);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(11, 40, 60, 21));
        sizePolicy.setHeightForWidth(label_2->sizePolicy().hasHeightForWidth());
        label_2->setSizePolicy(sizePolicy);
        portEdit = new QLineEdit(SrvConfDlg);
        portEdit->setObjectName(QString::fromUtf8("portEdit"));
        portEdit->setGeometry(QRect(80, 41, 100, 21));
        sizePolicy.setHeightForWidth(portEdit->sizePolicy().hasHeightForWidth());
        portEdit->setSizePolicy(sizePolicy);
        QWidget::setTabOrder(ipEdit, portEdit);
        QWidget::setTabOrder(portEdit, okButton);
        QWidget::setTabOrder(okButton, closeButton);

        retranslateUi(SrvConfDlg);

        okButton->setDefault(true);


        QMetaObject::connectSlotsByName(SrvConfDlg);
    } // setupUi

    void retranslateUi(QDialog *SrvConfDlg)
    {
        SrvConfDlg->setWindowTitle(QCoreApplication::translate("SrvConfDlg", "Server Setting", nullptr));
        closeButton->setText(QCoreApplication::translate("SrvConfDlg", "Cancel", nullptr));
        okButton->setText(QCoreApplication::translate("SrvConfDlg", "&Ok", nullptr));
        label->setText(QCoreApplication::translate("SrvConfDlg", "Address:", nullptr));
        ipEdit->setText(QString());
        label_2->setText(QCoreApplication::translate("SrvConfDlg", "Port:", nullptr));
        portEdit->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class SrvConfDlg: public Ui_SrvConfDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SRVCONFDLG_H
