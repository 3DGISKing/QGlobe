/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.18
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionPrint;
    QAction *actionExit;
    QAction *actionCut;
    QAction *actionCopy;
    QAction *actionImageCopy;
    QAction *actionPaste;
    QAction *actionDelete;
    QAction *actionContentDel;
    QAction *actionFindHistoryDel;
    QAction *actionProperty;
    QAction *actionSidebar;
    QAction *actionFullScreen;
    QAction *actionStatusbar;
    QAction *actionGrid;
    QAction *actionScaleLegend;
    QAction *actionSaveStartPos1;
    QAction *actionVGA;
    QAction *actionSVGA;
    QAction *actionNTSC;
    QAction *actionPAL;
    QAction *actionWideNTSC;
    QAction *actionWidePAL;
    QAction *action1080pHDTV;
    QAction *action720pHDTV;
    QAction *action480pDVD;
    QAction *actionLetter;
    QAction *actionTabloid;
    QAction *actionPostCard1;
    QAction *actionPostCard2;
    QAction *actionNavAuto;
    QAction *actionNavAlways;
    QAction *actionNavDial;
    QAction *actionNavNever;
    QAction *actionOption;
    QAction *actionAddFolder;
    QAction *actionAddPlaceMark;
    QAction *actionHelpManual;
    QAction *actionAbout;
    QAction *actionRuler1;
    QAction *actionOpenFile;
    QAction *actionSaveAsPlaceMarkFile;
    QAction *actionMoveToMyPlaceq;
    QAction *actionSaveMyPlace;
    QAction *actionSaveImage;
    QAction *actionContourSetting;
    QAction *actionTerrain;
    QAction *actionContour;
    QAction *actionServer;
    QAction *actionAddPath;
    QAction *actionAddPolygon;
    QAction *actionFlightSimulator1;
    QAction *actionCarSimulator1;
    QAction *actionTourPlay1;
    QAction *actionRenderTexture;
    QAction *actionRenderAltitude;
    QAction *actionRenderGradient;
    QAction *actionRenderIncline;
    QAction *actionAdd3DObject;
    QAction *actionAddPeople;
    QAction *actionBuildingFind;
    QAction *actionPeopleFind;
    QAction *actionGeoshapeFormat;
    QAction *actionLicence;
    QAction *actionBuildingName;
    QAction *actionFlightSimulator;
    QAction *actionCarSimulator;
    QAction *actionTourPlay;
    QAction *actionRuler;
    QAction *actionSaveStartPos;
    QAction *actionShowStandardToolBar;
    QAction *actionShowSimulationToolBar;
    QAction *actionMoveToMyPlace;
    QAction *actionShowOverviewMap;
    QAction *actionAnimation;
    QAction *actionMeasureArea;
    QAction *actionObjectPlaceEdit;
    QAction *actionObjectSetting;
    QAction *actionGoTo;
    QWidget *centralWidget;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuEdit;
    QMenu *menuView;
    QMenu *menuViewSize;
    QMenu *menuComputer;
    QMenu *menuTV;
    QMenu *menuPrintOutput;
    QMenu *menuNavigation;
    QMenu *menuToolbar;
    QMenu *menuSetting;
    QMenu *menuHelp;
    QMenu *menuLayer;
    QMenu *menuRender;
    QMenu *menuAdd;
    QMenu *menuFind;
    QMenu *menuSimulation;
    QMenu *menuTool;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(600, 400);
        actionPrint = new QAction(MainWindow);
        actionPrint->setObjectName(QString::fromUtf8("actionPrint"));
        actionExit = new QAction(MainWindow);
        actionExit->setObjectName(QString::fromUtf8("actionExit"));
        actionCut = new QAction(MainWindow);
        actionCut->setObjectName(QString::fromUtf8("actionCut"));
        actionCopy = new QAction(MainWindow);
        actionCopy->setObjectName(QString::fromUtf8("actionCopy"));
        actionImageCopy = new QAction(MainWindow);
        actionImageCopy->setObjectName(QString::fromUtf8("actionImageCopy"));
        actionPaste = new QAction(MainWindow);
        actionPaste->setObjectName(QString::fromUtf8("actionPaste"));
        actionDelete = new QAction(MainWindow);
        actionDelete->setObjectName(QString::fromUtf8("actionDelete"));
        actionContentDel = new QAction(MainWindow);
        actionContentDel->setObjectName(QString::fromUtf8("actionContentDel"));
        actionFindHistoryDel = new QAction(MainWindow);
        actionFindHistoryDel->setObjectName(QString::fromUtf8("actionFindHistoryDel"));
        actionProperty = new QAction(MainWindow);
        actionProperty->setObjectName(QString::fromUtf8("actionProperty"));
        actionSidebar = new QAction(MainWindow);
        actionSidebar->setObjectName(QString::fromUtf8("actionSidebar"));
        actionSidebar->setCheckable(true);
        actionSidebar->setChecked(true);
        actionFullScreen = new QAction(MainWindow);
        actionFullScreen->setObjectName(QString::fromUtf8("actionFullScreen"));
        actionFullScreen->setCheckable(true);
        actionStatusbar = new QAction(MainWindow);
        actionStatusbar->setObjectName(QString::fromUtf8("actionStatusbar"));
        actionStatusbar->setCheckable(true);
        actionStatusbar->setChecked(true);
        actionGrid = new QAction(MainWindow);
        actionGrid->setObjectName(QString::fromUtf8("actionGrid"));
        actionGrid->setCheckable(true);
        actionGrid->setChecked(false);
        actionScaleLegend = new QAction(MainWindow);
        actionScaleLegend->setObjectName(QString::fromUtf8("actionScaleLegend"));
        actionScaleLegend->setCheckable(true);
        actionSaveStartPos1 = new QAction(MainWindow);
        actionSaveStartPos1->setObjectName(QString::fromUtf8("actionSaveStartPos1"));
        actionVGA = new QAction(MainWindow);
        actionVGA->setObjectName(QString::fromUtf8("actionVGA"));
        actionSVGA = new QAction(MainWindow);
        actionSVGA->setObjectName(QString::fromUtf8("actionSVGA"));
        actionNTSC = new QAction(MainWindow);
        actionNTSC->setObjectName(QString::fromUtf8("actionNTSC"));
        actionPAL = new QAction(MainWindow);
        actionPAL->setObjectName(QString::fromUtf8("actionPAL"));
        actionWideNTSC = new QAction(MainWindow);
        actionWideNTSC->setObjectName(QString::fromUtf8("actionWideNTSC"));
        actionWidePAL = new QAction(MainWindow);
        actionWidePAL->setObjectName(QString::fromUtf8("actionWidePAL"));
        action1080pHDTV = new QAction(MainWindow);
        action1080pHDTV->setObjectName(QString::fromUtf8("action1080pHDTV"));
        action720pHDTV = new QAction(MainWindow);
        action720pHDTV->setObjectName(QString::fromUtf8("action720pHDTV"));
        action480pDVD = new QAction(MainWindow);
        action480pDVD->setObjectName(QString::fromUtf8("action480pDVD"));
        actionLetter = new QAction(MainWindow);
        actionLetter->setObjectName(QString::fromUtf8("actionLetter"));
        actionTabloid = new QAction(MainWindow);
        actionTabloid->setObjectName(QString::fromUtf8("actionTabloid"));
        actionPostCard1 = new QAction(MainWindow);
        actionPostCard1->setObjectName(QString::fromUtf8("actionPostCard1"));
        actionPostCard2 = new QAction(MainWindow);
        actionPostCard2->setObjectName(QString::fromUtf8("actionPostCard2"));
        actionNavAuto = new QAction(MainWindow);
        actionNavAuto->setObjectName(QString::fromUtf8("actionNavAuto"));
        actionNavAuto->setCheckable(true);
        actionNavAlways = new QAction(MainWindow);
        actionNavAlways->setObjectName(QString::fromUtf8("actionNavAlways"));
        actionNavAlways->setCheckable(true);
        actionNavDial = new QAction(MainWindow);
        actionNavDial->setObjectName(QString::fromUtf8("actionNavDial"));
        actionNavDial->setCheckable(true);
        actionNavNever = new QAction(MainWindow);
        actionNavNever->setObjectName(QString::fromUtf8("actionNavNever"));
        actionNavNever->setCheckable(true);
        actionNavNever->setChecked(true);
        actionOption = new QAction(MainWindow);
        actionOption->setObjectName(QString::fromUtf8("actionOption"));
        actionAddFolder = new QAction(MainWindow);
        actionAddFolder->setObjectName(QString::fromUtf8("actionAddFolder"));
        actionAddPlaceMark = new QAction(MainWindow);
        actionAddPlaceMark->setObjectName(QString::fromUtf8("actionAddPlaceMark"));
        actionHelpManual = new QAction(MainWindow);
        actionHelpManual->setObjectName(QString::fromUtf8("actionHelpManual"));
        actionAbout = new QAction(MainWindow);
        actionAbout->setObjectName(QString::fromUtf8("actionAbout"));
        actionRuler1 = new QAction(MainWindow);
        actionRuler1->setObjectName(QString::fromUtf8("actionRuler1"));
        actionRuler1->setCheckable(true);
        actionOpenFile = new QAction(MainWindow);
        actionOpenFile->setObjectName(QString::fromUtf8("actionOpenFile"));
        actionSaveAsPlaceMarkFile = new QAction(MainWindow);
        actionSaveAsPlaceMarkFile->setObjectName(QString::fromUtf8("actionSaveAsPlaceMarkFile"));
        actionMoveToMyPlaceq = new QAction(MainWindow);
        actionMoveToMyPlaceq->setObjectName(QString::fromUtf8("actionMoveToMyPlaceq"));
        actionSaveMyPlace = new QAction(MainWindow);
        actionSaveMyPlace->setObjectName(QString::fromUtf8("actionSaveMyPlace"));
        actionSaveImage = new QAction(MainWindow);
        actionSaveImage->setObjectName(QString::fromUtf8("actionSaveImage"));
        actionContourSetting = new QAction(MainWindow);
        actionContourSetting->setObjectName(QString::fromUtf8("actionContourSetting"));
        actionTerrain = new QAction(MainWindow);
        actionTerrain->setObjectName(QString::fromUtf8("actionTerrain"));
        actionTerrain->setCheckable(true);
        actionTerrain->setChecked(true);
        actionContour = new QAction(MainWindow);
        actionContour->setObjectName(QString::fromUtf8("actionContour"));
        actionContour->setCheckable(true);
        actionContour->setChecked(true);
        actionServer = new QAction(MainWindow);
        actionServer->setObjectName(QString::fromUtf8("actionServer"));
        actionAddPath = new QAction(MainWindow);
        actionAddPath->setObjectName(QString::fromUtf8("actionAddPath"));
        actionAddPolygon = new QAction(MainWindow);
        actionAddPolygon->setObjectName(QString::fromUtf8("actionAddPolygon"));
        actionFlightSimulator1 = new QAction(MainWindow);
        actionFlightSimulator1->setObjectName(QString::fromUtf8("actionFlightSimulator1"));
        actionFlightSimulator1->setCheckable(true);
        actionCarSimulator1 = new QAction(MainWindow);
        actionCarSimulator1->setObjectName(QString::fromUtf8("actionCarSimulator1"));
        actionCarSimulator1->setCheckable(true);
        actionTourPlay1 = new QAction(MainWindow);
        actionTourPlay1->setObjectName(QString::fromUtf8("actionTourPlay1"));
        actionTourPlay1->setCheckable(true);
        actionRenderTexture = new QAction(MainWindow);
        actionRenderTexture->setObjectName(QString::fromUtf8("actionRenderTexture"));
        actionRenderTexture->setCheckable(true);
        actionRenderTexture->setChecked(true);
        actionRenderAltitude = new QAction(MainWindow);
        actionRenderAltitude->setObjectName(QString::fromUtf8("actionRenderAltitude"));
        actionRenderAltitude->setCheckable(true);
        actionRenderGradient = new QAction(MainWindow);
        actionRenderGradient->setObjectName(QString::fromUtf8("actionRenderGradient"));
        actionRenderGradient->setCheckable(true);
        actionRenderIncline = new QAction(MainWindow);
        actionRenderIncline->setObjectName(QString::fromUtf8("actionRenderIncline"));
        actionRenderIncline->setCheckable(true);
        actionAdd3DObject = new QAction(MainWindow);
        actionAdd3DObject->setObjectName(QString::fromUtf8("actionAdd3DObject"));
        actionAddPeople = new QAction(MainWindow);
        actionAddPeople->setObjectName(QString::fromUtf8("actionAddPeople"));
        actionBuildingFind = new QAction(MainWindow);
        actionBuildingFind->setObjectName(QString::fromUtf8("actionBuildingFind"));
        actionPeopleFind = new QAction(MainWindow);
        actionPeopleFind->setObjectName(QString::fromUtf8("actionPeopleFind"));
        actionGeoshapeFormat = new QAction(MainWindow);
        actionGeoshapeFormat->setObjectName(QString::fromUtf8("actionGeoshapeFormat"));
        actionLicence = new QAction(MainWindow);
        actionLicence->setObjectName(QString::fromUtf8("actionLicence"));
        actionBuildingName = new QAction(MainWindow);
        actionBuildingName->setObjectName(QString::fromUtf8("actionBuildingName"));
        actionBuildingName->setCheckable(true);
        actionBuildingName->setChecked(true);
        actionFlightSimulator = new QAction(MainWindow);
        actionFlightSimulator->setObjectName(QString::fromUtf8("actionFlightSimulator"));
        actionFlightSimulator->setCheckable(true);
        actionCarSimulator = new QAction(MainWindow);
        actionCarSimulator->setObjectName(QString::fromUtf8("actionCarSimulator"));
        actionCarSimulator->setCheckable(true);
        actionTourPlay = new QAction(MainWindow);
        actionTourPlay->setObjectName(QString::fromUtf8("actionTourPlay"));
        actionTourPlay->setCheckable(true);
        actionRuler = new QAction(MainWindow);
        actionRuler->setObjectName(QString::fromUtf8("actionRuler"));
        actionRuler->setCheckable(true);
        actionSaveStartPos = new QAction(MainWindow);
        actionSaveStartPos->setObjectName(QString::fromUtf8("actionSaveStartPos"));
        actionShowStandardToolBar = new QAction(MainWindow);
        actionShowStandardToolBar->setObjectName(QString::fromUtf8("actionShowStandardToolBar"));
        actionShowStandardToolBar->setCheckable(true);
        actionShowSimulationToolBar = new QAction(MainWindow);
        actionShowSimulationToolBar->setObjectName(QString::fromUtf8("actionShowSimulationToolBar"));
        actionShowSimulationToolBar->setCheckable(true);
        actionMoveToMyPlace = new QAction(MainWindow);
        actionMoveToMyPlace->setObjectName(QString::fromUtf8("actionMoveToMyPlace"));
        actionShowOverviewMap = new QAction(MainWindow);
        actionShowOverviewMap->setObjectName(QString::fromUtf8("actionShowOverviewMap"));
        actionShowOverviewMap->setCheckable(true);
        actionShowOverviewMap->setChecked(true);
        actionAnimation = new QAction(MainWindow);
        actionAnimation->setObjectName(QString::fromUtf8("actionAnimation"));
        actionMeasureArea = new QAction(MainWindow);
        actionMeasureArea->setObjectName(QString::fromUtf8("actionMeasureArea"));
        actionObjectPlaceEdit = new QAction(MainWindow);
        actionObjectPlaceEdit->setObjectName(QString::fromUtf8("actionObjectPlaceEdit"));
        actionObjectSetting = new QAction(MainWindow);
        actionObjectSetting->setObjectName(QString::fromUtf8("actionObjectSetting"));
        actionGoTo = new QAction(MainWindow);
        actionGoTo->setObjectName(QString::fromUtf8("actionGoTo"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 600, 18));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        menuEdit = new QMenu(menuBar);
        menuEdit->setObjectName(QString::fromUtf8("menuEdit"));
        menuView = new QMenu(menuBar);
        menuView->setObjectName(QString::fromUtf8("menuView"));
        menuViewSize = new QMenu(menuView);
        menuViewSize->setObjectName(QString::fromUtf8("menuViewSize"));
        menuComputer = new QMenu(menuViewSize);
        menuComputer->setObjectName(QString::fromUtf8("menuComputer"));
        menuTV = new QMenu(menuViewSize);
        menuTV->setObjectName(QString::fromUtf8("menuTV"));
        menuPrintOutput = new QMenu(menuViewSize);
        menuPrintOutput->setObjectName(QString::fromUtf8("menuPrintOutput"));
        menuNavigation = new QMenu(menuView);
        menuNavigation->setObjectName(QString::fromUtf8("menuNavigation"));
        menuToolbar = new QMenu(menuView);
        menuToolbar->setObjectName(QString::fromUtf8("menuToolbar"));
        menuSetting = new QMenu(menuBar);
        menuSetting->setObjectName(QString::fromUtf8("menuSetting"));
        menuHelp = new QMenu(menuBar);
        menuHelp->setObjectName(QString::fromUtf8("menuHelp"));
        menuLayer = new QMenu(menuBar);
        menuLayer->setObjectName(QString::fromUtf8("menuLayer"));
        menuRender = new QMenu(menuLayer);
        menuRender->setObjectName(QString::fromUtf8("menuRender"));
        menuAdd = new QMenu(menuBar);
        menuAdd->setObjectName(QString::fromUtf8("menuAdd"));
        menuFind = new QMenu(menuBar);
        menuFind->setObjectName(QString::fromUtf8("menuFind"));
        menuSimulation = new QMenu(menuBar);
        menuSimulation->setObjectName(QString::fromUtf8("menuSimulation"));
        menuTool = new QMenu(menuBar);
        menuTool->setObjectName(QString::fromUtf8("menuTool"));
        MainWindow->setMenuBar(menuBar);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuEdit->menuAction());
        menuBar->addAction(menuFind->menuAction());
        menuBar->addAction(menuView->menuAction());
        menuBar->addAction(menuLayer->menuAction());
        menuBar->addAction(menuAdd->menuAction());
        menuBar->addAction(menuTool->menuAction());
        menuBar->addAction(menuSimulation->menuAction());
        menuBar->addAction(menuSetting->menuAction());
        menuBar->addAction(menuHelp->menuAction());
        menuFile->addAction(actionOpenFile);
        menuFile->addAction(actionSaveMyPlace);
        menuFile->addAction(actionSaveAsPlaceMarkFile);
        menuFile->addSeparator();
        menuFile->addAction(actionSaveImage);
        menuFile->addSeparator();
        menuFile->addAction(actionPrint);
        menuFile->addSeparator();
        menuFile->addAction(actionExit);
        menuEdit->addAction(actionCut);
        menuEdit->addAction(actionCopy);
        menuEdit->addAction(actionPaste);
        menuEdit->addAction(actionDelete);
        menuEdit->addSeparator();
        menuEdit->addAction(actionMoveToMyPlace);
        menuEdit->addSeparator();
        menuEdit->addAction(actionImageCopy);
        menuEdit->addSeparator();
        menuEdit->addAction(actionContentDel);
        menuEdit->addSeparator();
        menuEdit->addAction(actionFindHistoryDel);
        menuEdit->addSeparator();
        menuEdit->addAction(actionProperty);
        menuView->addAction(actionGrid);
        menuView->addSeparator();
        menuView->addAction(menuViewSize->menuAction());
        menuView->addAction(actionFullScreen);
        menuView->addSeparator();
        menuView->addAction(menuNavigation->menuAction());
        menuView->addSeparator();
        menuView->addAction(menuToolbar->menuAction());
        menuView->addAction(actionStatusbar);
        menuView->addAction(actionSidebar);
        menuView->addAction(actionShowOverviewMap);
        menuView->addSeparator();
        menuView->addAction(actionBuildingName);
        menuViewSize->addAction(menuComputer->menuAction());
        menuViewSize->addAction(menuTV->menuAction());
        menuViewSize->addAction(menuPrintOutput->menuAction());
        menuComputer->addAction(actionVGA);
        menuComputer->addAction(actionSVGA);
        menuTV->addAction(actionNTSC);
        menuTV->addAction(actionPAL);
        menuTV->addAction(actionWideNTSC);
        menuTV->addAction(actionWidePAL);
        menuTV->addAction(action1080pHDTV);
        menuTV->addAction(action720pHDTV);
        menuTV->addAction(action480pDVD);
        menuPrintOutput->addAction(actionLetter);
        menuPrintOutput->addAction(actionTabloid);
        menuPrintOutput->addAction(actionPostCard1);
        menuPrintOutput->addAction(actionPostCard2);
        menuNavigation->addAction(actionNavAuto);
        menuNavigation->addAction(actionNavAlways);
        menuNavigation->addAction(actionNavDial);
        menuNavigation->addAction(actionNavNever);
        menuToolbar->addAction(actionShowStandardToolBar);
        menuToolbar->addAction(actionShowSimulationToolBar);
        menuSetting->addAction(actionOption);
        menuSetting->addAction(actionServer);
        menuSetting->addAction(actionContourSetting);
        menuSetting->addAction(actionGeoshapeFormat);
        menuSetting->addAction(actionObjectSetting);
        menuSetting->addSeparator();
        menuSetting->addAction(actionLicence);
        menuHelp->addAction(actionHelpManual);
        menuHelp->addSeparator();
        menuHelp->addAction(actionAbout);
        menuLayer->addAction(actionTerrain);
        menuLayer->addAction(menuRender->menuAction());
        menuLayer->addAction(actionContour);
        menuRender->addAction(actionRenderTexture);
        menuRender->addAction(actionRenderAltitude);
        menuRender->addAction(actionRenderGradient);
        menuRender->addAction(actionRenderIncline);
        menuAdd->addAction(actionAddFolder);
        menuAdd->addAction(actionAddPlaceMark);
        menuAdd->addAction(actionAddPath);
        menuAdd->addAction(actionAddPolygon);
        menuAdd->addAction(actionAdd3DObject);
        menuAdd->addAction(actionObjectPlaceEdit);
        menuFind->addAction(actionBuildingFind);
        menuFind->addAction(actionPeopleFind);
        menuSimulation->addAction(actionFlightSimulator);
        menuSimulation->addAction(actionCarSimulator);
        menuSimulation->addAction(actionTourPlay);
        menuTool->addAction(actionRuler);
        menuTool->addAction(actionMeasureArea);
        menuTool->addAction(actionGoTo);
        menuTool->addAction(actionSaveStartPos);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "GDEM Client", nullptr));
        actionPrint->setText(QCoreApplication::translate("MainWindow", "Print...", nullptr));
#if QT_CONFIG(shortcut)
        actionPrint->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+P", nullptr));
#endif // QT_CONFIG(shortcut)
        actionExit->setText(QCoreApplication::translate("MainWindow", "Exit", nullptr));
        actionCut->setText(QCoreApplication::translate("MainWindow", "Cut", nullptr));
#if QT_CONFIG(shortcut)
        actionCut->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+X", nullptr));
#endif // QT_CONFIG(shortcut)
        actionCopy->setText(QCoreApplication::translate("MainWindow", "Copy", nullptr));
#if QT_CONFIG(shortcut)
        actionCopy->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+C", nullptr));
#endif // QT_CONFIG(shortcut)
        actionImageCopy->setText(QCoreApplication::translate("MainWindow", "Copy Image", nullptr));
#if QT_CONFIG(shortcut)
        actionImageCopy->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+Alt+C", nullptr));
#endif // QT_CONFIG(shortcut)
        actionPaste->setText(QCoreApplication::translate("MainWindow", "Paste", nullptr));
#if QT_CONFIG(shortcut)
        actionPaste->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+V", nullptr));
#endif // QT_CONFIG(shortcut)
        actionDelete->setText(QCoreApplication::translate("MainWindow", "Delete", nullptr));
#if QT_CONFIG(shortcut)
        actionDelete->setShortcut(QCoreApplication::translate("MainWindow", "Del", nullptr));
#endif // QT_CONFIG(shortcut)
        actionContentDel->setText(QCoreApplication::translate("MainWindow", "Delete Contents", nullptr));
        actionFindHistoryDel->setText(QCoreApplication::translate("MainWindow", "Clear Search History", nullptr));
        actionProperty->setText(QCoreApplication::translate("MainWindow", "Properties", nullptr));
#if QT_CONFIG(shortcut)
        actionProperty->setShortcut(QCoreApplication::translate("MainWindow", "Alt+Return", nullptr));
#endif // QT_CONFIG(shortcut)
        actionSidebar->setText(QCoreApplication::translate("MainWindow", "Sidebar", nullptr));
        actionFullScreen->setText(QCoreApplication::translate("MainWindow", "Full Screen", nullptr));
#if QT_CONFIG(shortcut)
        actionFullScreen->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+F11", nullptr));
#endif // QT_CONFIG(shortcut)
        actionStatusbar->setText(QCoreApplication::translate("MainWindow", "Status Bar", nullptr));
        actionGrid->setText(QCoreApplication::translate("MainWindow", "Grid", nullptr));
#if QT_CONFIG(shortcut)
        actionGrid->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+L", nullptr));
#endif // QT_CONFIG(shortcut)
        actionScaleLegend->setText(QCoreApplication::translate("MainWindow", "Scale Legend", nullptr));
        actionSaveStartPos1->setText(QCoreApplication::translate("MainWindow", "Make this my start location", nullptr));
        actionVGA->setText(QCoreApplication::translate("MainWindow", "640 x 480", nullptr));
#if QT_CONFIG(tooltip)
        actionVGA->setToolTip(QCoreApplication::translate("MainWindow", "640 x 480", nullptr));
#endif // QT_CONFIG(tooltip)
        actionSVGA->setText(QCoreApplication::translate("MainWindow", "800 x 600", nullptr));
#if QT_CONFIG(tooltip)
        actionSVGA->setToolTip(QCoreApplication::translate("MainWindow", "800 x 600", nullptr));
#endif // QT_CONFIG(tooltip)
        actionNTSC->setText(QCoreApplication::translate("MainWindow", "720 x 486(NTSC)", nullptr));
        actionPAL->setText(QCoreApplication::translate("MainWindow", "720 x 576(PAL)", nullptr));
        actionWideNTSC->setText(QCoreApplication::translate("MainWindow", "864 x 486(NTSC 16:9)", nullptr));
        actionWidePAL->setText(QCoreApplication::translate("MainWindow", "1024 x 576(PAL 16:9)", nullptr));
        action1080pHDTV->setText(QCoreApplication::translate("MainWindow", "1920 x 1080p(HDTV)", nullptr));
        action720pHDTV->setText(QCoreApplication::translate("MainWindow", "1280 x 720p(HDTV)", nullptr));
        action480pDVD->setText(QCoreApplication::translate("MainWindow", "852 x 480p(DVD)", nullptr));
        actionLetter->setText(QCoreApplication::translate("MainWindow", "8.5 in. * 11 in.(Landscape Aspect ratio)", nullptr));
        actionTabloid->setText(QCoreApplication::translate("MainWindow", "11 in. * 17 in.(Landscape Aspect ratio)", nullptr));
        actionPostCard1->setText(QCoreApplication::translate("MainWindow", "5 in. * 7 in.(Landscape Aspect ratio))", nullptr));
        actionPostCard2->setText(QCoreApplication::translate("MainWindow", "4 in. * 6 in.(Landscape Aspect ratio)", nullptr));
        actionNavAuto->setText(QCoreApplication::translate("MainWindow", "Automatically", nullptr));
        actionNavAlways->setText(QCoreApplication::translate("MainWindow", "Always", nullptr));
        actionNavDial->setText(QCoreApplication::translate("MainWindow", "Compass Only", nullptr));
        actionNavNever->setText(QCoreApplication::translate("MainWindow", "Never", nullptr));
        actionOption->setText(QCoreApplication::translate("MainWindow", "Options...", nullptr));
        actionAddFolder->setText(QCoreApplication::translate("MainWindow", "Folder", nullptr));
#if QT_CONFIG(shortcut)
        actionAddFolder->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+Shift+N", nullptr));
#endif // QT_CONFIG(shortcut)
        actionAddPlaceMark->setText(QCoreApplication::translate("MainWindow", "Placemark", nullptr));
#if QT_CONFIG(tooltip)
        actionAddPlaceMark->setToolTip(QCoreApplication::translate("MainWindow", "Add Placemark", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(shortcut)
        actionAddPlaceMark->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+Shift+P", nullptr));
#endif // QT_CONFIG(shortcut)
        actionHelpManual->setText(QCoreApplication::translate("MainWindow", "User Guide", nullptr));
        actionAbout->setText(QCoreApplication::translate("MainWindow", "About GDEM Client", nullptr));
        actionRuler1->setText(QCoreApplication::translate("MainWindow", "Ruler", nullptr));
#if QT_CONFIG(tooltip)
        actionRuler1->setToolTip(QCoreApplication::translate("MainWindow", "Show Ruler", nullptr));
#endif // QT_CONFIG(tooltip)
        actionOpenFile->setText(QCoreApplication::translate("MainWindow", "Open...", nullptr));
#if QT_CONFIG(shortcut)
        actionOpenFile->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+O", nullptr));
#endif // QT_CONFIG(shortcut)
        actionSaveAsPlaceMarkFile->setText(QCoreApplication::translate("MainWindow", "Save Place As ...", nullptr));
#if QT_CONFIG(shortcut)
        actionSaveAsPlaceMarkFile->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+S", nullptr));
#endif // QT_CONFIG(shortcut)
        actionMoveToMyPlaceq->setText(QCoreApplication::translate("MainWindow", "Move to My Places", nullptr));
#if QT_CONFIG(shortcut)
        actionMoveToMyPlaceq->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+Shift+S", nullptr));
#endif // QT_CONFIG(shortcut)
        actionSaveMyPlace->setText(QCoreApplication::translate("MainWindow", "Save My Places", nullptr));
        actionSaveImage->setText(QCoreApplication::translate("MainWindow", "Save Image", nullptr));
#if QT_CONFIG(shortcut)
        actionSaveImage->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+Alt+S", nullptr));
#endif // QT_CONFIG(shortcut)
        actionContourSetting->setText(QCoreApplication::translate("MainWindow", "Contour Setting", nullptr));
        actionTerrain->setText(QCoreApplication::translate("MainWindow", "Terrain", nullptr));
        actionContour->setText(QCoreApplication::translate("MainWindow", "Contour", nullptr));
        actionServer->setText(QCoreApplication::translate("MainWindow", "Server Setting", nullptr));
        actionAddPath->setText(QCoreApplication::translate("MainWindow", "Path", nullptr));
#if QT_CONFIG(tooltip)
        actionAddPath->setToolTip(QCoreApplication::translate("MainWindow", "Add Path", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(shortcut)
        actionAddPath->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+Shift+T", nullptr));
#endif // QT_CONFIG(shortcut)
        actionAddPolygon->setText(QCoreApplication::translate("MainWindow", "Polygon", nullptr));
#if QT_CONFIG(tooltip)
        actionAddPolygon->setToolTip(QCoreApplication::translate("MainWindow", "Add Polygon", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(shortcut)
        actionAddPolygon->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+Shift+G", nullptr));
#endif // QT_CONFIG(shortcut)
        actionFlightSimulator1->setText(QCoreApplication::translate("MainWindow", "Enter Flight Simulator", nullptr));
        actionCarSimulator1->setText(QCoreApplication::translate("MainWindow", "Enter Car Simulator", nullptr));
        actionTourPlay1->setText(QCoreApplication::translate("MainWindow", "TourPlay", nullptr));
        actionRenderTexture->setText(QCoreApplication::translate("MainWindow", "Texture", nullptr));
        actionRenderAltitude->setText(QCoreApplication::translate("MainWindow", "Altitude", nullptr));
        actionRenderGradient->setText(QCoreApplication::translate("MainWindow", "Gradient", nullptr));
        actionRenderIncline->setText(QCoreApplication::translate("MainWindow", "Incline", nullptr));
        actionAdd3DObject->setText(QCoreApplication::translate("MainWindow", "Add 3D Object...", nullptr));
        actionAddPeople->setText(QCoreApplication::translate("MainWindow", "Add People...", nullptr));
        actionBuildingFind->setText(QCoreApplication::translate("MainWindow", "BuildingFind", nullptr));
        actionPeopleFind->setText(QCoreApplication::translate("MainWindow", "PeopleFind", nullptr));
        actionGeoshapeFormat->setText(QCoreApplication::translate("MainWindow", "Geoshape Format...", nullptr));
        actionLicence->setText(QCoreApplication::translate("MainWindow", "Licence", nullptr));
        actionBuildingName->setText(QCoreApplication::translate("MainWindow", "Building Name", nullptr));
        actionFlightSimulator->setText(QCoreApplication::translate("MainWindow", "Enter Flight Simulator", nullptr));
        actionCarSimulator->setText(QCoreApplication::translate("MainWindow", "Enter Car Simulator", nullptr));
        actionTourPlay->setText(QCoreApplication::translate("MainWindow", "TourPlay", nullptr));
        actionRuler->setText(QCoreApplication::translate("MainWindow", "Ruler", nullptr));
        actionSaveStartPos->setText(QCoreApplication::translate("MainWindow", "Make this my start location", nullptr));
        actionShowStandardToolBar->setText(QCoreApplication::translate("MainWindow", "Standard", nullptr));
        actionShowSimulationToolBar->setText(QCoreApplication::translate("MainWindow", "Simulation", nullptr));
        actionMoveToMyPlace->setText(QCoreApplication::translate("MainWindow", "Move To My Place", nullptr));
        actionShowOverviewMap->setText(QCoreApplication::translate("MainWindow", "Overview Map", nullptr));
        actionAnimation->setText(QCoreApplication::translate("MainWindow", "Animation...", nullptr));
        actionMeasureArea->setText(QCoreApplication::translate("MainWindow", "Measure Area", nullptr));
        actionObjectPlaceEdit->setText(QCoreApplication::translate("MainWindow", "ObjectPlaceEdit...", nullptr));
        actionObjectSetting->setText(QCoreApplication::translate("MainWindow", "ObjectSetting...", nullptr));
        actionGoTo->setText(QCoreApplication::translate("MainWindow", "GoTo", nullptr));
        menuFile->setTitle(QCoreApplication::translate("MainWindow", "&File", nullptr));
        menuEdit->setTitle(QCoreApplication::translate("MainWindow", "&Edit", nullptr));
        menuView->setTitle(QCoreApplication::translate("MainWindow", "&View", nullptr));
        menuViewSize->setTitle(QCoreApplication::translate("MainWindow", "View Size", nullptr));
        menuComputer->setTitle(QCoreApplication::translate("MainWindow", "Computer Playback", nullptr));
        menuTV->setTitle(QCoreApplication::translate("MainWindow", "TV Playback", nullptr));
        menuPrintOutput->setTitle(QCoreApplication::translate("MainWindow", "Print Output", nullptr));
        menuNavigation->setTitle(QCoreApplication::translate("MainWindow", "Show Navigation", nullptr));
        menuToolbar->setTitle(QCoreApplication::translate("MainWindow", "Toolbar", nullptr));
        menuSetting->setTitle(QCoreApplication::translate("MainWindow", "&Setting", nullptr));
        menuHelp->setTitle(QCoreApplication::translate("MainWindow", "&Help", nullptr));
        menuLayer->setTitle(QCoreApplication::translate("MainWindow", "&Layers", nullptr));
        menuRender->setTitle(QCoreApplication::translate("MainWindow", "Render", nullptr));
        menuAdd->setTitle(QCoreApplication::translate("MainWindow", "&Add", nullptr));
        menuFind->setTitle(QCoreApplication::translate("MainWindow", "Find", nullptr));
        menuSimulation->setTitle(QCoreApplication::translate("MainWindow", "Simulation", nullptr));
        menuTool->setTitle(QCoreApplication::translate("MainWindow", "Tool", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
