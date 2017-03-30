#include "optiondlg.h"
#include "ui_optiondlg.h"
#include "gdemclient.h"
#include "gdemclientbase.h"
#include "config.h"
#include "mainwindow.h"
#include "guiuty.h"
#include "fontdialog.h"

class WorldMapScene :public QGraphicsScene
{
public:
	WorldMapScene(OptionDlg* dlg,double longi,double lat):QGraphicsScene()
	{
		optiondlg=dlg;
		longitude=longi;
		latitude=lat;
		setSceneRect(-180,-90,360,180);

		sun=addEllipse(longitude,-latitude,10,10,QPen(),QBrush(Qt::red));
	}

	void setSunPosition(double longi,double lat)
	{	
		QList<QGraphicsView*> views=this->views();

		int x=longi/360*views[0]->width();
		int y=-lat/180*views[0]->height();
		sun->setRect(x,y,10,10);
	}

	void setSunVisible(bool visible)
	{
		if(visible)
			sun->setVisible(true);
		else
			sun->setVisible(false);
	}
protected:
	virtual void	mouseMoveEvent ( QGraphicsSceneMouseEvent * mouseEvent )
	{
		if(mouseEvent->buttons() & Qt::LeftButton)
		{
			QPointF pos=mouseEvent->scenePos();	
			
			QList<QGraphicsView*> views=this->views();
		
			double tmpx=pos.x()*360/views[0]->width();
			double tmpy=pos.y()*180/views[0]->height();

			if(tmpx<-180 || tmpx>180)
				return;

			if(tmpy<-90 || tmpy>90)
				return ;

			sun->setRect(pos.x(),pos.y(),10,10);

			longitude=tmpx;
			latitude=tmpy;

			optiondlg->m_LightLongitude=longitude;
			optiondlg->m_LightLatitude=-latitude;
			optiondlg->Apply();
		}
	}

	virtual void	mousePressEvent ( QGraphicsSceneMouseEvent * mouseEvent )
	{
		if(mouseEvent->buttons() & Qt::LeftButton)
		{
			QPointF pos=mouseEvent->scenePos();	

			QList<QGraphicsView*> views=this->views();

			longitude=pos.x()*360/views[0]->width();
			latitude=pos.y()*180/views[0]->height();

			sun->setRect(pos.x(),pos.y(),10,10);

			optiondlg->m_LightLongitude=longitude;
			optiondlg->m_LightLatitude=-latitude;
			optiondlg->Apply();
		}
	}

	double Longitude(){return longitude; }
	double Latitude(){return latitude;}

private:
	OptionDlg *    optiondlg;

	QGraphicsEllipseItem* sun;
	double longitude,latitude;
};

OptionDlg::OptionDlg(QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::OptionDlg)
{
    m_ui->setupUi(this);

	setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint
				   | Qt::WindowCloseButtonHint);
	setAttribute(Qt::WA_DeleteOnClose);
	setFixedSize(size());

	InitDefaultValue();
	GetParameter();
	m_ui->optionProperty->setCurrentIndex(m_tabID);
	m_ui->actionOkButton->setFocus(Qt::OtherFocusReason);

	QString fileName = g_app->GetWorkDir()+"/res/scene/earth.jpg";
	QString style = QString("border-image: url(%1)").arg(fileName);
	m_ui->graphicsViewWorld->setStyleSheet(style);

	int width=m_ui->graphicsViewWorld->width();
	int height=m_ui->graphicsViewWorld->height();

	m_WorldMapScene=new WorldMapScene(this,m_LightLongitude*width/360,m_LightLatitude*height/180);
	m_ui->graphicsViewWorld->setScene(m_WorldMapScene);

	UpdateOutputInfo();

	connect(m_ui->optionProperty, SIGNAL(currentChanged(int)), this, SLOT(SetFocusOkButton()));
	connect(m_ui->actionToolTipCheck, SIGNAL(clicked()), this, SLOT(SetToolTipChecked()));
	connect(m_ui->actionShowGISInfo, SIGNAL(clicked()), this, SLOT(SetGISInfoChecked()));
	connect(m_ui->actionSmallSize, SIGNAL(clicked()), this, SLOT(SetLableSize()));
	connect(m_ui->actionMediumSize, SIGNAL(clicked()), this, SLOT(SetLableSize()));
	connect(m_ui->actionLargeSize, SIGNAL(clicked()), this, SLOT(SetLableSize()));
	connect(m_ui->action10Notation, SIGNAL(clicked()), this, SLOT(SetLLShowMode()));
	connect(m_ui->actionDMS, SIGNAL(clicked()), this, SLOT(SetLLShowMode()));
	connect(m_ui->action10NotationDM, SIGNAL(clicked()), this, SLOT(SetLLShowMode()));
	connect(m_ui->actionFeetMile, SIGNAL(clicked()), this, SLOT(SetDistShowMode()));
	connect(m_ui->actionMeterKilo, SIGNAL(clicked()), this, SLOT(SetDistShowMode()));
//	connect(m_ui->actionMainFont, SIGNAL(clicked()), this, SLOT(SetMainFont()));
	connect(m_ui->actionTopoQualitySlider, SIGNAL(valueChanged(int)), this, SLOT(SetTopoQuality(int)));
	connect(m_ui->actionHeightMagnify, SIGNAL(editingFinished()), this, SLOT(SetHeightMagnify()));

	connect(m_ui->actionMemoryCacheSize, SIGNAL(editingFinished()), this, SLOT(SetMemoryCacheSize()));
	connect(m_ui->actionDiskCacheSize, SIGNAL(editingFinished()), this, SLOT(SetDiskCacheSize()));
	connect(m_ui->actionDeleteMemoryCache, SIGNAL(clicked()), this, SLOT(DeleteMemoryCache()));
	connect(m_ui->actionDeleteDiskCache, SIGNAL(clicked()), this, SLOT(DeleteDiskCache()));

	connect(m_ui->cameraTiltLineEdit, SIGNAL(editingFinished()), this, SLOT(SetTourTiltAngle()));
	connect(m_ui->cameraTiltSlider, SIGNAL(valueChanged(int)), this, SLOT(SetTourTiltAngle(int)));
	connect(m_ui->cameraRangeLineEdit, SIGNAL(editingFinished()), this, SLOT(SetTourRange()));
	connect(m_ui->cameraRangeSlider, SIGNAL(valueChanged(int)), this, SLOT(SetTourRange(int)));
	connect(m_ui->speedLineEdit, SIGNAL(editingFinished()), this, SLOT(SetTourSpeed()));
	connect(m_ui->speedSlider, SIGNAL(valueChanged(int)), this, SLOT(SetTourSpeed(int)));
	connect(m_ui->actionQuickMoveSlider, SIGNAL(valueChanged(int)), this, SLOT(SetMoveSpeed(int)));
	connect(m_ui->actionQuickMoveSpeed, SIGNAL(editingFinished()), this, SLOT(SetMoveSpeed()));
	connect(m_ui->actionMouseWheelSlider, SIGNAL(valueChanged(int)), this, SLOT(SetWheelSpeed(int)));
	connect(m_ui->actionMouseWheelDirection, SIGNAL(clicked()), this, SLOT(SetWheelDirection()));

	connect(m_ui->actionDefaultButton, SIGNAL(clicked()), this, SLOT(SetDefaultValue()));
	connect(m_ui->actionOkButton, SIGNAL(clicked()), this, SLOT(accept()));
	connect(m_ui->actionCancelButton, SIGNAL(clicked()), this, SLOT(reject()));
	connect(m_ui->actionApplyButton, SIGNAL(clicked()), this, SLOT(Apply()));

	connect(m_ui->lineEditSunLongitude, SIGNAL(editingFinished()), this, SLOT(SetLightLongitude()));
	connect(m_ui->lineEditSunLatitude, SIGNAL(editingFinished()), this, SLOT(SetLightLatitude()));
	connect(m_ui->lineEditSunHeight, SIGNAL(editingFinished()), this, SLOT(SetLightHeight()));

	connect(m_ui->pushButtonLightColor,SIGNAL(clicked()),this,SLOT(SetLightColor()));
	connect(m_ui->pushButtonAmbientColor,SIGNAL(clicked()),this,SLOT(SetLightAmbientColor()));

	connect(m_ui->doubleSpinBoxLightConstantAttenuation,SIGNAL(valueChanged(double)),this,SLOT(SetLightAttenuationConstant(double)));
	connect(m_ui->checkBoxEnableLighting,SIGNAL(stateChanged(int)),this,SLOT(SetEnableLighing(int)));

	connect(m_ui->cameraFOVHorizontalSlider, SIGNAL(valueChanged(int)), this, SLOT(SetCameraFOV(int)));

}

OptionDlg::~OptionDlg()
{
    delete m_ui;
	delete m_WorldMapScene;
}

void OptionDlg::changeEvent(QEvent *e)
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

void OptionDlg::InitDefaultValue()
{
	m_tabID = OPTION_VIEW_TAB;

	m_toolTipChecked = true;
	m_gisInfoChecked = true;
	m_lableSize = OPTION_LABLE_MEDIUM;
	m_llShowMode = OPTION_LL_DMS;
	m_distShowMode = OPTION_DISTANCE_METER_KILO;
	m_mainFont = QFont("PRK P Gothic", 10);
	m_topoQuality = 50;
	m_heightMagnify = 1.0;

	m_memCacheSize = 300;
	m_diskCacheSize = 500;

	m_tourTiltAngle = OPTION_TOUR_TILT_DEFAULT;
	m_tourRange = OPTION_TOUR_RANGE_DEFAULT;
	m_tourSpeed = OPTION_TOUR_SPEED_DEFAULT;
	m_moveSpeed = 0.10;
	m_wheelSpeed = OPTION_MOUSE_WHELL_SPEED;
	m_wheelDirection = false;
}

void OptionDlg::GetParameter()
{
	Config *conf = g_app->GetConfig();

	m_tabID = conf->m_optionTabID;
	m_toolTipChecked = conf->m_toolTipChecked;
	m_gisInfoChecked = conf->m_gisInfoChecked;
	m_lableSize = conf->m_lableSize;
	m_llShowMode = conf->m_llShowMode;
	m_distShowMode = conf->m_distShowMode;
	m_mainFont = conf->m_mainFont;
	m_topoQuality = conf->m_topoQuality;
	m_heightMagnify = conf->m_heightMagnify;

	m_memCacheSize = conf->m_memCacheSize;
	m_diskCacheSize = conf->m_diskCacheSize;

	m_tourTiltAngle = conf->m_tourTiltAngle;
	m_tourRange = conf->m_tourRange;
	m_tourSpeed = conf->m_tourSpeed;
	m_moveSpeed = conf->m_moveSpeed;
	m_wheelSpeed = conf->m_wheelSpeed;
	m_wheelDirection = conf->m_wheelDirection;

	//Graphics
	m_LightingEnabled=conf->m_SunLightEnabled;
	m_LightLongitude=conf->m_SunLightLongitude;
	m_LightLatitude =conf->m_SunLightLatitude;
	m_LightHeight=conf->m_SunLightHeight;

	m_LightColor=conf->m_SunLightColor;
	m_AmbientLightColor=conf->m_AmbientLightColor;
	m_LightAttenuation=conf->m_LightAttenuation;
	m_CameraFOV=conf->m_CameraFOV;
}

void OptionDlg::SetParameter()
{
	Config *conf = g_app->GetConfig();

	conf->m_optionTabID = m_ui->optionProperty->currentIndex();
	conf->m_toolTipChecked = m_toolTipChecked;
	conf->m_gisInfoChecked = m_gisInfoChecked;
	conf->m_lableSize = m_lableSize;
	conf->m_llShowMode = m_llShowMode;
	conf->m_distShowMode = m_distShowMode;
	conf->m_mainFont = m_mainFont;
	conf->m_topoQuality = m_topoQuality;
	conf->m_heightMagnify = m_heightMagnify;

	conf->m_memCacheSize = m_memCacheSize;
	conf->m_diskCacheSize = m_diskCacheSize;

	conf->m_tourTiltAngle = m_tourTiltAngle;
	conf->m_tourRange = m_tourRange;
	conf->m_tourSpeed = m_tourSpeed;
	conf->m_moveSpeed = m_moveSpeed;
	conf->m_wheelSpeed = m_wheelSpeed;
	conf->m_wheelDirection = m_wheelDirection;

	//Graphics
	conf->m_SunLightEnabled=m_LightingEnabled;
	conf->m_SunLightLongitude=m_LightLongitude;
	conf->m_SunLightLatitude=m_LightLatitude;
	conf->m_SunLightHeight=m_LightHeight;
	conf->m_SunLightColor=m_LightColor;
	conf->m_AmbientLightColor=m_AmbientLightColor;
	conf->m_LightAttenuation=m_LightAttenuation;

	conf->m_CameraFOV       =m_CameraFOV;
}

void OptionDlg::UpdateOutputInfo()
{
	m_ui->actionToolTipCheck->setChecked(m_toolTipChecked);
	m_ui->actionShowGISInfo->setChecked(m_gisInfoChecked);

	if (m_lableSize == OPTION_LABLE_SMALL)
		m_ui->actionSmallSize->setChecked(true);
	else if (m_lableSize == OPTION_LABLE_MEDIUM)
		m_ui->actionMediumSize->setChecked(true);
	else
		m_ui->actionLargeSize->setChecked(true);

	if (m_llShowMode == OPTION_LL_10NOTATION)
		m_ui->action10Notation->setChecked(true);
	else if (m_llShowMode == OPTION_LL_DMS)
		m_ui->actionDMS->setChecked(true);
	else
		m_ui->action10NotationDM->setChecked(true);

	if (m_distShowMode == OPTION_DISTANCE_FEET_MILE)
		m_ui->actionFeetMile->setChecked(true);
	else
		m_ui->actionMeterKilo->setChecked(true);

	m_ui->actionTopoQualitySlider->setValue(m_topoQuality - 1);

	QString str;
	str.sprintf("%.2f", m_heightMagnify);
	m_ui->actionHeightMagnify->setText(str);

	str.clear();
	str.sprintf("%d", m_memCacheSize);
	m_ui->actionMemoryCacheSize->setText(str);

	str.clear();
	str.sprintf("%d", m_diskCacheSize);
	m_ui->actionDiskCacheSize->setText(str);

	str.clear();
	str.sprintf("%.1f", m_tourTiltAngle);
	m_ui->cameraTiltLineEdit->setText(str);
	int move = (int) (m_tourTiltAngle * 10);
	m_ui->cameraTiltSlider->setValue(move);

	str.clear();
	str.sprintf("%.1f", m_tourRange);
	m_ui->cameraRangeLineEdit->setText(str);
	move = (int) (m_tourRange * 10);
	m_ui->cameraRangeSlider->setValue(move);

	str.clear();
	str.sprintf("%.1f", m_tourSpeed);
	m_ui->speedLineEdit->setText(str);
	move = (int) (m_tourSpeed * 10);
	m_ui->speedSlider->setValue(move);

	move = (int)(m_moveSpeed * 20) - 1;
	m_ui->actionQuickMoveSlider->setValue(move);
	str.clear();
	str.sprintf("%.2f", m_moveSpeed);
	m_ui->actionQuickMoveSpeed->setText(str);

	m_ui->actionMouseWheelSlider->setValue(m_wheelSpeed - 1);
	m_ui->actionMouseWheelDirection->setChecked(m_wheelDirection);

	if(m_LightingEnabled)
	{
		m_ui->checkBoxEnableLighting->setCheckState(Qt::Checked);
		m_WorldMapScene->setSunVisible(true);
		m_ui->groupBoxSunPosition->setEnabled(true);
		m_ui->groupBoxSunProperty->setEnabled(true);
	}
	else
	{
		m_ui->checkBoxEnableLighting->setCheckState(Qt::Unchecked);
		m_WorldMapScene->setSunVisible(false);
		m_ui->groupBoxSunPosition->setEnabled(false);
		m_ui->groupBoxSunProperty->setEnabled(false);
	}

	m_ui->doubleSpinBoxLightConstantAttenuation->setValue(m_LightAttenuation);

	m_ui->lineEditSunLongitude->setText(QString("%1").arg(m_LightLongitude));
	m_ui->lineEditSunLatitude->setText(QString("%1").arg(m_LightLatitude));
	m_ui->lineEditSunHeight->setText(QString("%1").arg(m_LightHeight));
	
	QString sheet = QString("background-color: %1").arg(m_LightColor.name());
	m_ui->pushButtonLightColor->setStyleSheet(sheet);

	sheet = QString("background-color: %1").arg(m_AmbientLightColor.name());
	m_ui->pushButtonAmbientColor->setStyleSheet(sheet);

	m_ui->cameraFOVHorizontalSlider->setValue(m_CameraFOV);

}

void OptionDlg::SendFontInfo()
{
	FontInfo font;
	memset(&font, 0, sizeof(font));

	gu_qstring2ushort(m_mainFont.family(), font.family, FONT_FAMILY_MAX_SIZE);
	font.size = m_mainFont.pointSize();

	if (m_mainFont.italic())
		font.flag |= FONT_ITALIC;
	if (m_mainFont.bold())
		font.flag |= FONT_BOLD;
	if (m_mainFont.strikeOut())
		font.flag |= FONT_STRIKEOUT;
	if (m_mainFont.underline())
		font.flag |= FONT_UNDERLINE;

	//FontMng *fontMng = g_app->GetFontMng();
	//if (fontMng)
	//	fontMng->GetFontName(m_mainFont, font.fileName, FONT_NAME_MAX_SIZE);

	g_app->GetRenderProxy()->SendMessage(PM_CHANGE_FONT, (PrxParam)&font);
}

void OptionDlg::accept()
{
	Apply();

	QDialog::accept();
}

void OptionDlg::reject()
{
	QDialog::reject();
}

void OptionDlg::SetFocusOkButton()
{
	m_ui->actionOkButton->setFocus(Qt::OtherFocusReason);
}

void OptionDlg::SetToolTipChecked()
{
	m_toolTipChecked = m_ui->actionToolTipCheck->isChecked();
}

void OptionDlg::SetGISInfoChecked()
{
	m_gisInfoChecked = m_ui->actionShowGISInfo->isChecked();
}

void OptionDlg::SetLableSize()
{
	if (m_ui->actionSmallSize->isChecked())
		m_lableSize = OPTION_LABLE_SMALL;
	if (m_ui->actionMediumSize->isChecked())
		m_lableSize = OPTION_LABLE_MEDIUM;
	if (m_ui->actionLargeSize->isChecked())
		m_lableSize = OPTION_LABLE_LARGE;
}

void OptionDlg::SetLLShowMode()
{
	if (m_ui->action10Notation->isChecked())
		m_llShowMode = OPTION_LL_10NOTATION;
	if (m_ui->actionDMS->isChecked())
		m_llShowMode = OPTION_LL_DMS;
	if (m_ui->action10NotationDM->isChecked())
		m_llShowMode = OPTION_LL_10NOTATION_DM;
}

void OptionDlg::SetDistShowMode()
{
	if (m_ui->actionFeetMile->isChecked())
		m_distShowMode = OPTION_DISTANCE_FEET_MILE;
	if (m_ui->actionMeterKilo->isChecked())
		m_distShowMode = OPTION_DISTANCE_METER_KILO;
}
//
//void OptionDlg::SetMainFont()
//{
//	bool ok;
//	QFont font = QFontDialog::getFont(&ok, m_mainFont, this);
//	if (ok)
//		m_mainFont = font;
//}

void OptionDlg::SetTopoQuality(int value)
{
	m_topoQuality = value + 1;
}

void OptionDlg::SetHeightMagnify(bool update)
{
	double value = m_ui->actionHeightMagnify->text().toDouble();

	if (value < 0.5)
		m_heightMagnify = 1.0;
	else if (value > 3.0)
		m_heightMagnify = 3.0;
	else
		m_heightMagnify = value;

/*
	QString str = m_ui->actionHeightMagnify->text();
	QRegExp rx("^\\s*(\\d+(\\.\\d*))$");

	if (rx.exactMatch(str))
	{
		if (rx.cap(1).toDouble() < 0.5)
			m_heightMagnify = 0.5;
		else if (rx.cap(1).toDouble() > 3.0)
			m_heightMagnify = 3.0;
		else
			m_heightMagnify = rx.cap(1).toDouble();
	}*/

	if (update)
		UpdateOutputInfo();
}

void OptionDlg::SetMemoryCacheSize(bool update)
{
	QString str = m_ui->actionMemoryCacheSize->text();
	int size = str.toInt();

	if (size < 20)
		m_memCacheSize = 20;
	else if (size > 500)
		m_memCacheSize = 500;
	else
		m_memCacheSize = size;

	if (update)
		UpdateOutputInfo();
}

void OptionDlg::SetDiskCacheSize(bool update)
{
	QString str = m_ui->actionDiskCacheSize->text();
	int size = str.toInt();

	if (size < 0)
		m_diskCacheSize = 0;
	else if (size > 2000)
		m_diskCacheSize = 2000;
	else
		m_diskCacheSize = size;

	if (update)
		UpdateOutputInfo();
}

void OptionDlg::DeleteMemoryCache()
{
	g_app->GetRenderProxy()->SendMessage(PM_CLEAR_MEMORYCACHE);
}

void OptionDlg::DeleteDiskCache()
{
	g_app->GetRenderProxy()->SendMessage(PM_CLEAR_DISKCACHE);
}

void OptionDlg::SetTourTiltAngle(int value)
{
	if (value == -1)
	{
		QString str = m_ui->cameraTiltLineEdit->text();
		double angle = str.toDouble();

		if (angle <  OPTION_TOUR_TILT_MIN)
			m_tourTiltAngle = OPTION_TOUR_TILT_MIN;
		else if (angle > OPTION_TOUR_TILT_MAX)
			m_tourTiltAngle = OPTION_TOUR_TILT_MAX;
		else
			m_tourTiltAngle = angle;
	}
	else
	{
		m_tourTiltAngle = (double) (value) / 10;
	}

	UpdateOutputInfo();
}

void OptionDlg::SetTourRange(int value)
{
	if (value == -1)
	{
		QString str = m_ui->cameraRangeLineEdit->text();
		double range = str.toDouble();

		if (range <  OPTION_TOUR_RANGE_MIN)
			m_tourRange = OPTION_TOUR_RANGE_MIN;
		else if (range > OPTION_TOUR_RANGE_MAX)
			m_tourRange = OPTION_TOUR_RANGE_MAX;
		else
			m_tourRange = range;
	}
	else
	{
		m_tourRange = (double) (value) / 10;
	}

	UpdateOutputInfo();
}

void OptionDlg::SetTourSpeed(int value)
{
	if (value == -1)
	{
		QString str = m_ui->speedLineEdit->text();
		double speed = str.toDouble();

		if (speed <  OPTION_TOUR_SPEED_MIN)
			m_tourSpeed = OPTION_TOUR_SPEED_MIN;
		else if (speed > OPTION_TOUR_SPEED_MAX)
			m_tourSpeed = OPTION_TOUR_SPEED_MAX;
		else
			m_tourSpeed = speed;
	}
	else
	{
		m_tourSpeed = (double) (value) / 10;
	}

	UpdateOutputInfo();
}

void OptionDlg::SetMoveSpeed(int value)
{
	if (value == -1)
	{
		QString str = m_ui->actionQuickMoveSpeed->text();
		double speed = str.toDouble();

		if (speed <  0.05)
			m_moveSpeed = 0.05;
		else if (speed > 5.0)
			m_moveSpeed = 5.0;
		else
			m_moveSpeed = speed;
	}
	else
	{
		m_moveSpeed = (double) (value + 1) / 20;
		if (m_moveSpeed < 0.05)
			m_moveSpeed = 0.05;
	}

	UpdateOutputInfo();
}

void OptionDlg::SetWheelSpeed(int value)
{
	m_wheelSpeed = value + 1;
}

void OptionDlg::SetWheelDirection()
{
	if (m_ui->actionMouseWheelDirection->isChecked())
		m_wheelDirection = true;
	else
		m_wheelDirection = false;
}

void OptionDlg::SetDefaultValue()
{
	InitDefaultValue();
	UpdateOutputInfo();
}

void OptionDlg::Apply()
{
	SetHeightMagnify(false);
	SetMemoryCacheSize(false);
	SetDiskCacheSize(false);
	SetTourTiltAngle();
	SetTourRange();
	SetTourSpeed();
	SetMoveSpeed();

	SetParameter();
	g_app->GetMainWindow()->UpdateToolTip();
	//SendFontInfo();

	g_app->UpdateOptionInfo();
}

void OptionDlg::SetLightColor()
{
	m_LightColor = QColorDialog::getColor(m_LightColor);
	UpdateOutputInfo();
	Apply();
}

void OptionDlg::SetLightAmbientColor()
{
	m_AmbientLightColor = QColorDialog::getColor(m_AmbientLightColor);
	UpdateOutputInfo();
	Apply();
}

void OptionDlg::SetLightLongitude()
{
	QString str=m_ui->lineEditSunLongitude->text();

	bool ok;

	double tmp=str.toDouble(&ok);

	if(!ok)
	{
		gu_MessageBox(NULL,tr("Error"),tr("Correctly Input!"),MSGBOX_IDOK);
		m_ui->lineEditSunLongitude->selectAll();
		m_ui->lineEditSunLongitude->setFocus();
		return;
	}
	
	if(tmp<-180 || tmp>180)
	{
		gu_MessageBox(NULL,tr("Error"),tr("Invalid Longitude!"),MSGBOX_IDOK);
		m_ui->lineEditSunLongitude->selectAll();
		m_ui->lineEditSunLongitude->setFocus();
		return;
	}

	m_LightLongitude=tmp;
	m_WorldMapScene->setSunPosition(m_LightLongitude,m_LightLatitude);
	Apply();
}

void OptionDlg::SetLightLatitude()
{
	QString str=m_ui->lineEditSunLatitude->text();

	bool ok;

	double tmp=str.toDouble(&ok);

	if(!ok)
	{
		gu_MessageBox(NULL,tr("Error"),tr("Correctly Input!"),MSGBOX_IDOK);
		m_ui->lineEditSunLatitude->selectAll();
		m_ui->lineEditSunLatitude->setFocus();
		return;
	}

	if(tmp<-90 || tmp>90)
	{
		gu_MessageBox(NULL,tr("Error"),tr("Invalid Latitude!"),MSGBOX_IDOK);
		m_ui->lineEditSunLatitude->selectAll();
		m_ui->lineEditSunLatitude->setFocus();
		return;
	}

	m_LightLatitude=tmp;
	m_WorldMapScene->setSunPosition(m_LightLongitude,m_LightLatitude);
	Apply();
}

void OptionDlg::SetLightHeight()
{
	QString str=m_ui->lineEditSunHeight->text();

	bool ok;

	double tmp=str.toDouble(&ok);

	if(!ok)
	{
		gu_MessageBox(NULL,tr("Error"),tr("Correctly Input!"),MSGBOX_IDOK);
		m_ui->lineEditSunHeight->selectAll();
		m_ui->lineEditSunHeight->setFocus();
		return;
	}

	m_LightHeight=tmp;
	Apply();
}

void OptionDlg::SetLightAttenuationConstant(double c)
{
	m_LightAttenuation=c;
	Apply();
}

void OptionDlg::SetEnableLighing(int state)
{
	if(state==Qt::Checked)
		m_LightingEnabled=true;
	else
		m_LightingEnabled=false;

	UpdateOutputInfo();
	Apply();
}

void OptionDlg::SetCameraFOV(int val)
{
	m_CameraFOV=val;

	UpdateOutputInfo();
	Apply();
}