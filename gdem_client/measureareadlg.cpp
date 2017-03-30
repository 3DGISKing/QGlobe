#include "stable.h"
#include <QtCore/QTimer>

#include "measureareadlg.h"
#include "gdemclient.h"
#include "config.h"
#include "GDM_MeasureAreaCmd.h"
#include "GDM_CommandMgr.h"
#include "mainwindow.h"
#include "sidebar.h"
#include "GIS_Doc.h"
#include "GIS_Area.h"

#include "../gdem_gis/GIS_Polygon.h"

MeasureAreaDlg::MeasureAreaDlg(QWidget *parent)
	: CGDM_CommandDlg(parent)
{
	ui.setupUi(this);
	m_pCommand=NULL;

	ui.groupBoxSimulation->setChecked(false);

	ui.pushButtonStop->setEnabled(false);
	ui.lineEditMoveDirection->setText("0");
	ui.lineEditMoveVelocity->setText("1");
	ui.lineEditGrowVelocity->setText("1");

	connect(ui.deleteButton, SIGNAL(clicked()), this, SLOT(onClear()));
	connect(ui.groupBoxSimulation,SIGNAL(toggled(bool)),this,SLOT(onGroupBoxSimulationToggled(bool)));
	connect(ui.dialMoveDirection,SIGNAL(valueChanged(int )),this,SLOT(onDialMoveDirectionValueChanged(int)));

	connect(ui.pushButtonStart,SIGNAL(clicked()),this,SLOT(onPushButtonStartClicked()));
	connect(ui.pushButtonStop,SIGNAL(clicked()),this,SLOT(onPushButtonStopClicked()));
	connect(ui.lineEditMoveVelocity,SIGNAL(editingFinished()),SLOT(onLineEditMoveVelocityEditingFinished()));
	connect(ui.lineEditGrowVelocity,SIGNAL(editingFinished()),SLOT(onLineEditGrowVelocityEditingFinished()));

	connect(ui.btnSearch,SIGNAL(clicked()),SLOT(btnSearch_click()));
}

MeasureAreaDlg::~MeasureAreaDlg()
{
	
}

void MeasureAreaDlg::onLineEditGrowVelocityEditingFinished()
{
	QString val=ui.lineEditGrowVelocity->text();

	bool ok;
	double growvelocity=val.toDouble(&ok);

	growvelocity=growvelocity/EARTH_RADIUS;

	if(ok)
		m_pCommand->SetGrowVelocity(growvelocity);
	else
	{

	}
}
void MeasureAreaDlg::onLineEditMoveVelocityEditingFinished()
{
	QString val=ui.lineEditMoveVelocity->text();

	bool ok;
	double movevelocity=val.toDouble(&ok);
	
	movevelocity=movevelocity/EARTH_RADIUS;

	if(ok)
		m_pCommand->SetMoveVelocity(movevelocity);
	else
	{
		
	}
}
void MeasureAreaDlg::onDialMoveDirectionValueChanged(int val)
{
	val=360-val;

	QString s;
	s.setNum(val);
	ui.lineEditMoveDirection->setText(s);
	m_pCommand->SetMoveDirection(val-90);
}

void MeasureAreaDlg::onGroupBoxSimulationToggled(bool on)
{

}

void MeasureAreaDlg::onPushButtonStartClicked()
{
	m_SimulationTimer.start(1000,this);

	onLineEditMoveVelocityEditingFinished();
	onLineEditGrowVelocityEditingFinished();
	
	ui.pushButtonStart->setEnabled(false);
	ui.pushButtonStop->setEnabled(true);
}

void MeasureAreaDlg::onPushButtonStopClicked()
{
	m_SimulationTimer.stop();
	ui.pushButtonStart->setEnabled(true);
	ui.pushButtonStop->setEnabled(false);
}



void MeasureAreaDlg::onClear()
{
	Q_ASSERT(m_pCommand!=NULL);
	m_pCommand->Clear();
	onPushButtonStopClicked();
}

void MeasureAreaDlg::UpdateDialog()
{
	Q_ASSERT(m_pCommand!=NULL);

	double area=m_pCommand->GetArea();

	QString info;

	if (area < 1000000)
		info = QString("%1m%2").arg(area).arg(QChar(0x00B2));
	else
		info = QString("%1km%2").arg(area/1000000).arg(QChar(0x00B2));

	ui.lineArea->setText(info);
}

void MeasureAreaDlg::OnCheckCommand()
{
	if(g_app->GetConfig()->m_showMeasureArea)
	{
		if(m_pCommand == NULL)
		{
			
			g_app->GetMainWindow()->GetSideBar()->setSearchStyle(0);
			CGDM_CommandMgr* pCmdMgr = g_app->GetRenderProxy()->GetRenderSrv()->GetCommandMgr();
			m_pCommand = (CGDM_MeasureAreaCmd*)pCmdMgr->CreateCmd(eGDM_CMD_MEASUREAREA, this);
		}
	}
	else
	{
		onPushButtonStopClicked();

		if(m_pCommand)
			m_pCommand->OnEnd();
		
		m_pCommand = NULL;
	}
}

void MeasureAreaDlg::closeEvent(QCloseEvent * e)
{
	m_pCommand->OnCancel();
	m_pCommand=NULL;
	g_app->GetConfig()->m_showMeasureArea = false;
	g_app->GetMainWindow()->UpdateMeasureArea();
}

void MeasureAreaDlg::timerEvent(QTimerEvent* e)
{
	Q_ASSERT(m_pCommand!=NULL);

	m_pCommand->OnSimulation();
}

void MeasureAreaDlg::btnSearch_click()
{
	Q_ASSERT(m_pCommand!=NULL);

	if(m_pCommand->GetNode()->Get_TrackCount() < 3) return;

	onPushButtonStopClicked();

	CGIS_Area* searcharea=new CGIS_Area();

	searcharea->CopyFrom(*(m_pCommand->GetNode()));
	
	g_app->GetConfig()->m_showMeasureArea=false;
	m_pCommand->OnCancel();

	m_pCommand=NULL;

	g_app->GetMainWindow()->GetSideBar()->setSearchRegion(searcharea);

	delete searcharea;

	QDialog::reject();
}
