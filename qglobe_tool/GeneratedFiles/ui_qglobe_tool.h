/********************************************************************************
** Form generated from reading UI file 'qglobe_tool.ui'
**
** Created by: Qt User Interface Compiler version 5.15.18
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QGLOBE_TOOL_H
#define UI_QGLOBE_TOOL_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_qglobe_toolClass
{
public:
    QAction *actionShowGDemExtractDlg;
    QAction *actionShowSimpleBuildingProDlg;
    QAction *actionHopeDBToGDem;
    QAction *actionWriteRemoveJgw;
    QAction *actionLatLongToImage;
    QAction *actionImageMerge;
    QAction *actionShowBrowser;
    QAction *actionSqlBrowser;
    QAction *actionTileLatLong;
    QAction *actionGeoShape;
    QAction *actionKMLMaker;
    QAction *actionKeyGenerator;
    QAction *actionMergeJpgFiles;
    QAction *actionExit;
    QWidget *centralWidget;
    QMenuBar *menuBar;
    QMenu *menuDatabase;
    QMenu *menuSqlBrowser;
    QMenu *menuUtility;
    QMenu *menuKML;
    QMenu *menuFile;
    QToolBar *toolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *qglobe_toolClass)
    {
        if (qglobe_toolClass->objectName().isEmpty())
            qglobe_toolClass->setObjectName(QString::fromUtf8("qglobe_toolClass"));
        qglobe_toolClass->resize(598, 66);
        actionShowGDemExtractDlg = new QAction(qglobe_toolClass);
        actionShowGDemExtractDlg->setObjectName(QString::fromUtf8("actionShowGDemExtractDlg"));
        actionShowSimpleBuildingProDlg = new QAction(qglobe_toolClass);
        actionShowSimpleBuildingProDlg->setObjectName(QString::fromUtf8("actionShowSimpleBuildingProDlg"));
        actionHopeDBToGDem = new QAction(qglobe_toolClass);
        actionHopeDBToGDem->setObjectName(QString::fromUtf8("actionHopeDBToGDem"));
        actionWriteRemoveJgw = new QAction(qglobe_toolClass);
        actionWriteRemoveJgw->setObjectName(QString::fromUtf8("actionWriteRemoveJgw"));
        actionLatLongToImage = new QAction(qglobe_toolClass);
        actionLatLongToImage->setObjectName(QString::fromUtf8("actionLatLongToImage"));
        actionImageMerge = new QAction(qglobe_toolClass);
        actionImageMerge->setObjectName(QString::fromUtf8("actionImageMerge"));
        actionShowBrowser = new QAction(qglobe_toolClass);
        actionShowBrowser->setObjectName(QString::fromUtf8("actionShowBrowser"));
        actionSqlBrowser = new QAction(qglobe_toolClass);
        actionSqlBrowser->setObjectName(QString::fromUtf8("actionSqlBrowser"));
        actionTileLatLong = new QAction(qglobe_toolClass);
        actionTileLatLong->setObjectName(QString::fromUtf8("actionTileLatLong"));
        actionGeoShape = new QAction(qglobe_toolClass);
        actionGeoShape->setObjectName(QString::fromUtf8("actionGeoShape"));
        actionKMLMaker = new QAction(qglobe_toolClass);
        actionKMLMaker->setObjectName(QString::fromUtf8("actionKMLMaker"));
        actionKeyGenerator = new QAction(qglobe_toolClass);
        actionKeyGenerator->setObjectName(QString::fromUtf8("actionKeyGenerator"));
        actionMergeJpgFiles = new QAction(qglobe_toolClass);
        actionMergeJpgFiles->setObjectName(QString::fromUtf8("actionMergeJpgFiles"));
        actionExit = new QAction(qglobe_toolClass);
        actionExit->setObjectName(QString::fromUtf8("actionExit"));
        centralWidget = new QWidget(qglobe_toolClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(centralWidget->sizePolicy().hasHeightForWidth());
        centralWidget->setSizePolicy(sizePolicy);
        centralWidget->setMaximumSize(QSize(16777215, 32));
        qglobe_toolClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(qglobe_toolClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 598, 18));
        menuDatabase = new QMenu(menuBar);
        menuDatabase->setObjectName(QString::fromUtf8("menuDatabase"));
        menuSqlBrowser = new QMenu(menuBar);
        menuSqlBrowser->setObjectName(QString::fromUtf8("menuSqlBrowser"));
        menuUtility = new QMenu(menuBar);
        menuUtility->setObjectName(QString::fromUtf8("menuUtility"));
        menuKML = new QMenu(menuBar);
        menuKML->setObjectName(QString::fromUtf8("menuKML"));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        qglobe_toolClass->setMenuBar(menuBar);
        toolBar = new QToolBar(qglobe_toolClass);
        toolBar->setObjectName(QString::fromUtf8("toolBar"));
        qglobe_toolClass->addToolBar(Qt::TopToolBarArea, toolBar);
        statusBar = new QStatusBar(qglobe_toolClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        qglobe_toolClass->setStatusBar(statusBar);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuDatabase->menuAction());
        menuBar->addAction(menuSqlBrowser->menuAction());
        menuBar->addAction(menuKML->menuAction());
        menuBar->addAction(menuUtility->menuAction());
        menuDatabase->addAction(actionShowGDemExtractDlg);
        menuDatabase->addAction(actionHopeDBToGDem);
        menuDatabase->addAction(actionWriteRemoveJgw);
        menuDatabase->addAction(actionShowSimpleBuildingProDlg);
        menuDatabase->addAction(actionLatLongToImage);
        menuDatabase->addAction(actionImageMerge);
        menuDatabase->addAction(actionGeoShape);
        menuSqlBrowser->addAction(actionSqlBrowser);
        menuUtility->addAction(actionTileLatLong);
        menuUtility->addAction(actionKeyGenerator);
        menuUtility->addAction(actionMergeJpgFiles);
        menuKML->addAction(actionKMLMaker);
        menuFile->addAction(actionExit);

        retranslateUi(qglobe_toolClass);

        QMetaObject::connectSlotsByName(qglobe_toolClass);
    } // setupUi

    void retranslateUi(QMainWindow *qglobe_toolClass)
    {
        qglobe_toolClass->setWindowTitle(QCoreApplication::translate("qglobe_toolClass", "qglobe_tool", nullptr));
        actionShowGDemExtractDlg->setText(QCoreApplication::translate("qglobe_toolClass", "Extract", nullptr));
        actionShowSimpleBuildingProDlg->setText(QCoreApplication::translate("qglobe_toolClass", "Generate Simple Building ", nullptr));
        actionHopeDBToGDem->setText(QCoreApplication::translate("qglobe_toolClass", "Make Gt File", nullptr));
        actionWriteRemoveJgw->setText(QCoreApplication::translate("qglobe_toolClass", "Write Or Remove jgw files", nullptr));
        actionLatLongToImage->setText(QCoreApplication::translate("qglobe_toolClass", "Tiling Image", nullptr));
        actionImageMerge->setText(QCoreApplication::translate("qglobe_toolClass", "Merge Images", nullptr));
        actionShowBrowser->setText(QCoreApplication::translate("qglobe_toolClass", "ShowBrowser", nullptr));
        actionSqlBrowser->setText(QCoreApplication::translate("qglobe_toolClass", "SqlBrowser", nullptr));
        actionTileLatLong->setText(QCoreApplication::translate("qglobe_toolClass", "TileLatLong", nullptr));
        actionGeoShape->setText(QCoreApplication::translate("qglobe_toolClass", "Make GeoShape", nullptr));
        actionKMLMaker->setText(QCoreApplication::translate("qglobe_toolClass", "KMLMaker", nullptr));
        actionKeyGenerator->setText(QCoreApplication::translate("qglobe_toolClass", "KeyGenerator", nullptr));
        actionMergeJpgFiles->setText(QCoreApplication::translate("qglobe_toolClass", "Merge Jpg Files", nullptr));
        actionExit->setText(QCoreApplication::translate("qglobe_toolClass", "Exit", nullptr));
        menuDatabase->setTitle(QCoreApplication::translate("qglobe_toolClass", "Database", nullptr));
        menuSqlBrowser->setTitle(QCoreApplication::translate("qglobe_toolClass", "SqlBrowser", nullptr));
        menuUtility->setTitle(QCoreApplication::translate("qglobe_toolClass", "Utility", nullptr));
        menuKML->setTitle(QCoreApplication::translate("qglobe_toolClass", "KML", nullptr));
        menuFile->setTitle(QCoreApplication::translate("qglobe_toolClass", "File", nullptr));
        toolBar->setWindowTitle(QCoreApplication::translate("qglobe_toolClass", "toolBar", nullptr));
    } // retranslateUi

};

namespace Ui {
    class qglobe_toolClass: public Ui_qglobe_toolClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QGLOBE_TOOL_H
