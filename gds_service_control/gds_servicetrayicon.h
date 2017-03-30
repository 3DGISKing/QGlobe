/*
	This file is part of Free Space Monitor.

    Free Space Monitor is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    Free Space Monitor is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Free Space Monitor; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/


#ifndef _NOTE_H_
#define NOTE_H_ 

#include <QtGui>
#include <QtGui/QPainter>
#include <QtGui/QApplication>
#include "../gds_service_common/gdsservice_common.h"
#include <qglobal.h>

#if USE_DIALOG
#include "gds_ctrldialog.h"
#endif

#define SERVICE_NAME    "GDEMService"
#define MONITOR_SERVICE_NAME	"GDS Service Monitoring"
#if defined  (Q_WS_WIN)
#define SERVICE_PATH    "/gds_service.exe"
#define MONITOR_SERVICE_PATH    "/gds_service_monitoring.exe"
#else
#define SERVICE_PATH    "/gds_service"
#define MONITOR_SERVICE_PATH    "/gds_service_monitoring"
#endif

#define STRING_MENU_ITEM_SHOW_HIDE_WINDOW		"Show/Hide Window"
#define STRING_MENU_ITEM_SERVICE_MONITOR		"Monitor GDS Server Service"
#define STRING_MENU_ITEM_SERVICE_INSTALL		"Install GDS Server Service"
#define STRING_MENU_ITEM_SERVICE_UNINSTALL		"Uninstall GDS Server Service"
#define STRING_MENU_ITEM_SERVICE_START			"Start GDS Server Service"
#define STRING_MENU_ITEM_SERVICE_STOP			"Stop GDS Server Service"
#if USE_SERVICE_PAUSE
#define STRING_MENU_ITEM_SERVICE_PAUSE			"Pause GDS Server Service"
#define STRING_MENU_ITEM_SERVICE_RESUME			"Resume GDS Server Service"
#endif
#define STRING_MENU_ITEM_QUIT					"Quit"

#undef  USE_MENU_MONITOR
#define USE_MENU_MONITOR		0
enum E_MENU_INDEX
{
#if USE_MENU_MONITOR
	eMenuIdMonitor			= 0, // no used
	eMenuIdInstall,
#else
	eMenuIdInstall			= 0,
#endif
	eMenuIdUninstall,
	eMenuIdStart,
	eMenuIdStop,
#if USE_SERVICE_PAUSE
	eMenuIdPause,
	eMenuIdResume,
#endif
	eMenuIdMax,
};

enum E_SVC_QUERY_TYPE
{
	eSvcQueryNone		= 0,
	eSvcQueryMonitor,
	eSvcQueryInstall,
	eSvcQueryUninstall,
	eSvcQueryStart,
	eSvcQueryStop,
#if USE_SERVICE_PAUSE
	eSvcQueryPause,
	eSvcQueryResume,
#endif
};

class GDSServiceTrayIcon : public QSystemTrayIcon {
Q_OBJECT
public:
#if USE_DIALOG
	GDSServiceTrayIcon(QApplication *qapp, CtrlDialog *qdlg);
#else
	GDSServiceTrayIcon(QApplication *qapp);
#endif
	~GDSServiceTrayIcon(void);
	QMenu * getContextMenu(void);
protected:
private slots:
	void menuOption(QAction * action);
	void trayActivated(QSystemTrayIcon::ActivationReason reason);

private:
	void createTrayIconMenu(void);
	void initMenu(QMenu *_menu);
	void setMyIcon(const QString fileName);
	QString GetResource(const QString& fileName);

	QApplication *myQapp;	
#if USE_DIALOG
	CtrlDialog *myQdlg;
#endif
	QMenu *menu;

	QList<QAction* > menuActList;
	
	//enum modes { SPECCY, C64 } ;
	//int mode;
	void serviceQuery(const QString& serviceName, const E_SVC_QUERY_TYPE queryType);
	void updateUIServiceStatus(const QString& serviceName, const E_SVC_QUERY_TYPE queryType);
};
#endif
