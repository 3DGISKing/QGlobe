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

#include <QCoreApplication>
#include <QtCore/QDir>
#include "gds_servicetrayicon.h"

#if USE_DIALOG
GDSServiceTrayIcon::GDSServiceTrayIcon(QApplication *qapp, CtrlDialog *qdlg)
#else
GDSServiceTrayIcon::GDSServiceTrayIcon(QApplication *qapp)
#endif
{
	myQapp=qapp;
#if USE_DIALOG
	myQdlg = qdlg;
#endif
	menu=new QMenu();

	initMenu(menu);
	
	setToolTip("Please wait ...");
	
	connect(menu,
			SIGNAL(triggered(QAction *)),
			this,
			SLOT(menuOption(QAction *))
	);

	connect(this,
			SIGNAL(activated(QSystemTrayIcon::ActivationReason )),
			this,
			SLOT(trayActivated(QSystemTrayIcon::ActivationReason ))
	);

	setMyIcon(QString("/gdemserver.png"));
	updateUIServiceStatus(QString(SERVICE_NAME), eSvcQueryNone);
	updateUIServiceStatus(QString(MONITOR_SERVICE_NAME), eSvcQueryMonitor);
}

GDSServiceTrayIcon::~GDSServiceTrayIcon(void)
{
}

void GDSServiceTrayIcon::initMenu(QMenu *_menu)
{
	if (_menu)
	{
		QAction* act = NULL;

		menuActList.clear();
#if USE_DIALOG
		_menu->addAction(STRING_MENU_ITEM_SHOW_HIDE_WINDOW);
		_menu->addSeparator();
#endif

		//act = _menu->addAction(STRING_MENU_ITEM_SERVICE_MONITOR);
		//act->setCheckable(true);
		//menuActList.append(act);
		//_menu->addSeparator();

		menuActList.append(_menu->addAction(STRING_MENU_ITEM_SERVICE_INSTALL));
		menuActList.append(_menu->addAction(STRING_MENU_ITEM_SERVICE_UNINSTALL));
		_menu->addSeparator();
		menuActList.append(_menu->addAction(STRING_MENU_ITEM_SERVICE_START));
		menuActList.append(_menu->addAction(STRING_MENU_ITEM_SERVICE_STOP));
#if USE_SERVICE_PAUSE
		menuActList.append(_menu->addAction(STRING_MENU_ITEM_SERVICE_PAUSE));
		menuActList.append(_menu->addAction(STRING_MENU_ITEM_SERVICE_RESUME));
#endif

		_menu->addSeparator();
		_menu->addAction(STRING_MENU_ITEM_QUIT);

		setContextMenu(_menu);
	}
}

QString GDSServiceTrayIcon::GetResource(const QString& fileName)
{
	QString workDir;
	workDir = QCoreApplication::applicationDirPath();
	QString path = workDir + QString("/res") + fileName;
	return path;
}

void GDSServiceTrayIcon::setMyIcon(const QString fileName)
{
	QPixmap trayPicture(16,16);	
	// trayPicture.fill(QColor(const_cast<char*>("#F00")));
	trayPicture.load(GetResource(fileName));
	QPixmap pixmap(trayPicture);
	QIcon icon(pixmap);
	setIcon(icon);
	show();
}

void GDSServiceTrayIcon::trayActivated(QSystemTrayIcon::ActivationReason reason)
{
	switch(reason)
	{
	case QSystemTrayIcon::Context:
		updateUIServiceStatus(QString(SERVICE_NAME), eSvcQueryNone);
		updateUIServiceStatus(QString(MONITOR_SERVICE_NAME), eSvcQueryMonitor);
		break;
	case QSystemTrayIcon::DoubleClick:
	case QSystemTrayIcon::Trigger:
	case QSystemTrayIcon::MiddleClick:
	default:
		break;
	}
}

void GDSServiceTrayIcon::menuOption(QAction * action){
	if(action->text().startsWith(STRING_MENU_ITEM_QUIT)){
		myQapp->quit();
		return;
	}

	if(action->text().startsWith(STRING_MENU_ITEM_SHOW_HIDE_WINDOW))
	{
#if USE_DIALOG
		if (myQdlg)
		{
			myQdlg->ShowHideWindow();
		}
#endif
		return;
	}

	E_SVC_QUERY_TYPE svcQueryType = eSvcQueryNone;
	if (action->text().startsWith(STRING_MENU_ITEM_SERVICE_MONITOR))
	{
		svcQueryType = eSvcQueryMonitor;
	}
	if (action->text().startsWith(STRING_MENU_ITEM_SERVICE_INSTALL))
	{
		svcQueryType = eSvcQueryInstall;
	}
	else if (action->text().startsWith(STRING_MENU_ITEM_SERVICE_UNINSTALL))
	{
		svcQueryType = eSvcQueryUninstall;
	}
	else if (action->text().startsWith(STRING_MENU_ITEM_SERVICE_START))
	{
		svcQueryType = eSvcQueryStart;
	}
	else if (action->text().startsWith(STRING_MENU_ITEM_SERVICE_STOP))
	{
		svcQueryType = eSvcQueryStop;
	}
#if USE_SERVICE_PAUSE
	else if (action->text().startsWith(STRING_MENU_ITEM_SERVICE_PAUSE))
	{
		svcQueryType = eSvcQueryPause;
	}
	else if (action->text().startsWith(STRING_MENU_ITEM_SERVICE_RESUME))
	{
		svcQueryType = eSvcQueryResume;
	}
#endif

	if (svcQueryType != eSvcQueryMonitor
		&& svcQueryType != eSvcQueryStop
		&& svcQueryType != eSvcQueryStart
		)
	{
		serviceQuery(QString(SERVICE_NAME), svcQueryType);
	}

	if (svcQueryType == eSvcQueryMonitor
		|| svcQueryType == eSvcQueryInstall
		|| svcQueryType == eSvcQueryUninstall
		|| svcQueryType == eSvcQueryStart
		|| svcQueryType == eSvcQueryStop)
	{
		serviceQuery(QString(MONITOR_SERVICE_NAME), svcQueryType);
	}
}

void GDSServiceTrayIcon::serviceQuery(const QString& serviceName, const E_SVC_QUERY_TYPE queryType)
{
	if (!serviceName.isEmpty())
	{
		GDSServiceController gdsServiceController(serviceName);

		switch (queryType)
		{
		case eSvcQueryMonitor:
			if (gdsServiceController.isInstalled())
			{
				if (gdsServiceController.isRunning())
				{
					gdsServiceController.stop();
				}
				else
				{
					gdsServiceController.start();
				}
			}
			break;
		case eSvcQueryInstall:
			if (!gdsServiceController.isInstalled())
			{
				QString strPath = QDir::currentPath();
				strPath = strPath.left(strPath.lastIndexOf("control"));
				if (serviceName.compare(MONITOR_SERVICE_NAME) == 0)
				{
					strPath += MONITOR_SERVICE_PATH;
				}
				else
				{
					strPath += SERVICE_PATH;
				}
				gdsServiceController.install(strPath);
			}
			break;
		case eSvcQueryUninstall:
			if (gdsServiceController.isInstalled())
				gdsServiceController.uninstall();
			break;
		case eSvcQueryStart:
			if (!gdsServiceController.isRunning())
				gdsServiceController.start();
			break;
		case eSvcQueryStop:
			if (gdsServiceController.isRunning())
				gdsServiceController.stop();
			break;
#if USE_SERVICE_PAUSE
		case eSvcQueryPause:
			if (gdsServiceController.isRunning())
				gdsServiceController.pause();
			break;
		case eSvcQueryResume:
			if (gdsServiceController.isRunning())
				gdsServiceController.resume();
			break;
#endif
		default:
			break;
		}
	}
}

void GDSServiceTrayIcon::updateUIServiceStatus(const QString& serviceName, const E_SVC_QUERY_TYPE queryType)
{
	if (!serviceName.isEmpty())
	{
		GDSServiceController gdsServiceController(serviceName);

		switch (queryType)
		{
		case eSvcQueryMonitor:
#if USE_MENU_MONITOR
			if (gdsServiceController.isInstalled())
			{
				if (gdsServiceController.isRunning())
					menuActList[eMenuIdMonitor]->setChecked(true);
				else
					menuActList[eMenuIdMonitor]->setChecked(false);
			}
			else
			{
				menuActList[eMenuIdMonitor]->setEnabled(false);
			}
			break;
#endif
		case eSvcQueryInstall:
			if (gdsServiceController.isInstalled())
			{
				menuActList[eMenuIdInstall]->setEnabled(false);
				menuActList[eMenuIdUninstall]->setEnabled(true);
				if (gdsServiceController.isRunning())
				{
					menuActList[eMenuIdStart]->setEnabled(false);
					menuActList[eMenuIdStop]->setEnabled(true);
#if USE_SERVICE_PAUSE
					menuActList[eMenuIdPause]->setEnabled(true);
					menuActList[eMenuIdResume]->setEnabled(true);
#endif
				}
				else
				{
					menuActList[eMenuIdStart]->setEnabled(true);
					menuActList[eMenuIdStop]->setEnabled(false);
#if USE_SERVICE_PAUSE
					menuActList[eMenuIdPause]->setEnabled(false);
					menuActList[eMenuIdResume]->setEnabled(false);
#endif
				}
			}
			break;
		case eSvcQueryUninstall:
			if (!gdsServiceController.isInstalled())
			{
				menuActList[eMenuIdInstall]->setEnabled(true);
				menuActList[eMenuIdUninstall]->setEnabled(false);
				menuActList[eMenuIdStart]->setEnabled(false);
				menuActList[eMenuIdStop]->setEnabled(false);
#if USE_SERVICE_PAUSE
				menuActList[eMenuIdPause]->setEnabled(false);
				menuActList[eMenuIdResume]->setEnabled(false);
#endif
			}
			break;
		case eSvcQueryStart:
			if (gdsServiceController.isInstalled() && gdsServiceController.isRunning())
			{
				menuActList[eMenuIdInstall]->setEnabled(false);
				menuActList[eMenuIdUninstall]->setEnabled(false);
				menuActList[eMenuIdStart]->setEnabled(false);
				menuActList[eMenuIdStop]->setEnabled(true);
#if USE_SERVICE_PAUSE
				menuActList[eMenuIdPause]->setEnabled(true);
				menuActList[eMenuIdResume]->setEnabled(false);
#endif
			}
			break;
		case eSvcQueryStop:
			if (gdsServiceController.isInstalled() && !gdsServiceController.isRunning())
			{
				menuActList[eMenuIdInstall]->setEnabled(false);
				menuActList[eMenuIdUninstall]->setEnabled(true);
				menuActList[eMenuIdStart]->setEnabled(true);
				menuActList[eMenuIdStop]->setEnabled(false);
#if USE_SERVICE_PAUSE
				menuActList[eMenuIdPause]->setEnabled(false);
				menuActList[eMenuIdResume]->setEnabled(false);
#endif
			}
			break;
#if USE_SERVICE_PAUSE
		case eSvcQueryPause:
			if (gdsServiceController.isInstalled() && gdsServiceController.isRunning())
			{
				menuActList[eMenuIdInstall]->setEnabled(false);
				menuActList[eMenuIdUninstall]->setEnabled(false);
				menuActList[eMenuIdStart]->setEnabled(false);
				menuActList[eMenuIdStop]->setEnabled(true);
				menuActList[eMenuIdPause]->setEnabled(false);
				menuActList[eMenuIdResume]->setEnabled(true);
			}
			break;
		case eSvcQueryResume:
			if (gdsServiceController.isInstalled() && gdsServiceController.isRunning())
			{
				menuActList[eMenuIdInstall]->setEnabled(false);
				menuActList[eMenuIdUninstall]->setEnabled(false);
				menuActList[eMenuIdStart]->setEnabled(false);
				menuActList[eMenuIdStop]->setEnabled(true);
				menuActList[eMenuIdPause]->setEnabled(true);
				menuActList[eMenuIdResume]->setEnabled(false);
			}
			break;
#endif
		default:
			if (gdsServiceController.isInstalled())
			{
				menuActList[eMenuIdInstall]->setEnabled(false);
				if (gdsServiceController.isRunning())
				{
#if USE_MENU_MONITOR
					menuActList[eMenuIdMonitor]->setEnabled(true);
#endif
					menuActList[eMenuIdUninstall]->setEnabled(false);
					menuActList[eMenuIdStart]->setEnabled(false);
					menuActList[eMenuIdStop]->setEnabled(true);
#if USE_SERVICE_PAUSE
					menuActList[eMenuIdPause]->setEnabled(true);
					menuActList[eMenuIdResume]->setEnabled(false);
#endif
				}
				else
				{
#if USE_MENU_MONITOR
					menuActList[eMenuIdMonitor]->setEnabled(false);
#endif
					menuActList[eMenuIdUninstall]->setEnabled(true);
					menuActList[eMenuIdStart]->setEnabled(true);
					menuActList[eMenuIdStop]->setEnabled(false);
#if USE_SERVICE_PAUSE
					menuActList[eMenuIdPause]->setEnabled(false);
					menuActList[eMenuIdResume]->setEnabled(false);
#endif
				}
			}
			else
			{
#if USE_MENU_MONITOR
				menuActList[eMenuIdMonitor]->setEnabled(false);
#endif
				menuActList[eMenuIdInstall]->setEnabled(true);
				menuActList[eMenuIdUninstall]->setEnabled(false);
				menuActList[eMenuIdStart]->setEnabled(false);
				menuActList[eMenuIdStop]->setEnabled(false);
#if USE_SERVICE_PAUSE
				menuActList[eMenuIdPause]->setEnabled(false);
				menuActList[eMenuIdResume]->setEnabled(false);
#endif
			}
			break;
		}
	}
}
