/********************************************************************************
** Form generated from reading UI file 'shapedraw.ui'
**
** Created by: Qt User Interface Compiler version 5.15.18
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SHAPEDRAW_H
#define UI_SHAPEDRAW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ShapeDrawClass
{
public:
    QAction *actionExit;
    QAction *actionAdd;
    QAction *actionConvert;
    QAction *actionClear;
    QWidget *centralWidget;
    QListWidget *treeTOC;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuTheme;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *ShapeDrawClass)
    {
        if (ShapeDrawClass->objectName().isEmpty())
            ShapeDrawClass->setObjectName(QString::fromUtf8("ShapeDrawClass"));
        ShapeDrawClass->resize(855, 607);
        actionExit = new QAction(ShapeDrawClass);
        actionExit->setObjectName(QString::fromUtf8("actionExit"));
        actionAdd = new QAction(ShapeDrawClass);
        actionAdd->setObjectName(QString::fromUtf8("actionAdd"));
        actionConvert = new QAction(ShapeDrawClass);
        actionConvert->setObjectName(QString::fromUtf8("actionConvert"));
        actionClear = new QAction(ShapeDrawClass);
        actionClear->setObjectName(QString::fromUtf8("actionClear"));
        centralWidget = new QWidget(ShapeDrawClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        treeTOC = new QListWidget(centralWidget);
        treeTOC->setObjectName(QString::fromUtf8("treeTOC"));
        treeTOC->setGeometry(QRect(10, 10, 241, 241));
        ShapeDrawClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(ShapeDrawClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 855, 19));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        menuTheme = new QMenu(menuBar);
        menuTheme->setObjectName(QString::fromUtf8("menuTheme"));
        ShapeDrawClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(ShapeDrawClass);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        ShapeDrawClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(ShapeDrawClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        ShapeDrawClass->setStatusBar(statusBar);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuTheme->menuAction());
        menuFile->addAction(actionExit);
        menuTheme->addAction(actionAdd);
        menuTheme->addAction(actionConvert);
        menuTheme->addSeparator();
        menuTheme->addAction(actionClear);

        retranslateUi(ShapeDrawClass);

        QMetaObject::connectSlotsByName(ShapeDrawClass);
    } // setupUi

    void retranslateUi(QMainWindow *ShapeDrawClass)
    {
        ShapeDrawClass->setWindowTitle(QCoreApplication::translate("ShapeDrawClass", "ShapeDraw", nullptr));
        actionExit->setText(QCoreApplication::translate("ShapeDrawClass", "Exit", nullptr));
        actionAdd->setText(QCoreApplication::translate("ShapeDrawClass", "Add...", nullptr));
        actionConvert->setText(QCoreApplication::translate("ShapeDrawClass", "Convert...", nullptr));
        actionClear->setText(QCoreApplication::translate("ShapeDrawClass", "Clear All Data", nullptr));
        menuFile->setTitle(QCoreApplication::translate("ShapeDrawClass", "File", nullptr));
        menuTheme->setTitle(QCoreApplication::translate("ShapeDrawClass", "Theme", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ShapeDrawClass: public Ui_ShapeDrawClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SHAPEDRAW_H
