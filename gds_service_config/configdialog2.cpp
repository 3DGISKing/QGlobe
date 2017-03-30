#include "configdialog2.h"
#include "ui_configdialog2.h"
#include "directorydialog.h"
#include "../gds_service_common/gdsservice_common.h"
#include "../gdem_common/gdem_distributeconfig.h"
#include "../gds_common/GDSConfigMgr.h"
#include <QMessageBox>
#include <QDir>
#include <QCoreApplication>

#define MAIN_SERVICE_NAME    "GDEMService"
#define MONITOR_SERVICE_NAME	"GDS Service Monitoring"
#if defined  (Q_WS_WIN)
#define SERVICE_PATH    "/gds_service.exe"
#define MONITOR_SERVICE_PATH    "/gds_service_monitoring.exe"
#else
#define SERVICE_PATH    "/gds_service"
#define MONITOR_SERVICE_PATH    "/gds_service_monitoring"
#endif

#define MIN_SRV_PORT		1025
#define MAX_SRV_PORT		99999
#define MIN_SRV_CONNECT_COUNT	1
#define MAX_SRV_CONNECT_COUNT	9999

static void artisticSleep(int sleepTime)
{
	QTime time;
	time.restart();
	while (time.elapsed() < sleepTime)
	{
		QCoreApplication::processEvents(QEventLoop::AllEvents, 50);
	}
}

void StartService(const QString &name)
{
	GDSServiceController controller(name);
	if (!controller.isRunning())
	{
		controller.start();

		artisticSleep(2000);

		//while(!controller.isRunning())
		//{
		//	artisticSleep(100);
		//}
	}
}

void StopService(const QString &name)
{
	GDSServiceController controller(name);
	if (controller.isRunning())
	{
		controller.stop();

		artisticSleep(2000);

		while(controller.isRunning())
		{
			artisticSleep(100);
		}
	}
}


void gc_wchar2ushort(wchar_t *src, ushort *dst, int len)
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


ConfigDialog2::ConfigDialog2(QWidget *parent)
	: QDialog(parent), m_ui(new Ui::ConfigDialog2)
{
	m_ui->setupUi(this);
}

ConfigDialog2::~ConfigDialog2()
{
	Release();
	delete m_ui;
}

bool ConfigDialog2::Init()
{
	InstallService();

	if (!ReadConfig())
		return false;

	connect(m_ui->browseButton, SIGNAL(clicked()), this, SLOT(OpenBrowse()));
	connect(m_ui->browseButton_2, SIGNAL(clicked()), this, SLOT(OpenBrowse2()));
	connect(m_ui->buttonBox, SIGNAL(clicked(QAbstractButton*)), this,
		SLOT(ClickButtonBox(QAbstractButton*)));
	connect(m_ui->buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

	SetValues();

	return true;
}

void ConfigDialog2::Release()
{
}

bool ConfigDialog2::ReadConfig()
{
	g_ServerConfig.ReadServerConfig();
	g_DistributeConfig.ReadFromRegistry();

	if (g_DistributeConfig.GetDistributableType() != E_DISTRIBUTE_TYPE_DIRECTORY
		|| g_DistributeConfig.GetDistributeNum() != 1024
		|| g_DistributeConfig.GetDistributeTargetNum() != 2)
	{
		// setting distribute
		g_DistributeConfig.Initialize();

		g_DistributeConfig.SetDistributableType(E_DISTRIBUTE_TYPE_DIRECTORY);
		g_DistributeConfig.SetDistributeNum(1024);
		g_DistributeConfig.SetDistributeTargetNum(2);

		ST_DISTRIBUTE_FOLDER tFolder;
		tFolder.start_index = 0;
		tFolder.end_index = 643;

		QString strDir1 = g_ServerConfig.GetFileDBPath();
		QString strDir2 = strDir1;
		strDir1 += "1";
		strDir2 += "2";

		g_ServerConfig.SetFileDBPath(strDir1);

		wchar_t tmpPath[MAX_DIST_PATH];
		strDir1.toWCharArray(tmpPath);
		gc_wchar2ushort(tmpPath, tFolder.path, strDir1.count());

		g_DistributeConfig.AddFolderInfo(tFolder);

		tFolder.start_index = 644;
		tFolder.end_index = 1023;
		strDir2.toWCharArray(tmpPath);
		gc_wchar2ushort(tmpPath, tFolder.path, strDir2.count());

		g_DistributeConfig.AddFolderInfo(tFolder);

		g_DistributeConfig.MakeIndexingMap();

		g_ServerConfig.WriteServerConfig();
		g_DistributeConfig.WriteToRegistry();
	}

	return true;
}

void ConfigDialog2::SaveConfig()
{
	g_ServerConfig.WriteServerConfig();
	g_DistributeConfig.WriteToRegistry();
}

void ConfigDialog2::GetValues()
{
	wchar_t tmpPath[MAX_DIST_PATH];
	QString strDBPath1 = m_ui->dbpathEdit->text();
	int nLen = strDBPath1.length();
	if (nLen > 0
		&& strDBPath1[nLen-1] == '/')
	{
		strDBPath1[nLen-1] = '\0';
		strDBPath1.toWCharArray(tmpPath);
                strDBPath1 = QString::fromUtf16((ushort*)tmpPath);
	}	

	g_ServerConfig.SetPortNo(m_ui->srvportSpin->value());
	g_ServerConfig.SetMaxConnectNum(m_ui->srvconnectSpin->value());
    g_ServerConfig.SetFileDBPath(strDBPath1);

	ST_DISTRIBUTE_FOLDER tFolder;
	g_DistributeConfig.GetFolderInfo(0, tFolder);

	strDBPath1.toWCharArray(tmpPath);
	gc_wchar2ushort(tmpPath, tFolder.path, strDBPath1.count());
	g_DistributeConfig.UpdateFolderInfo(0, tFolder);

	QString strDBPath2 = m_ui->dbpathEdit_2->text();
	nLen = strDBPath2.length();
	if (nLen > 0
		&& strDBPath2[nLen-1] == '/')
	{
		strDBPath2[nLen-1] = '\0';
		strDBPath2.toWCharArray(tmpPath);
                strDBPath2 = QString::fromUtf16((ushort*)tmpPath);
	}	

	g_DistributeConfig.GetFolderInfo(1, tFolder);

	strDBPath2.toWCharArray(tmpPath);
	gc_wchar2ushort(tmpPath, tFolder.path, strDBPath2.count());
	g_DistributeConfig.UpdateFolderInfo(1, tFolder);
}

void ConfigDialog2::SetValues()
{
	// configure
	m_ui->srvportSpin->setValue(g_ServerConfig.GetPortNo());
	m_ui->srvconnectSpin->setValue(g_ServerConfig.GetMaxConnectNum());
	m_ui->dbpathEdit->setText(g_ServerConfig.GetFileDBPath());


	ST_DISTRIBUTE_FOLDER tFolder;
	g_DistributeConfig.GetFolderInfo(1, tFolder);
	m_ui->dbpathEdit_2->setText(QString::fromUtf16(tFolder.path));
}

bool ConfigDialog2::IsChanged()
{
	if (g_ServerConfig.GetPortNo() != m_ui->srvportSpin->value())
		return true;
	if (g_ServerConfig.GetMaxConnectNum() != m_ui->srvconnectSpin->value())
		return true;
	if (g_ServerConfig.GetFileDBPath().compare(m_ui->dbpathEdit->text()) != 0)
		return true;

	ST_DISTRIBUTE_FOLDER tFolder;
	g_DistributeConfig.GetFolderInfo(1, tFolder);

	QString strDBPath2 = QString::fromUtf16(tFolder.path);
	if (strDBPath2.compare(m_ui->dbpathEdit_2->text()) != 0)
		return true;

	return false;
}

bool ConfigDialog2::IsValidPath(QString& sPath)
{
	sPath = sPath.trimmed();
	if (sPath.isEmpty())
		return false;
	QDir dir(sPath);
	if (!dir.exists())
		return false;
	return true;
}

bool ConfigDialog2::IsValidValue()
{
	if (m_ui->srvportSpin->value() < MIN_SRV_PORT
		|| m_ui->srvportSpin->value() > MAX_SRV_PORT)
	{
// 		QString sMsg;
// 		sMsg.sprintf("Service Port Error! Between %d-%d, Enter the number.", MIN_SRV_PORT, MAX_SRV_PORT);
		QMessageBox::warning(this, tr("Service Options"),
			QObject::tr("Service Port Error! Between 1025 ~ 99999, Enter the number.")
			, QMessageBox::Ok, QMessageBox::Ok);
		return false;
	}

	if (m_ui->srvconnectSpin->value() < MIN_SRV_CONNECT_COUNT
		|| m_ui->srvconnectSpin->value() > MAX_SRV_CONNECT_COUNT)
	{
// 		QString sMsg;
// 		sMsg.sprintf("Max Connections Error! Between %d-%d, Enter the number.", MIN_SRV_CONNECT_COUNT, MAX_SRV_CONNECT_COUNT);
		QMessageBox::warning(this, tr("Service Options"),
			QObject::tr("Max Connections Error! Between 1 ~ 9999, Enter the number.")
			, QMessageBox::Ok, QMessageBox::Ok);
		return false;
	}

	QString sPath;
	sPath = m_ui->dbpathEdit->text();
	if (!IsValidPath(sPath))
	{
		QMessageBox::warning(this, tr("Service Options"),
			QObject::tr("Database path1 is incorrect!")
			, QMessageBox::Ok, QMessageBox::Ok);
		return false;
	}
	m_ui->dbpathEdit->setText(sPath);

	sPath = m_ui->dbpathEdit_2->text();
	if (!IsValidPath(sPath))
	{
		QMessageBox::warning(this, tr("Service Options"),
			QObject::tr("Database path1 is incorrect!")
			, QMessageBox::Ok, QMessageBox::Ok);
		return false;
	}
	m_ui->dbpathEdit_2->setText(sPath);

	return true;
}

void ConfigDialog2::OpenBrowse()
{
	QString dir = m_ui->dbpathEdit->text();
	m_ui->dbpathEdit->setText(DirectoryDialog::GetDirectory(this, dir));
}

void ConfigDialog2::OpenBrowse2()
{
	QString dir = m_ui->dbpathEdit_2->text();
	m_ui->dbpathEdit_2->setText(DirectoryDialog::GetDirectory(this, dir));
}

bool ConfigDialog2::CreateLayout()
{
	//QVBoxLayout *mainLayout = new QVBoxLayout;
	//if (!mainLayout)
	//	return false;

	//mainLayout->addWidget(m_ui->buttonBox,0,Qt::AlignHCenter);
	//setLayout(mainLayout);

	return true;
}

void ConfigDialog2::ApplyService()
{
	if (IsChanged())
	{
		int ret = QMessageBox::warning(this, tr("Service Options"),
			QObject::tr("Service Configure Info was Changed. Restart Service?(Yes/No)")
			, QMessageBox::Yes|QMessageBox::No, QMessageBox::Yes);
		if (ret == QMessageBox::Yes)
		{
			GetValues();
			SaveConfig();
			RestartAllService();
		}
	}
	else
	{
		StartAllService();
	}
}

void ConfigDialog2::ClickButtonBox(QAbstractButton *button)
{
	QDialogButtonBox::ButtonRole role = m_ui->buttonBox->buttonRole(button);
	switch (role)
	{
	case QDialogButtonBox::AcceptRole:
		if (IsValidValue())
		{
			ApplyService();
			accept();
		}
		break;
	case QDialogButtonBox::ApplyRole:
		if (IsValidValue())
		{
			ApplyService();
		}
		break;
	default:
		break;
	}
}

void ConfigDialog2::InstallService()
{
	GDSServiceController gdsMonitorServiceController(MONITOR_SERVICE_NAME);
	if (!gdsMonitorServiceController.isInstalled())
	{
		QString strPath = QDir::currentPath();
		strPath += MONITOR_SERVICE_PATH;
		gdsMonitorServiceController.install(strPath);
	}
	if (gdsMonitorServiceController.isInstalled())
	{
		GDSServiceController gdsServiceController(MAIN_SERVICE_NAME);
		if (!gdsServiceController.isInstalled())
		{
			QString strPath = QDir::currentPath();
			strPath += SERVICE_PATH;
			gdsServiceController.install(strPath);
		}
	}
}

void ConfigDialog2::StartAllService()
{
	this->setDisabled(true);

	StartService(MAIN_SERVICE_NAME);
	StartService(MONITOR_SERVICE_NAME);

	this->setDisabled(false);
}

void ConfigDialog2::RestartAllService()
{
	this->setDisabled(true);

	StopService(MONITOR_SERVICE_NAME);
	StopService(MAIN_SERVICE_NAME);

	StartService(MAIN_SERVICE_NAME);
	StartService(MONITOR_SERVICE_NAME);

	this->setDisabled(false);
}

