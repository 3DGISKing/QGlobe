#include "placefolderdlg.h"
#include "ui_placefolderdlg.h"
#include "gdemclient.h"
#include "mainwindow.h"
#include "renderfrm.h"
#include "placemarkmgr.h"
#include "msgdata.h"
#include "guiuty.h"
#include "config.h"

PlaceFolderDlg::PlaceFolderDlg(QWidget *parent, bool isAdd) :
    QDialog(parent),
    m_ui(new Ui::PlaceFolderDlg)
{
    m_ui->setupUi(this);

	setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint
				   | Qt::WindowCloseButtonHint);
	setAttribute(Qt::WA_DeleteOnClose);
	m_ui->propertyTab->setCurrentIndex(0);
	m_ui->okButton->setFocus(Qt::OtherFocusReason);
	m_ui->lineName->setMaxLength(PMD_STR_MAX_SIZE-1);

	m_placeMarkMgr = g_app->GetMainWindow()->GetRenderFrame()->GetPlaceMarkMgr();
	memset(&m_placeMarkData, 0, sizeof(m_placeMarkData));
	m_isAdd = isAdd;
	m_state = PLACEDLG_NORMAL;

	connect(m_ui->propertyTab, SIGNAL(currentChanged(int)), this, SLOT(SetFocusOkButton()));
	connect(m_ui->lineName, SIGNAL(textChanged(QString)), this, SLOT(SetFolderName()));
	connect(m_ui->textDescription, SIGNAL(textChanged()), this, SLOT(SetDescription()));
	connect(m_ui->lineViewLatitude, SIGNAL(editingFinished()), this, SLOT(SetViewLatitude()));
	connect(m_ui->lineViewLongitude, SIGNAL(editingFinished()), this, SLOT(SetViewLongitude()));
	connect(m_ui->lineViewRange, SIGNAL(editingFinished()), this, SLOT(SetViewRange()));
	connect(m_ui->lineViewHeading, SIGNAL(editingFinished()), this, SLOT(SetViewHeading()));
	connect(m_ui->lineViewTilt, SIGNAL(editingFinished()), this, SLOT(SetViewTilt()));
	connect(m_ui->snapshotViewButton, SIGNAL(clicked()), this, SLOT(SnapshotView()));
	connect(m_ui->resetButton, SIGNAL(clicked()), this, SLOT(ResetView()));
	connect(m_ui->okButton, SIGNAL(clicked()), this, SLOT(accept()));
	connect(m_ui->cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
}

PlaceFolderDlg::~PlaceFolderDlg()
{
    delete m_ui;

	if (m_state == PLACEDLG_ACCEPT)
		m_placeMarkMgr->GetApplyPlaceData(&m_placeMarkData, m_isAdd);
	else if (m_state == PLACEDLG_REJECT)
		m_placeMarkMgr->GetCancelPlaceData(&m_placeMarkData, m_isAdd);

	m_placeMarkMgr->SetResultTreeView(NULL);
}

void PlaceFolderDlg::accept()
{
	m_state = PLACEDLG_ACCEPT;
	g_app->GetMainWindow()->UpdatePlaceDlg(true);

	QDialog::accept();
}

void PlaceFolderDlg::reject()
{
	m_state = PLACEDLG_REJECT;
	g_app->GetMainWindow()->UpdatePlaceDlg(true);

	QDialog::reject();
}

void PlaceFolderDlg::changeEvent(QEvent *e)
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

void PlaceFolderDlg::showEvent(QShowEvent* event)
{
	if (m_isAdd)
	{
		m_placeMarkMgr->GetNewPlaceFolderData(&m_placeMarkData);
	}
	else
	{
		setWindowTitle(tr("Edit PlaceFolder"));
		m_placeMarkMgr->GetPropertyPlaceData(&m_placeMarkData, m_isAdd);
	}

	UpdateOutputInfo();
}

void PlaceFolderDlg::EnableOutputInfo()
{
	m_ui->lineViewLatitude->setEnabled(true);
	m_ui->lineViewLongitude->setEnabled(true);
	m_ui->lineViewRange->setEnabled(true);
	m_ui->lineViewHeading->setEnabled(true);
	m_ui->lineViewTilt->setEnabled(true);
}

void PlaceFolderDlg::DisableOutputInfo()
{
	m_ui->lineViewLatitude->setText(MSG_PLACEFOLDERDLG_NO);
	m_ui->lineViewLatitude->setDisabled(true);
	m_ui->lineViewLongitude->setText(MSG_PLACEFOLDERDLG_NO);
	m_ui->lineViewLongitude->setDisabled(true);
	m_ui->lineViewRange->setText(MSG_PLACEFOLDERDLG_NO);
	m_ui->lineViewRange->setDisabled(true);
	m_ui->lineViewHeading->setText(MSG_PLACEFOLDERDLG_NO);
	m_ui->lineViewHeading->setDisabled(true);
	m_ui->lineViewTilt->setText(MSG_PLACEFOLDERDLG_NO);
	m_ui->lineViewTilt->setDisabled(true);
}

void PlaceFolderDlg::UpdateOutputInfo()
{
	QString str = QString::fromUtf16(m_placeMarkData.name);
	m_ui->lineName->setText(str);

	str = QString::fromUtf16(m_placeMarkData.description);
	m_ui->textDescription->setPlainText(str);

	if (!m_placeMarkData.snapshot)
	{
		DisableOutputInfo();
		return;
	}

	EnableOutputInfo();

	switch(g_app->GetConfig()->m_llShowMode)
	{
		case OPTION_LL_10NOTATION:
			str = QString("%1%2").arg(m_placeMarkData.view_latitude, 10, 'f', 6).arg(QChar(0x00B0));
			m_ui->lineViewLatitude->setText(str);
			str = QString("%1%2").arg(m_placeMarkData.view_longitude, 10, 'f', 6).arg(QChar(0x00B0));
			m_ui->lineViewLongitude->setText(str);
			break;
		case OPTION_LL_DMS:
			str = gu_Degree2DMS(m_placeMarkData.view_latitude, GEO_TYPE_LATITUDE);
			m_ui->lineViewLatitude->setText(str);
			str = gu_Degree2DMS(m_placeMarkData.view_longitude, GEO_TYPE_LONGITUDE);
			m_ui->lineViewLongitude->setText(str);
			break;
		case OPTION_LL_10NOTATION_DM:
			str = gu_Degree2DM(m_placeMarkData.view_latitude, GEO_TYPE_LATITUDE);
			m_ui->lineViewLatitude->setText(str);
			str = gu_Degree2DM(m_placeMarkData.view_longitude, GEO_TYPE_LONGITUDE);
			m_ui->lineViewLongitude->setText(str);
			break;
		default:
			break;
	}

	unsigned int range = (unsigned int) (m_placeMarkData.view_range + 0.5);
	str.sprintf("%dm", range);
	m_ui->lineViewRange->setText(str);

	double val;
	if (m_placeMarkData.view_heading > -DELTA && m_placeMarkData.view_heading < DELTA)
		val = 0.0;
	else
		val = -m_placeMarkData.view_heading;
	str = QString("%1%2").arg(val, 0, 'f', 6).arg(QChar(0x00B0));
	m_ui->lineViewHeading->setText(str);

	if (m_placeMarkData.view_tilt > -DELTA && m_placeMarkData.view_tilt < DELTA)
		val = 0.0;
	else
		val = m_placeMarkData.view_tilt;
	str = QString("%1%2").arg(val, 0, 'f', 6).arg(QChar(0x00B0));
	m_ui->lineViewTilt->setText(str);
}

void PlaceFolderDlg::SetFocusOkButton()
{
	m_ui->okButton->setFocus(Qt::OtherFocusReason);
}

void PlaceFolderDlg::SetFolderName()
{
	QString str = m_ui->lineName->text();
	memset(&m_placeMarkData.name, 0, sizeof(m_placeMarkData.name));
	gu_qstring2ushort(str, m_placeMarkData.name, PMD_STR_MAX_SIZE);

	m_placeMarkMgr->GetEditPlaceData(&m_placeMarkData, m_state);
}

void PlaceFolderDlg::SetDescription()
{
	QString str = m_ui->textDescription->toPlainText();
	if (str.length() < PMD_STR_MAX_SIZE)
	{
		memset(&m_placeMarkData.description, 0, sizeof(m_placeMarkData.description));
		gu_qstring2ushort(str, m_placeMarkData.description, PMD_STR_MAX_SIZE);
	}
	else
	{
		m_ui->textDescription->setPlainText(m_ui->textDescription->toPlainText().left(
				m_ui->textDescription->toPlainText().length()-1));
		m_ui->textDescription->moveCursor(QTextCursor::End);
	}
}

void PlaceFolderDlg::SetViewLatitude()
{
	if (m_state == PLACEDLG_REJECT)
		return;

	QString str = m_ui->lineViewLatitude->text();
	double res = 0.0;

	switch(g_app->GetConfig()->m_llShowMode)
	{
		case OPTION_LL_10NOTATION:
			res = gu_Degree2Degree(str);
			break;
		case OPTION_LL_DMS:
			res = gu_DMS2Degree(str, GEO_TYPE_LATITUDE);
			break;
		case OPTION_LL_10NOTATION_DM:
			res = gu_DM2Degree(str, GEO_TYPE_LATITUDE);
			break;
		default:
			break;
	}

	if ((res + 90) >= -DELTA && (res - 90 <= DELTA))
	{
		m_placeMarkData.view_latitude = res;
		m_placeMarkMgr->GetEditPlaceData(&m_placeMarkData, PLACEDLG_VIEW_EDIT);
	}

	UpdateOutputInfo();
}

void PlaceFolderDlg::SetViewLongitude()
{
	if (m_state == PLACEDLG_REJECT)
		return;

	QString str = m_ui->lineViewLongitude->text();
	double res = 0.0;

	switch(g_app->GetConfig()->m_llShowMode)
	{
		case OPTION_LL_10NOTATION:
			res = gu_Degree2Degree(str);
			break;
		case OPTION_LL_DMS:
			res = gu_DMS2Degree(str, GEO_TYPE_LONGITUDE);
			break;
		case OPTION_LL_10NOTATION_DM:
			res = gu_DM2Degree(str, GEO_TYPE_LONGITUDE);
			break;
		default:
			break;
	}

	if ((res + 180 >= -DELTA) && (res - 180 <= DELTA))
	{
		m_placeMarkData.view_longitude = res;
		m_placeMarkMgr->GetEditPlaceData(&m_placeMarkData, PLACEDLG_VIEW_EDIT);
	}

	UpdateOutputInfo();
}

void PlaceFolderDlg::SetViewRange()
{
	if (m_state == PLACEDLG_REJECT)
		return;

	QString str = m_ui->lineViewRange->text();
	QRegExp rx("^\\s*(\\d+(\\.\\d*)?)\\s*[\\m]$");

	if (rx.exactMatch(str))
	{
		if (rx.cap(1).toDouble() > 63710098)
			m_placeMarkData.view_range = 63710098;
		else
			m_placeMarkData.view_range = rx.cap(1).toDouble();

		m_placeMarkMgr->GetEditPlaceData(&m_placeMarkData, PLACEDLG_VIEW_EDIT);
	}

	UpdateOutputInfo();
}

void PlaceFolderDlg::SetViewHeading()
{
	if (m_state == PLACEDLG_REJECT)
		return;

	QString str = m_ui->lineViewHeading->text();
	QRegExp rx("^\\s*([\\-+]?\\d+(\\.\\d*)?)\\s*[\\x00b0]$");

	if (rx.exactMatch(str))
	{
		if (rx.cap(1).toDouble() > 180.00)
			m_placeMarkData.view_heading = 180.00;
		else if (rx.cap(1).toDouble() < -180.00)
			m_placeMarkData.view_heading = -180.00;
		else
			m_placeMarkData.view_heading = -rx.cap(1).toDouble();

		m_placeMarkMgr->GetEditPlaceData(&m_placeMarkData, PLACEDLG_VIEW_EDIT);
	}

	UpdateOutputInfo();
}

void PlaceFolderDlg::SetViewTilt()
{
	if (m_state == PLACEDLG_REJECT)
		return;

	QString str = m_ui->lineViewTilt->text();
	QRegExp rx("^\\s*(\\d+(\\.\\d*)?)\\s*[\\x00b0]$");

	if (rx.exactMatch(str))
	{
		if (rx.cap(1).toDouble() > 90.00)
			m_placeMarkData.view_tilt = 90.00;
		else
			m_placeMarkData.view_tilt = rx.cap(1).toDouble();

		m_placeMarkMgr->GetEditPlaceData(&m_placeMarkData, PLACEDLG_VIEW_EDIT);
	}

	UpdateOutputInfo();
}

void PlaceFolderDlg::SnapshotView()
{
	m_placeMarkMgr->GetSnapshotPlaceData(&m_placeMarkData);
	UpdateOutputInfo();
}

void PlaceFolderDlg::ResetView()
{
	if (!m_placeMarkData.snapshot)
		return;

	m_placeMarkMgr->GetResetPlaceData(&m_placeMarkData);
	UpdateOutputInfo();
}
