#ifndef GUIUTY_H
#define GUIUTY_H

#include <QWidget>
#include <QString>

#include "../gdem_common/gdemclientbase.h"
#include "../gdem_libmain/GDM_Interface.h"

class CGIS_Polygon;


//for messge box
#define MSGBOX_IDOK			0x0001
#define MSGBOX_IDCANCEL		0x0002
#define MSGBOX_IDOKCANCEL	(MSGBOX_IDOK | MSGBOX_IDCANCEL)
#define MSGBOX_IDYES		0x0004
#define MSGBOX_IDNO			0x0008
#define MSGBOX_IDYESNO		(MSGBOX_IDYES | MSGBOX_IDNO)

#define MSGBOX_ICONQUESTION	0x0100
#define MSGBOX_ICONINFO		0x0200
#define MSGBOX_ICONWARNING	0x0400
#define MSGBOX_ICONCRITICAL	0x0800

#define MSGBOX_ID(f)		(f & 0x00ff)
#define MSGBOX_ICON(f)		(f & 0xff00)

int gu_MessageBox(QWidget *parent, const QString &title, const QString &text, unsigned int flag = 0);

//for convert between second data and DMS format
#define DELTA				(1E-6)
#define GEO_NOT_TYPE		-500
#define GEO_TYPE_LONGITUDE	1
#define GEO_TYPE_LATITUDE	2

QString gu_Degree2DMS(double degree, unsigned int type);
QString gu_Degree2DM(double degree, unsigned int type);
double gu_Degree2Degree(QString degree);
double gu_DMS2Degree(QString dms, unsigned int type);
double gu_DM2Degree(QString dm, unsigned int type);

//for drawing a outline text
void gu_DrawText(QPainter *painter, int x, int y, const QString &text,
						int thick, const QPen &bodyPen, const QPen &outPen, const double z = 0.0);
void gu_DrawText(QPainter *painter, const QRect &rect, int flags, const QString &text,
						int thick, const QPen &bodyPen, const QPen &outPen, const double z = 0.0);

void gu_DrawTextEx(QPainter *painter, const QRect &rect, int flags, const QString &text,
						int thick, const QPen &bodyPen, const QPen &outPen, const double z = 0.0);

//for convert PLACEMARK_DATA to radian or degree
void gu_MakeRenderData(PLACEMARK_DATA *pmd);
void gu_MakeLocalData(PLACEMARK_DATA *pmd);

//wchar & ushort
void gu_wchar2ushort(wchar_t *src, ushort *dst, int len);
void gu_ushort2wchar(ushort *src, wchar_t *dst, int len);

//QString to ushort
void gu_qstring2ushort(const QString &str, ushort *dst, int len);

//m to km
enum GIS_UNIT_TYPE { GIS_UNIT_LENGTH = 1, GIS_UNIT_AREA, GIS_UNIT_VOLUME };

QString gu_m2km(const double val, int type);

//milli sec to HH:MM:SS format
QString gu_msec2hms(int msec);
QString gu_sec2hms(double sec);

bool GetLookAtInfo(CGIS_Polygon* pNode, GDM_CAMERA_INFO *pLookAtInfo);
void GetValidPos(CGDM_Location3D* pFirstPos, CGDM_Location3D* pSecondPos, CGDM_Location3D* pDstPos, bool blClosed);

#define OBJECT_TYPE_NO 40

#endif // GUIUTY_H
