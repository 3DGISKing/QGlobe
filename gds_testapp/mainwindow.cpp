#include <QtGui>
#include <QDir>
#include <QString>
#include <QMessageBox>

#include "mainwindow.h"
#include "../gds_core/GDSCoreInterface.h"
#include "GDSTestSuiteManager.h"

MainWindow::MainWindow()
{
	ui.setupUi(this);

	m_bStartedServer = false;

	/*QWidget *widget = new QWidget;
	setCentralWidget(widget);

	QWidget *topFiller = new QWidget;
	topFiller->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

	QWidget *bottomFiller = new QWidget;
	bottomFiller->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

        QVBoxLayout *layout =61
 new QVBoxLayout;
	layout->setMargin(5);
	layout->addWidget(topFiller);
	layout->addWidget(bottomFiller);
	widget->setLayout(layout);*/

	connect(ui.actionStart_Server, SIGNAL(triggered()), this, SLOT(OnStartServer()));
	connect(ui.actionStop_Server, SIGNAL(triggered()), this, SLOT(OnStopServer()));
	connect(ui.actionExit, SIGNAL(triggered()), this, SLOT(close()));

	connect(ui.actionFlow_Test, SIGNAL(triggered()), this, SLOT(OnFlowTest()));
	connect(ui.actionSynchronize_Test, SIGNAL(triggered()), this, SLOT(OnSynchronizeTest()));
	connect(ui.actionAsynchronize_Test, SIGNAL(triggered()), this, SLOT(OnAsynchronizeTest()));

	UpdateMenu();

	QString message = tr("A context menu is available by right-clicking");
	statusBar()->showMessage(message);

	setWindowTitle(tr("Menus"));

	m_pTestMngr = new GDSTestSuiteManager;
}

MainWindow::~MainWindow()
{
	if (m_pTestMngr)
	{
		delete m_pTestMngr;
		m_pTestMngr = NULL;
	}
}

void MainWindow::OnStartServer()
{
	if (m_bStartedServer)
		return;

        QString curPath = QDir::currentPath();

        if (InitializeGDSCore(curPath))
		m_bStartedServer = true;
	else
		QMessageBox::critical(NULL, "Error", "Occured error to start server");

	UpdateMenu();
}

void MainWindow::OnStopServer()
{
	if (m_bStartedServer)
		ReleaseGDSCore();

	m_bStartedServer = false;

	UpdateMenu();
}

void MainWindow::OnFlowTest()
{
	QString outStr;
	outStr = m_pTestMngr->GetFlowTestResult();
	ui.plainTextEdit->clear();
	ui.plainTextEdit->appendPlainText(outStr);
}

void MainWindow::OnSynchronizeTest()
{
	QString outStr;
	outStr = m_pTestMngr->GetSyncTestResult();
	ui.plainTextEdit->clear();
	ui.plainTextEdit->appendPlainText(outStr);
}

void MainWindow::OnAsynchronizeTest()
{
	QString outStr;
	outStr = m_pTestMngr->GetAsyncTestResult();
	ui.plainTextEdit->clear();
	ui.plainTextEdit->appendPlainText(outStr);
}

void MainWindow::UpdateMenu()
{
	ui.actionStart_Server->setEnabled(!m_bStartedServer);
	ui.actionStop_Server->setEnabled(m_bStartedServer);
}
