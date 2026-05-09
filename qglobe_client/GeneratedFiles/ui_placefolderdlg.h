/********************************************************************************
** Form generated from reading UI file 'placefolderdlg.ui'
**
** Created by: Qt User Interface Compiler version 5.15.18
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PLACEFOLDERDLG_H
#define UI_PLACEFOLDERDLG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_PlaceFolderDlg
{
public:
    QTabWidget *propertyTab;
    QWidget *tabDescription;
    QPlainTextEdit *textDescription;
    QLabel *label_10;
    QWidget *tabView;
    QWidget *layoutWidget;
    QGridLayout *gridLayout_2;
    QLabel *label_5;
    QLineEdit *lineViewLatitude;
    QLabel *label_4;
    QLineEdit *lineViewLongitude;
    QLabel *label_7;
    QLineEdit *lineViewRange;
    QLabel *label_6;
    QLineEdit *lineViewHeading;
    QLabel *label_9;
    QLineEdit *lineViewTilt;
    QPushButton *snapshotViewButton;
    QPushButton *resetButton;
    QLineEdit *lineName;
    QLabel *label;
    QPushButton *okButton;
    QPushButton *cancelButton;

    void setupUi(QDialog *PlaceFolderDlg)
    {
        if (PlaceFolderDlg->objectName().isEmpty())
            PlaceFolderDlg->setObjectName(QString::fromUtf8("PlaceFolderDlg"));
        PlaceFolderDlg->resize(450, 395);
        PlaceFolderDlg->setMinimumSize(QSize(450, 395));
        PlaceFolderDlg->setMaximumSize(QSize(450, 395));
        propertyTab = new QTabWidget(PlaceFolderDlg);
        propertyTab->setObjectName(QString::fromUtf8("propertyTab"));
        propertyTab->setGeometry(QRect(10, 80, 430, 271));
        tabDescription = new QWidget();
        tabDescription->setObjectName(QString::fromUtf8("tabDescription"));
        textDescription = new QPlainTextEdit(tabDescription);
        textDescription->setObjectName(QString::fromUtf8("textDescription"));
        textDescription->setGeometry(QRect(10, 30, 405, 215));
        label_10 = new QLabel(tabDescription);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->setGeometry(QRect(10, 10, 100, 22));
        propertyTab->addTab(tabDescription, QString());
        tabView = new QWidget();
        tabView->setObjectName(QString::fromUtf8("tabView"));
        layoutWidget = new QWidget(tabView);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(60, 30, 301, 136));
        gridLayout_2 = new QGridLayout(layoutWidget);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        gridLayout_2->setContentsMargins(0, 0, 0, 0);
        label_5 = new QLabel(layoutWidget);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        gridLayout_2->addWidget(label_5, 0, 0, 1, 1);

        lineViewLatitude = new QLineEdit(layoutWidget);
        lineViewLatitude->setObjectName(QString::fromUtf8("lineViewLatitude"));

        gridLayout_2->addWidget(lineViewLatitude, 0, 1, 1, 1);

        label_4 = new QLabel(layoutWidget);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout_2->addWidget(label_4, 1, 0, 1, 1);

        lineViewLongitude = new QLineEdit(layoutWidget);
        lineViewLongitude->setObjectName(QString::fromUtf8("lineViewLongitude"));

        gridLayout_2->addWidget(lineViewLongitude, 1, 1, 1, 1);

        label_7 = new QLabel(layoutWidget);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        gridLayout_2->addWidget(label_7, 2, 0, 1, 1);

        lineViewRange = new QLineEdit(layoutWidget);
        lineViewRange->setObjectName(QString::fromUtf8("lineViewRange"));

        gridLayout_2->addWidget(lineViewRange, 2, 1, 1, 1);

        label_6 = new QLabel(layoutWidget);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        gridLayout_2->addWidget(label_6, 3, 0, 1, 1);

        lineViewHeading = new QLineEdit(layoutWidget);
        lineViewHeading->setObjectName(QString::fromUtf8("lineViewHeading"));

        gridLayout_2->addWidget(lineViewHeading, 3, 1, 1, 1);

        label_9 = new QLabel(layoutWidget);
        label_9->setObjectName(QString::fromUtf8("label_9"));

        gridLayout_2->addWidget(label_9, 4, 0, 1, 1);

        lineViewTilt = new QLineEdit(layoutWidget);
        lineViewTilt->setObjectName(QString::fromUtf8("lineViewTilt"));

        gridLayout_2->addWidget(lineViewTilt, 4, 1, 1, 1);

        snapshotViewButton = new QPushButton(tabView);
        snapshotViewButton->setObjectName(QString::fromUtf8("snapshotViewButton"));
        snapshotViewButton->setGeometry(QRect(180, 210, 150, 24));
        snapshotViewButton->setFocusPolicy(Qt::StrongFocus);
        snapshotViewButton->setAutoDefault(false);
        resetButton = new QPushButton(tabView);
        resetButton->setObjectName(QString::fromUtf8("resetButton"));
        resetButton->setGeometry(QRect(340, 210, 75, 24));
        resetButton->setFocusPolicy(Qt::StrongFocus);
        resetButton->setAutoDefault(false);
        propertyTab->addTab(tabView, QString());
        lineName = new QLineEdit(PlaceFolderDlg);
        lineName->setObjectName(QString::fromUtf8("lineName"));
        lineName->setGeometry(QRect(70, 20, 370, 22));
        label = new QLabel(PlaceFolderDlg);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(10, 20, 45, 24));
        okButton = new QPushButton(PlaceFolderDlg);
        okButton->setObjectName(QString::fromUtf8("okButton"));
        okButton->setGeometry(QRect(270, 365, 75, 24));
        okButton->setFocusPolicy(Qt::StrongFocus);
        okButton->setAutoDefault(false);
        cancelButton = new QPushButton(PlaceFolderDlg);
        cancelButton->setObjectName(QString::fromUtf8("cancelButton"));
        cancelButton->setGeometry(QRect(365, 365, 75, 24));
        cancelButton->setFocusPolicy(Qt::StrongFocus);
        cancelButton->setAutoDefault(false);
        QWidget::setTabOrder(lineName, propertyTab);
        QWidget::setTabOrder(propertyTab, textDescription);
        QWidget::setTabOrder(textDescription, lineViewLatitude);
        QWidget::setTabOrder(lineViewLatitude, lineViewLongitude);
        QWidget::setTabOrder(lineViewLongitude, lineViewRange);
        QWidget::setTabOrder(lineViewRange, lineViewHeading);
        QWidget::setTabOrder(lineViewHeading, lineViewTilt);
        QWidget::setTabOrder(lineViewTilt, snapshotViewButton);
        QWidget::setTabOrder(snapshotViewButton, resetButton);
        QWidget::setTabOrder(resetButton, okButton);
        QWidget::setTabOrder(okButton, cancelButton);

        retranslateUi(PlaceFolderDlg);

        propertyTab->setCurrentIndex(1);
        okButton->setDefault(true);


        QMetaObject::connectSlotsByName(PlaceFolderDlg);
    } // setupUi

    void retranslateUi(QDialog *PlaceFolderDlg)
    {
        PlaceFolderDlg->setWindowTitle(QCoreApplication::translate("PlaceFolderDlg", "New Folder", nullptr));
        label_10->setText(QCoreApplication::translate("PlaceFolderDlg", "Description:", nullptr));
        propertyTab->setTabText(propertyTab->indexOf(tabDescription), QCoreApplication::translate("PlaceFolderDlg", "Description", nullptr));
        label_5->setText(QCoreApplication::translate("PlaceFolderDlg", "Latitude:", nullptr));
        label_4->setText(QCoreApplication::translate("PlaceFolderDlg", "Longitude:", nullptr));
        label_7->setText(QCoreApplication::translate("PlaceFolderDlg", "Range:", nullptr));
        label_6->setText(QCoreApplication::translate("PlaceFolderDlg", "Heading:", nullptr));
        label_9->setText(QCoreApplication::translate("PlaceFolderDlg", "Tilt:", nullptr));
        snapshotViewButton->setText(QCoreApplication::translate("PlaceFolderDlg", "Snapshot current view", nullptr));
        resetButton->setText(QCoreApplication::translate("PlaceFolderDlg", "Reset", nullptr));
        propertyTab->setTabText(propertyTab->indexOf(tabView), QCoreApplication::translate("PlaceFolderDlg", "View", nullptr));
        label->setText(QCoreApplication::translate("PlaceFolderDlg", "Name: ", nullptr));
        okButton->setText(QCoreApplication::translate("PlaceFolderDlg", "Ok", nullptr));
        cancelButton->setText(QCoreApplication::translate("PlaceFolderDlg", "Cancel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class PlaceFolderDlg: public Ui_PlaceFolderDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PLACEFOLDERDLG_H
