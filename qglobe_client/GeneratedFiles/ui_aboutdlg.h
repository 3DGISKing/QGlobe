/********************************************************************************
** Form generated from reading UI file 'aboutdlg.ui'
**
** Created by: Qt User Interface Compiler version 5.15.18
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ABOUTDLG_H
#define UI_ABOUTDLG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_AboutDlg
{
public:
    QPushButton *okButton;
    QGraphicsView *graphicsView;
    QLabel *label_3;
    QLabel *label_4;

    void setupUi(QDialog *AboutDlg)
    {
        if (AboutDlg->objectName().isEmpty())
            AboutDlg->setObjectName(QString::fromUtf8("AboutDlg"));
        AboutDlg->resize(440, 229);
        okButton = new QPushButton(AboutDlg);
        okButton->setObjectName(QString::fromUtf8("okButton"));
        okButton->setGeometry(QRect(182, 190, 75, 24));
        graphicsView = new QGraphicsView(AboutDlg);
        graphicsView->setObjectName(QString::fromUtf8("graphicsView"));
        graphicsView->setGeometry(QRect(0, 0, 440, 123));
        label_3 = new QLabel(AboutDlg);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(50, 135, 341, 21));
        label_3->setAlignment(Qt::AlignCenter);
        label_4 = new QLabel(AboutDlg);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(100, 155, 241, 21));
        label_4->setAlignment(Qt::AlignCenter);

        retranslateUi(AboutDlg);

        okButton->setDefault(true);


        QMetaObject::connectSlotsByName(AboutDlg);
    } // setupUi

    void retranslateUi(QDialog *AboutDlg)
    {
        AboutDlg->setWindowTitle(QCoreApplication::translate("AboutDlg", "About", nullptr));
        okButton->setText(QCoreApplication::translate("AboutDlg", "Ok", nullptr));
        label_3->setText(QCoreApplication::translate("AboutDlg", "Institute of Geoenvironment Information ,Academy of Science", nullptr));
        label_4->setText(QCoreApplication::translate("AboutDlg", "March 2013", nullptr));
    } // retranslateUi

};

namespace Ui {
    class AboutDlg: public Ui_AboutDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ABOUTDLG_H
