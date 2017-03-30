#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ui_gdstestsuite.h"

class GDSTestSuiteManager;

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow();
	virtual ~MainWindow();

private slots:
		void OnStartServer();
		void OnStopServer();
		
		void OnFlowTest();
		void OnSynchronizeTest();
		void OnAsynchronizeTest();

private:
	void UpdateMenu();

	bool m_bStartedServer;

	GDSTestSuiteManager *m_pTestMngr;
private:
	Ui::GDSTestSuiteClass ui;
};

#endif