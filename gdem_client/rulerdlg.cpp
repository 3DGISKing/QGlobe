#include "rulerdlg.h"
#include "ui_rulerdlg.h"
#include "gdemclient.h"
#include "mainwindow.h"
#include "config.h"
#include "msgdata.h"
#include "renderfrm.h"
#include "rendercursor.h"
#include "GDM_RulerCmd.h"
#include "GDM_CommandMgr.h"

RulerDlg::RulerDlg(QWidget *parent) :
    CGDM_CommandDlg(parent),
    m_ui(new Ui::RulerDlg)
{
    m_ui->setupUi(this);

	m_measMode = MEASURE_MODE_LINE;
	m_lineMode.length = 0;
	m_lineMode.angle = 0;
	m_lineMode.unit = 2;
	m_pathMode.length = 0;
	m_pathMode.angle = 0;
	m_pathMode.unit = 1;
	m_pCommand		= NULL;

	memset(&m_curState, 0, sizeof(m_curState));

	Init();
	connect(m_ui->mouseCheck, SIGNAL(clicked()), this, SLOT(SetMouseCheck()));
	connect(m_ui->deleteButton, SIGNAL(clicked()), this, SLOT(DeleteMeasureInfo()));
	connect(m_ui->rulerModeTab, SIGNAL(currentChanged(int)), this, SLOT(ChangeMeasureMode(int)));
	connect(m_ui->lineMetricsCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(ChangeMeasureUnit(int)));
	connect(m_ui->pathMetricsCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(ChangeMeasureUnit(int)));
}

RulerDlg::~RulerDlg()
{
    delete m_ui;
}

void RulerDlg::reject()
{
	// empty function for ignore esc key
}

void RulerDlg::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void RulerDlg::closeEvent(QCloseEvent *e)
{
	ReleaseLastPoint ();

	g_app->GetConfig()->m_showRuler = false;
	g_app->GetMainWindow()->UpdateRuler();

	RenderCursor *cursor = g_app->GetMainWindow()->GetRenderFrame()->GetRenderCursor();
	cursor->SetCursor(CURSOR_SHAPE_NORMAL);
}

void RulerDlg::Init()
{
        QPoint mainPos = g_app->GetMainWindow()->pos();
        move(mainPos.x()+20, mainPos.y()+100);

	setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint
				   | Qt::WindowCloseButtonHint);
	m_ui->rulerModeTab->setCurrentIndex(m_measMode);

	QStringList items;
	items   << tr("Korean Ri")<< tr("Kilometre") << tr("Metre") 
		    << tr("Centimeter") << tr("Inch") << tr("Feet")
			<< tr("Yard")       << tr("Mile");

	m_ui->lineMetricsCombo->addItems(items);
        m_ui->lineMetricsCombo->setCurrentIndex(m_lineMode.unit);
	m_ui->pathMetricsCombo->addItems(items);
        m_ui->pathMetricsCombo->setCurrentIndex(m_pathMode.unit);

	m_ui->lineLengthValue->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
	m_ui->lineAngleValue->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
	m_ui->pathLengthValue->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

	OutputResult();
}

void RulerDlg::OnCheckCommand(void)
{
	if(g_app->GetConfig()->m_showRuler)
	{
		if(m_pCommand == NULL)
		{
			// create command
			CGDM_CommandMgr* pCmdMgr = g_app->GetRenderProxy()->GetRenderSrv()->GetCommandMgr();
			m_pCommand = (CGDM_RulerCmd*)pCmdMgr->CreateCmd(eGDM_CMD_RULER, this);
			// end
		}

		if (m_measMode == MEASURE_MODE_LINE)
			m_pCommand->m_blLine = true;
		else
			m_pCommand->m_blLine = false;

		m_pCommand->m_blMouseCheck = m_blMouseCheck;
	}
	else
	{
		if(m_pCommand)
			m_pCommand->OnEnd();
		m_pCommand = NULL;
	}
}

void RulerDlg::OutputResult()
{
	double result = ConvertLength();

	QString length, angle;
	if (m_measMode == MEASURE_MODE_LINE)
	{
		length = QString("%1").arg(result, 0, 'f', 2, QLatin1Char('0'));
		angle = QString("%1").arg(m_lineMode.angle, 0, 'f', 2, QLatin1Char('0'));

		int len = (length.length() - 1) / 3;
		for (int i = 1; i < len; i++)
			length.insert(length.length() - 2 - (4 * i), QChar(','));

		m_ui->lineLengthValue->setText(length);
		m_ui->lineAngleValue->setText(angle);
	}
	else if (m_measMode == MEASURE_MODE_PATH)
	{
		length = QString("%1").arg(result, 0, 'f', 2, QLatin1Char('0'));

		int len = (length.length() - 1) / 3;
		for (int i = 1; i < len; i++)
			length.insert(length.length() - 2 - (4 * i), QChar(','));

		m_ui->pathLengthValue->setText(length);
	}
	m_ui->mouseCheck->setChecked(m_blMouseCheck);
}

double RulerDlg::ConvertLength()
{
	double result = 0.0;

	if (m_measMode == MEASURE_MODE_LINE)
	{
		switch(m_lineMode.unit)
		{
			case 0:	   // added by RSH 2013.3.11 for Korean Ri
				result = m_lineMode.length / 393.0;
				break;
			case 1:
				result = m_lineMode.length/1000;
				break;
			case 2:
				result = m_lineMode.length;
				break;
			case 3:
				result = m_lineMode.length *100;
				break;
			case 4:
				result = (m_lineMode.length * 100) / 2.54;
				break;
			case 5:
				result = (m_lineMode.length * 100) / 30.48;
				break;
			case 6:
				result = (m_lineMode.length * 100) / 91.4;
				break;
			case 7:
				result = m_lineMode.length / 1609.3;
				break;
			default:
				break;
		}
	}
	else if (m_measMode == MEASURE_MODE_PATH)
	{
		switch(m_pathMode.unit)
		{
			case 0:	   // added by RSH 2013.3.11 for Korean Ri
				result = m_pathMode.length / 393.0;
				break;
			case 1:
				result = m_pathMode.length/1000;
				break;
			case 2:
				result = m_pathMode.length;
				break;
			case 3:
				result = m_pathMode.length *100;
				break;
			case 4:
				result = (m_pathMode.length * 100) / 2.54;
				break;
			case 5:
				result = (m_pathMode.length * 100) / 30.48;
				break;
			case 6:
				result = (m_pathMode.length * 100) / 91.4;
				break;
			case 7:
				result = m_pathMode.length / 1609.3;
				break;
			default:
				break;
		}
	}

	return result;
}

void RulerDlg::DeleteMeasureInfo()
{
	if(!m_pCommand)
		return;
	m_pCommand->Clear();

	m_lineMode.length = 0;
	m_lineMode.angle = 0;
	m_pathMode.length = 0;
	m_pathMode.angle = 0;

	if (m_measMode == MEASURE_MODE_LINE)
	{
		if (m_pCommand)
			m_pCommand->SetMeasureMode(m_lineMode);
	}
	else if (m_measMode == MEASURE_MODE_PATH)
	{
		if (m_pCommand)
			m_pCommand->SetMeasureMode(m_pathMode);
	}

	OutputResult();
}

void RulerDlg::SetMouseCheck()
{
	m_blMouseCheck = m_ui->mouseCheck->isChecked();

	if(!m_pCommand)
		return;

	m_pCommand->m_blMouseCheck = m_blMouseCheck;

	OutputResult();
}

void RulerDlg::ChangeMeasureMode(int mode)
{
	if (m_measMode == mode)
		return;

	ReleaseLastPoint ();
	m_measMode = mode;

	OnCheckCommand();

	if (m_measMode == MEASURE_MODE_LINE)
	{
		if (m_pCommand)
			m_pCommand->SetMeasureMode(m_lineMode);
	}
	else if (m_measMode == MEASURE_MODE_PATH)
	{
		if (m_pCommand)
			m_pCommand->SetMeasureMode(m_pathMode);
	}

	OutputResult();
}

void RulerDlg::ChangeMeasureUnit(int unit)
{
	if (m_measMode == MEASURE_MODE_LINE)
	{
		m_lineMode.unit = unit;

		if (m_pCommand)
			m_pCommand->SetMeasureMode(m_lineMode);
	}
	else if (m_measMode == MEASURE_MODE_PATH)
	{
		m_pathMode.unit = unit;
		if (m_pCommand)
			m_pCommand->SetMeasureMode(m_pathMode);
	}

	OutputResult();
}

void RulerDlg::UpdateMeasureInfo()
{
	if(!m_pCommand)
		return;
	if (m_measMode == MEASURE_MODE_LINE)
	{
		m_lineMode.length = m_pCommand->CountAllLength();
		m_lineMode.angle = m_pCommand->CountAngle();
	}
	else if (m_measMode == MEASURE_MODE_PATH)
	{
		m_pathMode.length = m_pCommand->CountAllLength();
	}

	OutputResult();
}


void RulerDlg::UpdateDialog(void)
{
	UpdateMeasureInfo();
}

void RulerDlg::ReleaseLastPoint()
{
	m_pCommand->OnEnd();
	m_pCommand = NULL;
}
