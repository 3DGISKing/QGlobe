/********************************************************************************
** Form generated from reading UI file 'gdstestsuite.ui'
**
** Created by: Qt User Interface Compiler version 5.15.18
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GDSTESTSUITE_H
#define UI_GDSTESTSUITE_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_QGlobeDSTestSuiteClass
{
public:
    QAction *actionStart_Server;
    QAction *actionStop_Server;
    QAction *actionExit;
    QAction *actionFlow_Test;
    QAction *actionSynchronize_Test;
    QAction *actionAsynchronize_Test;
    QWidget *centralWidget;
    QPlainTextEdit *plainTextEdit;
    QMenuBar *menuBar;
    QMenu *menuCommand;
    QMenu *menuTest_Module;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *QGlobeDSTestSuiteClass)
    {
        if (QGlobeDSTestSuiteClass->objectName().isEmpty())
            QGlobeDSTestSuiteClass->setObjectName(QString::fromUtf8("QGlobeDSTestSuiteClass"));
        QGlobeDSTestSuiteClass->resize(600, 400);
        actionStart_Server = new QAction(QGlobeDSTestSuiteClass);
        actionStart_Server->setObjectName(QString::fromUtf8("actionStart_Server"));
        actionStop_Server = new QAction(QGlobeDSTestSuiteClass);
        actionStop_Server->setObjectName(QString::fromUtf8("actionStop_Server"));
        actionExit = new QAction(QGlobeDSTestSuiteClass);
        actionExit->setObjectName(QString::fromUtf8("actionExit"));
        actionFlow_Test = new QAction(QGlobeDSTestSuiteClass);
        actionFlow_Test->setObjectName(QString::fromUtf8("actionFlow_Test"));
        actionSynchronize_Test = new QAction(QGlobeDSTestSuiteClass);
        actionSynchronize_Test->setObjectName(QString::fromUtf8("actionSynchronize_Test"));
        actionAsynchronize_Test = new QAction(QGlobeDSTestSuiteClass);
        actionAsynchronize_Test->setObjectName(QString::fromUtf8("actionAsynchronize_Test"));
        centralWidget = new QWidget(QGlobeDSTestSuiteClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        plainTextEdit = new QPlainTextEdit(centralWidget);
        plainTextEdit->setObjectName(QString::fromUtf8("plainTextEdit"));
        plainTextEdit->setGeometry(QRect(-1, 0, 601, 341));
        QGlobeDSTestSuiteClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(QGlobeDSTestSuiteClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 600, 19));
        menuCommand = new QMenu(menuBar);
        menuCommand->setObjectName(QString::fromUtf8("menuCommand"));
        menuTest_Module = new QMenu(menuCommand);
        menuTest_Module->setObjectName(QString::fromUtf8("menuTest_Module"));
        QGlobeDSTestSuiteClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(QGlobeDSTestSuiteClass);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        QGlobeDSTestSuiteClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(QGlobeDSTestSuiteClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        QGlobeDSTestSuiteClass->setStatusBar(statusBar);

        menuBar->addAction(menuCommand->menuAction());
        menuCommand->addAction(actionStart_Server);
        menuCommand->addAction(actionStop_Server);
        menuCommand->addSeparator();
        menuCommand->addAction(menuTest_Module->menuAction());
        menuCommand->addSeparator();
        menuCommand->addAction(actionExit);
        menuTest_Module->addAction(actionFlow_Test);
        menuTest_Module->addAction(actionSynchronize_Test);
        menuTest_Module->addAction(actionAsynchronize_Test);

        retranslateUi(QGlobeDSTestSuiteClass);

        QMetaObject::connectSlotsByName(QGlobeDSTestSuiteClass);
    } // setupUi

    void retranslateUi(QMainWindow *QGlobeDSTestSuiteClass)
    {
        QGlobeDSTestSuiteClass->setWindowTitle(QCoreApplication::translate("QGlobeDSTestSuiteClass", "QGlobeDSTestSuite", nullptr));
        actionStart_Server->setText(QCoreApplication::translate("QGlobeDSTestSuiteClass", "Start Server", nullptr));
        actionStop_Server->setText(QCoreApplication::translate("QGlobeDSTestSuiteClass", "Stop Server", nullptr));
        actionExit->setText(QCoreApplication::translate("QGlobeDSTestSuiteClass", "Exit", nullptr));
        actionFlow_Test->setText(QCoreApplication::translate("QGlobeDSTestSuiteClass", "Flow Test", nullptr));
        actionSynchronize_Test->setText(QCoreApplication::translate("QGlobeDSTestSuiteClass", "Synchronize Test", nullptr));
        actionAsynchronize_Test->setText(QCoreApplication::translate("QGlobeDSTestSuiteClass", "Asynchronize Test", nullptr));
        menuCommand->setTitle(QCoreApplication::translate("QGlobeDSTestSuiteClass", "Command", nullptr));
        menuTest_Module->setTitle(QCoreApplication::translate("QGlobeDSTestSuiteClass", "Test Module", nullptr));
    } // retranslateUi

};

namespace Ui {
    class QGlobeDSTestSuiteClass: public Ui_QGlobeDSTestSuiteClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GDSTESTSUITE_H
