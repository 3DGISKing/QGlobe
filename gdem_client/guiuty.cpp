#include "gdemclient.h"
#include "mainwindow.h"
#include "renderfrm.h"
#include "rendertext.h"
#include "msgdata.h"
#include "guiuty.h"
#include "gdemclientbase.h"
#include "config.h"
#include "GIS_Polygon.h"

int gu_MessageBox(QWidget *parent, const QString &title, const QString &text, unsigned int flag)
{
	QMessageBox msgBox(parent);
	QAbstractButton *okButton = NULL, *yesButton = NULL;
	QAbstractButton*noButton = NULL, *cancelButton = NULL;
	int res = 0;

	unsigned int id = MSGBOX_ID(flag);
	unsigned int ic = MSGBOX_ICON(flag);

	if (title.length() > 0)
		msgBox.setWindowTitle(title);
	if (text.length() > 0)
		msgBox.setText(text);

	if (id == 0)
		id = MSGBOX_IDOK;

	if (id & MSGBOX_IDOK)
		okButton = msgBox.addButton(QApplication::translate("GDemClient","Ok",0,QApplication::UnicodeUTF8), QMessageBox::AcceptRole);
	if (id & MSGBOX_IDYES)
		yesButton = msgBox.addButton(QApplication::translate("GDemClient","Yes",0,QApplication::UnicodeUTF8), QMessageBox::YesRole);
	if (id & MSGBOX_IDNO)
		noButton = msgBox.addButton(QApplication::translate("GDemClient","No",0,QApplication::UnicodeUTF8), QMessageBox::NoRole);
	if (id & MSGBOX_IDCANCEL)
		cancelButton = msgBox.addButton(QApplication::translate("GDemClient","Cancel",0,QApplication::UnicodeUTF8), QMessageBox::RejectRole);

	if (ic == MSGBOX_ICONQUESTION)
		msgBox.setIcon(QMessageBox::Question);
	else if (ic == MSGBOX_ICONINFO)
		msgBox.setIcon(QMessageBox::Information);
	else if (ic == MSGBOX_ICONWARNING)
		msgBox.setIcon(QMessageBox::Warning);
	else if (ic == MSGBOX_ICONCRITICAL)
		msgBox.setIcon(QMessageBox::Critical);

	msgBox.exec();

	if (msgBox.clickedButton() == okButton)
		res = MSGBOX_IDOK;
	else if (msgBox.clickedButton() == cancelButton)
		res = MSGBOX_IDCANCEL;
	else if (msgBox.clickedButton() == yesButton)
		res = MSGBOX_IDYES;
	else if (msgBox.clickedButton() == noButton)
		res = MSGBOX_IDNO;

	return res;
}

static bool decisionSign(QString *dms, int &sign, unsigned int type)
{
	sign = 1;
	if (type == GEO_TYPE_LONGITUDE)
	{
		if (dms->contains("E"))
		{
			dms->replace("E", "");
		}
		else if (dms->contains("W"))
		{
			dms->replace("W", "");
			sign = -1;
		}
		else
			return false;
	}
	else if (type == GEO_TYPE_LATITUDE)
	{
		if (dms->contains("N"))
		{
			dms->replace("N", "");
		}
		else if (dms->contains("S"))
		{
			dms->replace("S", "");
			sign = -1;
		}
		else
			return false;
	}
	return true;
}

static void degree2DMS(double degree, bool& sign, unsigned int& d, unsigned int& m, double& s)
{
	sign = true;
	if (degree < 0)
	{
		degree *= -1;
		sign = false;
	}

	d = (unsigned int) (degree + DELTA);
	m = (unsigned int)((degree - d) * 60  + DELTA);
	s = qAbs(((degree - d) * 3600) - (60 * m));
}

static void degree2DM(double degree, bool& sign, unsigned int& d, double& m)
{
	sign = true;
	if (degree < 0)
	{
		degree *= -1;
		sign = false;
	}

	d = (unsigned int) (degree + DELTA);
	m = (degree - d) * 60;
}

QString gu_Degree2DMS(double degree, unsigned int type)
{
	bool sign;
	unsigned int d,m;
	double s;

	degree2DMS(degree, sign, d, m, s);

	QString res;
	switch(type)
	{
	case GEO_TYPE_LONGITUDE:
		res = QString("%1%2%3\'%4\"%5")
			.arg(d, 3, 10, QLatin1Char(' '))
			.arg(QChar(0x00B0))
			.arg(m, 2, 10, QLatin1Char('0'))
			.arg(s, 5, 'f', 2, QLatin1Char('0'))
			.arg((sign?'E':'W'));
		break;
	case GEO_TYPE_LATITUDE:
		res = QString("%1%2%3\'%4\"%5")
			.arg(d, 3, 10, QLatin1Char(' '))
			.arg(QChar(0x00B0))
			.arg(m, 2, 10, QLatin1Char('0'))
			.arg(s, 5, 'f', 2, QLatin1Char('0'))
			.arg((sign?'N':'S'));
		break;
	default:
		break;
	}
	return res;
}

QString gu_Degree2DM(double degree, unsigned int type)
{
	bool sign;
	unsigned int d;
	double m;

	degree2DM(degree, sign, d, m);

	QString res;
	switch(type)
	{
	case GEO_TYPE_LONGITUDE:
		res = QString("%1%2%3\'%4")
			.arg(d, 3, 10, QLatin1Char(' '))
			.arg(QChar(0x00B0))
			.arg(m, 6, 'f', 3, QLatin1Char('0'))
			.arg((sign?'E':'W'));
		break;
	case GEO_TYPE_LATITUDE:
		res = QString("%1%2%3\'%4")
			.arg(d, 3, 10, QLatin1Char(' '))
			.arg(QChar(0x00B0))
			.arg(m, 6, 'f', 3, QLatin1Char('0'))
			.arg((sign?'N':'S'));
		break;
	default:
		break;
	}
	return res;
}

double gu_Degree2Degree(QString degree) //asd 2014.6.4
{
	QRegExp degRx("^\\s*([\\-+]?)\\s*(\\d+\\s*\\.?\\s*\\d*)\\D*$");
	if (degRx.exactMatch(degree)) {
		int sign = 1;
		if(degRx.cap(1) == "-")
			sign = -1;
		return (double)sign * degRx.cap(2).remove(QRegExp("\\s*")).toDouble();
	} else
		return GEO_NOT_TYPE;
}

double gu_DMS2Degree(QString dms, unsigned int type) //asd 2014.6.4
{
	QRegExp dmsRx("^\\s*([\\-+]?)\\s*(\\d+)\\D*(\\d*)\\D*(\\d*\\s*\\.?\\s*\\d*)\\D*$");
	if (dmsRx.exactMatch(dms)) {
		int sign = 1;
		if(dmsRx.cap(1) == "-")
			sign = -1;
		unsigned int degree = dmsRx.cap(2).toUInt();
		unsigned int minute = dmsRx.cap(3).toUInt();
		double second = dmsRx.cap(4).remove(QRegExp("\\s*")).toDouble();

		return (double)sign * ((double)degree + (double)minute/60 + second/3600);
	} else
		return GEO_NOT_TYPE;
}

double gu_DM2Degree(QString dm, unsigned int type) //asd 2014.6.4
{
	QRegExp dmRx("^\\s*([\\-+]?)\\s*(\\d+)\\D*(\\d*\\s*\\.?\\s*\\d*)\\D*$");
	if (dmRx.exactMatch(dm)) {
		int sign = 1;
		if(dmRx.cap(1) == "-")
			sign = -1;
		unsigned int degree = dmRx.cap(2).toUInt();
		double minute = dmRx.cap(3).remove(QRegExp("\\s*")).toDouble();

		return (double)sign * ((double)degree + minute/60);
	} else
		return GEO_NOT_TYPE;
}

#define DRAW_TEXT_BACKOPACITY	0.5
void gu_DrawText(QPainter *painter, int x, int y, const QString &text,
				 int thick, const QPen &bodyPen, const QPen &outPen, const double z)
{
	double opacity = DRAW_TEXT_BACKOPACITY;
	if (thick > 0)
		opacity = DRAW_TEXT_BACKOPACITY/(thick*2);

	painter->save();
	painter->setOpacity(opacity - opacity*z);
	painter->setBackgroundMode(Qt::TransparentMode);
	painter->setPen(outPen);

	for (int row = -thick; row <= thick; row ++)
	{
		for (int col = -thick; col <= thick; col ++)
		{
			if (row == 0 && col == 0)
				continue;
			painter->drawText(x+col, y+row, text);
		}
	}

	painter->setOpacity(1.0 - z);
	painter->setPen(bodyPen);
	painter->drawText(x, y, text);
	painter->restore();
}

void gu_DrawText(QPainter *painter, const QRect &rect, int flags, const QString &text,
				 int thick, const QPen &bodyPen, const QPen &outPen, const double z)
{
	QRect rcDest;

	double opacity = DRAW_TEXT_BACKOPACITY;
	if (thick > 1)
		opacity = DRAW_TEXT_BACKOPACITY/(thick*2);

	painter->save();
	painter->setOpacity(opacity-opacity*z);
	painter->setBackgroundMode(Qt::TransparentMode);
	painter->setPen(outPen);

	for (int row = -thick; row <= thick; row ++)
	{
		for (int col = -thick; col <= thick; col ++)
		{
			if (row == 0 && col == 0)
				continue;
			rcDest = rect;
			rcDest.moveTo(rect.left()+col, rect.top()+row);
			painter->drawText(rcDest, flags, text);
		}
	}

	painter->setOpacity(1.0 - z);
	painter->setPen(bodyPen);
	painter->drawText(rect, flags, text);
	painter->restore();
}

void gu_DrawTextEx(QPainter *painter, const QRect &rect, int flags, const QString &text,
				   int thick, const QPen &bodyPen, const QPen &outPen, const double z)
{
#ifndef Q_WS_WIN
	RenderFrame *renderFrame = g_app->GetMainWindow()->GetRenderFrame();
	RenderText *renderText = (renderFrame != NULL) ? renderFrame->GetRenderText() : NULL;
	if (renderText)
	{
		renderText->SetOpacity(1.0 - z, DRAW_TEXT_BACKOPACITY);
		renderText->DrawText(painter, rect, flags, text, thick, bodyPen, outPen);
	}
#else //Q_WS_WIN
	gu_DrawText(painter, rect, flags, text, thick, bodyPen, outPen, z);
#endif
}

void gu_MakeRenderData(PLACEMARK_DATA *pmd)
{
	pmd->longitude = degree2radian(pmd->longitude);
	pmd->latitude = degree2radian(pmd->latitude);
	pmd->view_longitude = degree2radian(pmd->view_longitude);
	pmd->view_latitude = degree2radian(pmd->view_latitude);
	pmd->view_heading = degree2radian(pmd->view_heading);
	pmd->view_tilt = degree2radian(pmd->view_tilt);
}

void gu_MakeLocalData(PLACEMARK_DATA *pmd)
{
	pmd->longitude = radian2degree(pmd->longitude);
	pmd->latitude = radian2degree(pmd->latitude);
	pmd->view_longitude = radian2degree(pmd->view_longitude);
	pmd->view_latitude = radian2degree(pmd->view_latitude);
	pmd->view_heading = radian2degree(pmd->view_heading);
	pmd->view_tilt = radian2degree(pmd->view_tilt);
}

void gu_wchar2ushort(wchar_t *src, ushort *dst, int len)
{
	int i = 0;
	while (src[i] > 0)
	{
		if (len > 0 && i >= len)
			break;
		dst[i] = (ushort)src[i];
		i ++;
	}
	dst[i] = 0;
}

void gu_ushort2wchar(ushort *src, wchar_t *dst, int len)
{
	int i = 0;
	while (src[i] > 0)
	{
		if (len > 0 && i >= len)
			break;
		dst[i] = ((wchar_t)src[i]) & 0x00ffff;
		i ++;
	}
	dst[i] = 0;
}

void gu_qstring2ushort(const QString &str, ushort *dst, int len)
{
	const ushort *name = str.utf16();
	int textLen = (len > 0) ? qMin(str.length(), len-1) : str.length();

	memcpy(dst, name, textLen * sizeof(ushort));
	dst[textLen] = 0;
}

QString gu_m2km(const double val, int type)
{
	QString res;

	if (type == GIS_UNIT_LENGTH)
	{
		if (val < 1000)
			res = QString("%1m").arg(val, 0, 'f', 2);
		else
			res = QString("%1km").arg(val/1000, 0, 'f', 2);
	}
	else if (type == GIS_UNIT_AREA)
	{
		if (val < 1000000)
			res = QString("%1m").arg(val, 0, 'f', 2);
		else
			res = QString("%1km").arg(val/1000000, 0, 'f', 2);
	}
	else if (type == GIS_UNIT_VOLUME)
	{
		if (val < 1000000000)
			res = QString("%1m").arg(val, 0, 'f', 2);
		else
			res = QString("%1km").arg(val/1000000000, 0, 'f', 2);
	}

	return res;
}

QString gu_msec2hms(int msec)
{
	int hour = msec/3600000;
	int minute = (msec % 3600000)/60000;
	int second = ((msec % 3600000) % 60000)/1000;

	QString res = QString("%1:%2:%3")
		.arg(hour, 2, 10, QLatin1Char('0'))
		.arg(minute, 2, 10, QLatin1Char('0'))
		.arg(second, 2, 10, QLatin1Char('0'));

	return res;
}

QString gu_sec2hms(double sec)
{
	QString res;
	int hour, minute, second;

	if (sec <= 60)
	{
		second = (int) sec;
		res = QString("%1%2").arg(second).arg(("second"));
		return res;
	}

	if (sec <= 3600)
	{
		minute = (int)sec/60;
		second = (int) (sec - minute*60);
		res = QString("%1%2 %3%4").arg(minute).arg(("minute"))
			.arg(second).arg(("second"));
		return res;
	}

	hour = (int) sec/3600;
	minute = (int)((sec - hour*3600)/60);
	second = (int)(sec - hour*3600 - minute*60);
	res = QString("%1%2 %3%4 %5%6").arg(hour).arg(("hour"))
		.arg(minute).arg(("minute"))
		.arg(second).arg(("second"));
	return res;
}

bool GetLookAtInfo(CGIS_Polygon* pNode, GDM_CAMERA_INFO *pLookAtInfo)
{
	if(pNode)
	{
		GDM_Coord3D coord;
		GDM_LOCATION ltLoc = {0}, rbLoc = {0};
		double ltBaseHeigt = 0 , rbBaseHeight = 0;
		if(g_app->GetConfig()->m_blTerrain)
		{
			ltBaseHeigt = gdm_getHeight(pNode->m_cBound.m_cMinPt.m_tX, pNode->m_cBound.m_cMinPt.m_tY);
			rbBaseHeight = gdm_getHeight(pNode->m_cBound.m_cMaxPt.m_tX, pNode->m_cBound.m_cMaxPt.m_tY);
		}
		switch(pNode->m_nHeightType)
		{
		case E_GISPOLYHEIGHT_RELATIVE_TO_GROUND:
			{
				ltBaseHeigt += pNode->m_dHeight;
				rbBaseHeight += pNode->m_dHeight;
			}
			break;
		case E_GISPOLYHEIGHT_ABSOLUTE:
			{
				ltBaseHeigt = rbBaseHeight = pNode->m_dHeight;
			}
			break;
		}

		ltLoc.m_dLongitude = pNode->m_cBound.m_cMinPt.m_tX;
		ltLoc.m_dLatitude = pNode->m_cBound.m_cMinPt.m_tY;
		ltLoc.m_dDist = ltBaseHeigt;
		rbLoc.m_dLongitude = pNode->m_cBound.m_cMaxPt.m_tX;
		rbLoc.m_dLatitude = pNode->m_cBound.m_cMaxPt.m_tY;
		rbLoc.m_dDist = rbBaseHeight;
		gdm_GetOptimzeCameraCoord(ltLoc, rbLoc, &coord);
		gdm_GetCoordInfo(&coord, pLookAtInfo);
		return true;
	}
	return false;
}

void GetValidPos(CGDM_Location3D* pFirstPos, CGDM_Location3D* pSecondPos, CGDM_Location3D* pDstPos, bool blClosed)
{
	if(pDstPos == NULL)
		return;
	double offset = 10.0;

	if(pFirstPos == NULL && pSecondPos == NULL)
	{
		GDM_Coord3D coord;
		CGDM_Vector3DF vt;
		CGDM_Point3DF pt;
		GDM_LOCATION loc;
		gdm_CameraGetCoord(g_pRender, &coord);
		vt = coord.m_z * (-1);
		if(GDM_FAIL == gdm_HitPoint(coord.m_org,vt, &pt))
		{
			pt = coord.m_org;
		}
		gdm_GetLocation(&pt, &loc);
		pDstPos->m_tX = loc.m_dLongitude;
		pDstPos->m_tY = loc.m_dLatitude;
		pDstPos->m_tZ = 0;
	}
	else if(pFirstPos != NULL && pSecondPos != NULL)
	{
		*pDstPos = (*pFirstPos + *pSecondPos) / 2;
		pDstPos->m_tZ = 0;
		if(*pFirstPos == *pSecondPos)
		{
			pDstPos->m_tX += offset / EARTH_RADIUS;
			pDstPos->m_tY += offset / EARTH_RADIUS;
		}
	}
	else
	{
		CGDM_Location3D* pos = NULL;
		pos = pFirstPos == NULL ? pSecondPos : pFirstPos;
		*pDstPos = *pos;
		pDstPos->m_tX += offset / EARTH_RADIUS;
		pDstPos->m_tY += offset / EARTH_RADIUS;
	}
}