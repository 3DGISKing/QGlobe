/********************************************************************************
** Form generated from reading UI file 'vectorconverter.ui'
**
** Created by: Qt User Interface Compiler version 5.15.18
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VECTORCONVERTER_H
#define UI_VECTORCONVERTER_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_VectorConverterClass
{
public:
    QWidget *centralWidget;
    QLineEdit *txtDirSrc;
    QLineEdit *txtDirSave;
    QPushButton *btnOK;
    QPushButton *btnNo;
    QLineEdit *txtStartLevel;
    QLineEdit *txtEndLevel;
    QLabel *label;
    QLabel *label_2;
    QPushButton *btnBrowseSrc;
    QPushButton *btnBrowseSave;
    QLabel *lblSource;
    QLabel *label_4;
    QCheckBox *chkIDWrite;
    QProgressBar *progressBar;
    QLabel *lblStatus;
    QLineEdit *txtToleranceNum;
    QLabel *label_5;
    QLineEdit *txtMaxFileSize;
    QLabel *label_6;
    QLabel *label_7;
    QLineEdit *txtMaxFeatureNum;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *VectorConverterClass)
    {
        if (VectorConverterClass->objectName().isEmpty())
            VectorConverterClass->setObjectName(QString::fromUtf8("VectorConverterClass"));
        VectorConverterClass->setWindowModality(Qt::ApplicationModal);
        VectorConverterClass->resize(341, 274);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(VectorConverterClass->sizePolicy().hasHeightForWidth());
        VectorConverterClass->setSizePolicy(sizePolicy);
        centralWidget = new QWidget(VectorConverterClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        txtDirSrc = new QLineEdit(centralWidget);
        txtDirSrc->setObjectName(QString::fromUtf8("txtDirSrc"));
        txtDirSrc->setGeometry(QRect(10, 30, 291, 31));
        txtDirSave = new QLineEdit(centralWidget);
        txtDirSave->setObjectName(QString::fromUtf8("txtDirSave"));
        txtDirSave->setGeometry(QRect(10, 80, 291, 31));
        btnOK = new QPushButton(centralWidget);
        btnOK->setObjectName(QString::fromUtf8("btnOK"));
        btnOK->setGeometry(QRect(254, 120, 75, 24));
        btnNo = new QPushButton(centralWidget);
        btnNo->setObjectName(QString::fromUtf8("btnNo"));
        btnNo->setGeometry(QRect(254, 150, 75, 24));
        txtStartLevel = new QLineEdit(centralWidget);
        txtStartLevel->setObjectName(QString::fromUtf8("txtStartLevel"));
        txtStartLevel->setGeometry(QRect(70, 114, 21, 20));
        txtEndLevel = new QLineEdit(centralWidget);
        txtEndLevel->setObjectName(QString::fromUtf8("txtEndLevel"));
        txtEndLevel->setGeometry(QRect(69, 136, 21, 20));
        label = new QLabel(centralWidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(14, 114, 51, 21));
        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(14, 136, 51, 21));
        btnBrowseSrc = new QPushButton(centralWidget);
        btnBrowseSrc->setObjectName(QString::fromUtf8("btnBrowseSrc"));
        btnBrowseSrc->setGeometry(QRect(310, 30, 21, 24));
        btnBrowseSave = new QPushButton(centralWidget);
        btnBrowseSave->setObjectName(QString::fromUtf8("btnBrowseSave"));
        btnBrowseSave->setGeometry(QRect(310, 90, 21, 24));
        lblSource = new QLabel(centralWidget);
        lblSource->setObjectName(QString::fromUtf8("lblSource"));
        lblSource->setGeometry(QRect(10, 4, 261, 20));
        label_4 = new QLabel(centralWidget);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(10, 60, 171, 21));
        chkIDWrite = new QCheckBox(centralWidget);
        chkIDWrite->setObjectName(QString::fromUtf8("chkIDWrite"));
        chkIDWrite->setGeometry(QRect(10, 160, 81, 19));
        chkIDWrite->setChecked(true);
        progressBar = new QProgressBar(centralWidget);
        progressBar->setObjectName(QString::fromUtf8("progressBar"));
        progressBar->setGeometry(QRect(0, 185, 331, 23));
        progressBar->setValue(24);
        lblStatus = new QLabel(centralWidget);
        lblStatus->setObjectName(QString::fromUtf8("lblStatus"));
        lblStatus->setGeometry(QRect(50, 215, 311, 16));
        txtToleranceNum = new QLineEdit(centralWidget);
        txtToleranceNum->setObjectName(QString::fromUtf8("txtToleranceNum"));
        txtToleranceNum->setGeometry(QRect(204, 162, 31, 21));
        label_5 = new QLabel(centralWidget);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(119, 162, 81, 21));
        txtMaxFileSize = new QLineEdit(centralWidget);
        txtMaxFileSize->setObjectName(QString::fromUtf8("txtMaxFileSize"));
        txtMaxFileSize->setGeometry(QRect(204, 139, 31, 21));
        label_6 = new QLabel(centralWidget);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(109, 139, 91, 21));
        label_7 = new QLabel(centralWidget);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setGeometry(QRect(109, 116, 91, 21));
        txtMaxFeatureNum = new QLineEdit(centralWidget);
        txtMaxFeatureNum->setObjectName(QString::fromUtf8("txtMaxFeatureNum"));
        txtMaxFeatureNum->setGeometry(QRect(204, 116, 31, 21));
        VectorConverterClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(VectorConverterClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 341, 19));
        VectorConverterClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(VectorConverterClass);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        VectorConverterClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(VectorConverterClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        VectorConverterClass->setStatusBar(statusBar);

        retranslateUi(VectorConverterClass);

        QMetaObject::connectSlotsByName(VectorConverterClass);
    } // setupUi

    void retranslateUi(QMainWindow *VectorConverterClass)
    {
        VectorConverterClass->setWindowTitle(QCoreApplication::translate("VectorConverterClass", "VectorConverter", nullptr));
        btnOK->setText(QCoreApplication::translate("VectorConverterClass", "Convert", nullptr));
        btnNo->setText(QCoreApplication::translate("VectorConverterClass", "Close", nullptr));
        txtStartLevel->setText(QCoreApplication::translate("VectorConverterClass", "5", nullptr));
        txtEndLevel->setText(QCoreApplication::translate("VectorConverterClass", "13", nullptr));
        label->setText(QCoreApplication::translate("VectorConverterClass", "Start Level", nullptr));
        label_2->setText(QCoreApplication::translate("VectorConverterClass", "End Level", nullptr));
        btnBrowseSrc->setText(QCoreApplication::translate("VectorConverterClass", "...", nullptr));
        btnBrowseSave->setText(QCoreApplication::translate("VectorConverterClass", "...", nullptr));
        lblSource->setText(QCoreApplication::translate("VectorConverterClass", "Select Source Shape File", nullptr));
        label_4->setText(QCoreApplication::translate("VectorConverterClass", "Select Output Folder", nullptr));
        chkIDWrite->setText(QCoreApplication::translate("VectorConverterClass", "HOPE Data?", nullptr));
        lblStatus->setText(QString());
        txtToleranceNum->setText(QCoreApplication::translate("VectorConverterClass", "3", nullptr));
        label_5->setText(QCoreApplication::translate("VectorConverterClass", "Tolerance(pix)", nullptr));
        txtMaxFileSize->setText(QCoreApplication::translate("VectorConverterClass", "300", nullptr));
        label_6->setText(QCoreApplication::translate("VectorConverterClass", "Max. File Size (KB)", nullptr));
        label_7->setText(QCoreApplication::translate("VectorConverterClass", "Max. Polygon Num", nullptr));
        txtMaxFeatureNum->setText(QCoreApplication::translate("VectorConverterClass", "1000", nullptr));
    } // retranslateUi

};

namespace Ui {
    class VectorConverterClass: public Ui_VectorConverterClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VECTORCONVERTER_H
