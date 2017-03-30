#include "stable.h"
#include "gotodialog.h"
#include "gdemclient.h"
#include "guiuty.h"

GoToDialog::GoToDialog(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	_longitude=0.0;
	_latitude=0.0;
	_height=1000;

	QString val;
	val.setNum(_longitude);
	ui.lineEditLongitude->setText(val);
	val.setNum(_latitude);
	ui.lineEditLatitude->setText(val);
	val.setNum(_height);
	ui.lineEditHeight->setText(val);

	connect(ui.pushButtonClose,SIGNAL(clicked()),this,SLOT(onClickedPushButtonClose()));
	connect(ui.pushButtonGoto,SIGNAL(clicked()),this,SLOT(onClickedPushButtonGoTo()));
	connect(ui.lineEditLongitude,SIGNAL(editingFinished()),this,SLOT(onEditingFinishedLongitude()));
	connect(ui.lineEditLatitude,SIGNAL(editingFinished()),this,SLOT(onEditingFinishedLatitude()));
	connect(ui.lineEditHeight,SIGNAL(editingFinished()),this,SLOT(onEditingFinishedHeight()));

	QVariant v;

	v.setValue(DegreeMinuteSecond);
	ui.comboBoxUnit->addItem(tr("Degrees, Minutes, Seconds"),v);

	v.setValue(DegreeDecimalMinute);
	ui.comboBoxUnit->addItem(tr("Degrees, Decimal Minutes"),v);
	
	v.setValue(DecimalDegree);
	ui.comboBoxUnit->addItem(tr("Decimal Degrees"),v);
}

GoToDialog::~GoToDialog()
{

}

void GoToDialog::onClickedPushButtonGoTo()
{
	GDM_CAMERA_INFO cameraInfo;	
	memset(&cameraInfo , 0 , sizeof(GDM_CAMERA_INFO));
	cameraInfo.m_loc.m_dDist=_height;
	cameraInfo.m_loc.m_dLongitude=_longitude;
	cameraInfo.m_loc.m_dLatitude=_latitude;

	g_app->GetRenderProxy()->GetRenderSrv()->m_pCameraController->AutoMoveTo(0, cameraInfo, 4000);
}

void GoToDialog::onClickedPushButtonClose()
{
	this->hide();
}

void GoToDialog::onEditingFinishedHeight()
{
	QString val=ui.lineEditHeight->text();
	
	bool ok;

	double d=val.toDouble(&ok);
	
	if(ok && d >=0.0)
		_height=d;
	else
		gu_MessageBox(this,tr("Error"),tr("Correctly input!"));
}

void GoToDialog::onEditingFinishedLongitude()
{
	double val;
	QString text=ui.lineEditLongitude->text();

	if(getGeographicValue(text,GEO_TYPE_LONGITUDE,val))
	{
		_longitude=val;
	}
	else
	{	
		gu_MessageBox(this,tr("Error"),tr("Correctly input!"));
		ui.lineEditLongitude->selectAll();
		ui.lineEditLongitude->setFocus(Qt::MouseFocusReason);
	}
}

void GoToDialog::onEditingFinishedLatitude()
{
	double val;
	QString text=ui.lineEditLatitude->text();

	if(getGeographicValue(text,GEO_TYPE_LATITUDE,val))
	{
		_latitude=val;
	}
	else
	{
		gu_MessageBox(this,tr("Error"),tr("Correctly input!"));
		ui.lineEditLatitude->selectAll();
		ui.lineEditLatitude->setFocus(Qt::MouseFocusReason);
	}
}

bool GoToDialog::getGeographicValue(QString str,unsigned int type,double& val)
{
	QVariant v=ui.comboBoxUnit->itemData(ui.comboBoxUnit->currentIndex());

	GoToDialog::Unit unit=v.value<GoToDialog::Unit>();
	
	double res=GEO_NOT_TYPE;
	switch(unit)
	{
	case DecimalDegree:
		res = gu_Degree2Degree(str);
		break;
	case DegreeMinuteSecond:
		res = gu_DMS2Degree(str, type);
		break;
	case DegreeDecimalMinute:
		res = gu_DM2Degree(str, type);
		break;
	default:
		break;
	}

	if(res==GEO_NOT_TYPE)
		return false;

	if ((res + 180 >= -DELTA && (res - 180) <= DELTA))
	{
		val=res*gdm_DEGTORAD;
		return true;
	}
	return false;
}