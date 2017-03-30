#include "camerakeyframedlg.h"
#include "keyframe.h"
#include "Proxy.h"
#include "gdemclient.h"

CameraKeyFrameDlg::CameraKeyFrameDlg(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	m_CameraKeyFrame=new CameraKeyFrame;
}

CameraKeyFrameDlg::~CameraKeyFrameDlg()
{
	delete m_CameraKeyFrame;
}

void CameraKeyFrameDlg::onCapture()
{
	PLACEMARK_DATA pmd ;

	RenderProxy *render = g_app->GetRenderProxy();
	render->SendMessage(PM_SNAPSHOT_PLACEDATA, (PrxParam)&pmd);

	ui.lineEditLongitude->setText(QString::number(pmd.view_longitude,'f',15));
	ui.lineEditLatitude->setText(QString::number(pmd.view_latitude,'f',15));
	ui.lineEditAltitude->setText(QString::number(pmd.orgDist,'f',6));
	ui.lineEditHeight->setText(QString::number(pmd.view_range,'f',6));
	ui.lineEditHeading->setText(QString::number(pmd.view_heading,'f',10));
	ui.lineEditTilt->setText(QString::number(pmd.view_tilt,'f',10));
}

void CameraKeyFrameDlg::onOk()
{
	m_CameraKeyFrame->setName(ui.lineEditName->text());
	m_CameraKeyFrame->setLongitude(ui.lineEditLongitude->text().toDouble());
	m_CameraKeyFrame->setLatitude(ui.lineEditLatitude->text().toDouble());
	m_CameraKeyFrame->setAltitude(ui.lineEditAltitude->text().toDouble());
	m_CameraKeyFrame->setHeight(ui.lineEditHeight->text().toDouble());
	m_CameraKeyFrame->setHeading(ui.lineEditHeading->text().toDouble());
	m_CameraKeyFrame->setTilt(ui.lineEditTilt->text().toDouble());
	m_CameraKeyFrame->setGotoTime(ui.lineEditGoToTime->text().toInt());
	m_CameraKeyFrame->setStopTime(ui.lineEditStopTime->text().toInt());

	accept();
}

void CameraKeyFrameDlg::SetData(CameraKeyFrame* ckf)
{
	*m_CameraKeyFrame=*ckf;
	ui.lineEditName->setText(ckf->getName());
	ui.lineEditLongitude->setText(QString("%1").arg(ckf->getLongitude(),10,'f',15));
	ui.lineEditLatitude->setText(QString("%1").arg(ckf->getLatitude(),10,'f',15));
	ui.lineEditAltitude->setText(QString("%1").arg(ckf->getAltitude(),10,'f',6));
	ui.lineEditHeading->setText(QString("%1").arg(ckf->getHeading(),10,'f',6));
	ui.lineEditHeight->setText(QString("%1").arg(ckf->getHeight(),10,'f',10));
	ui.lineEditTilt->setText(QString("%1").arg(ckf->getTilt(),10,'f',10));
	ui.lineEditGoToTime->setText(QString("%1").arg(ckf->getGotoTime()));
	ui.lineEditStopTime->setText(QString("%1").arg(ckf->getStopTime()));
}
