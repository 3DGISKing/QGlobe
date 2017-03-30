#include <QtCore/QFileInfo>
#include <QtCore/QDir>
#include <string.h>
#include "gdemclient.h"
#include "config.h"
#include "mainwindow.h"
#include "renderfrm.h"
#include "rendertext.h"
#include "navbar.h"
#include "sidebar.h"
#include "searchdlg.h"
#include "guiuty.h"
#include "GDM_Interface.h"

#include "proxy.h"
#include "msgdata.h"



#ifdef Q_WS_MAC
#define APP_TRANS_FNAME		"/gdemclient_mac.qm"
#else
#define APP_TRANS_FNAME		"/geobrowser.qm"
#endif

#define APP_FONT_SIZE		10
#define APP_FONT_FAMILY		"PRK P ChongBong"
#define	APP_SEARCH_HISTORY	"/dbsearch.dat"
#define APP_PLUGIN_PATH     "/plugins"

QSplashScreen* g_Splash; 

GDemClient::GDemClient(int argc, char **argv) :
		QtSingleApplication(argc, argv)
{
	m_mainWnd = NULL;
	m_config = NULL;
	m_dataProxy = NULL;
	m_renderProxy = NULL;
	m_searchModel = NULL;

	//set organization & application name
	setOrganizationName(QString(CONF_SET_ORG));
	setApplicationName(QString(CONF_SET_APP));

	// get working directory
	m_workDir = applicationDirPath();
#ifdef MAC_DEBUG_APP_PATH
        m_workDir += QString("/../../../");
        QDir dir(m_workDir);
        m_workDir = dir.absolutePath();
#endif
#ifdef DEBUG
        qDebug() << "GDemClient::GDemClient:working dir=" << m_workDir;
#endif

	gdm_SetWorkPath(m_workDir);

	//create user data directory
	CreateUserDataDir();

	//install translator
	QString msgPath = m_workDir + APP_TRANS_FNAME;
	if (m_translator.load(msgPath))
		this->installTranslator(&m_translator);

	//set style sheet
	
	int fontSize = GetDrawFontSize(APP_FONT_SIZE);
	QString styleSheet = QString("* {font: %1pt %2;}").arg(fontSize).arg(APP_FONT_FAMILY);

	setStyleSheet(styleSheet);

	QCoreApplication::addLibraryPath(m_workDir+APP_PLUGIN_PATH);
}

GDemClient::~GDemClient()
{
	if(g_app)
		Release();
}

bool GDemClient::Init()
{
	if (!InitSearchHistroy())
		return false;

	//create config
	if (!InitConfig())
		return false;

    // apply style
	setStyle("Plastique");
	//QApplication::setStyle(new GDEMCustomStyle);
	
	return true;
}

bool GDemClient::InitConfig()
{
    m_config = new Config();
	if (m_config)
	{
		m_config->ReadSettings();

		if(!m_config->m_IsLicence)
		{
			gu_MessageBox(NULL, tr("Error"), tr("Please input licence!"));
		}
		return true;
	}
	return false;
}

bool GDemClient::InitProxy()
{
	//init data server
	m_dataProxy = new DataProxy();
	m_dataProxy->SendMessage(PM_SET_DATASERVER, (PrxParam)m_config->m_srvAddr,
							 (PrxParam) m_config->m_srvPort);
	if (!m_dataProxy->SendMessage(PM_INIT))
		return false;

	//init render server
	m_renderProxy = new RenderProxy();
	PrxRes dataSrv = m_dataProxy->SendMessage(PM_GET_DATASERVER);
	m_renderProxy->SendMessage(PM_SET_DATASERVER, dataSrv);
	if (!m_renderProxy->SendMessage(PM_INIT))
		return false;

	return true;
}

bool GDemClient::InitSearchHistroy()
{
	//set search history
	QString shPath = GetUserDataDir() + APP_SEARCH_HISTORY;
	m_searchModel = LoadSearchHistory(shPath);

	return (m_searchModel != NULL) ? true : false;
}

void GDemClient::Release()
{
	if (m_config)
		m_config->WriteSettings();

	delete m_config;

	if (m_renderProxy)
		m_renderProxy->SendMessage(PM_RELEASE);
	delete m_renderProxy;

	if (m_dataProxy)
		m_dataProxy->SendMessage(PM_RELEASE);
	delete m_dataProxy;

	SaveSearchHistory();
	if (m_searchModel)
	{
		delete m_searchModel;
		m_searchModel = NULL;
	}

	/*m_fontMng->Release();
	delete m_fontMng;*/

	if(m_searchModel)
		delete m_searchModel;
}

QString GDemClient::GetResource(const char *fileName)
{
    QString path = m_workDir + QString("/res") + fileName;
    return path;
}

QString GDemClient::GetResource(QString &fileName)
{
    QString path = m_workDir + QString("/res") + fileName;
    return path;
}

QString GDemClient::Trans(const char *s, const char *c)
{
	QString res = m_translator.translate(NULL, s, c);
	if (res.isEmpty() && s!=NULL)
		res = s;
	return res;
}

QString GDemClient::Trans(const QString &s)
{
	QByteArray byteArr = s.toAscii();
	QString res = m_translator.translate(NULL, byteArr.data(), NULL);
	if (res.isEmpty() && s.length() > 0)
		res = s;
	return res;
}

void GDemClient::InitializeGL()
{
	m_renderProxy->SendMessage(PM_SET_RENDERVIEW, (PrxParam)m_mainWnd->GetRenderFrame());

	FontInfo font;
	memset(&font, 0, sizeof(font));
	gu_qstring2ushort(m_config->m_mainFont.family(), font.family, FONT_FAMILY_MAX_SIZE);
	font.size = m_config->m_mainFont.pointSize();	

	if (m_config->m_mainFont.italic())
		font.flag |= FONT_ITALIC;
	if (m_config->m_mainFont.bold())
		font.flag |= FONT_BOLD;
	if (m_config->m_mainFont.strikeOut())
		font.flag |= FONT_STRIKEOUT;
	if (m_config->m_mainFont.underline())
		font.flag |= FONT_UNDERLINE;

	QString strKorName = GetFontFileName(true);
	QString strEngName = GetFontFileName(false);
	int		defaultSize = 11;

	m_renderProxy->SendMessage(PM_INIT_FONT, (PrxParam)&strKorName, (PrxParam)&strEngName);
	m_renderProxy->SendMessage(PM_INIT_FONT_SIZE, (PrxParam)defaultSize);

	GetMainWindow()->GetSideBar()->MoveStartPosition();
	UpdateOptionInfo();
}

void GDemClient::ChangedRenderSize(int w, int h)
{
	m_renderProxy->SendMessage(PM_CHANGED_VIEWSIZE, (PrxParam)w, (PrxParam)h);
}

int GDemClient::GetDrawFontSize(int size)
{
	int dpi = QApplication::desktop()->physicalDpiX();
	return (96 * size) / dpi;
}

int GDemClient::GetDrawFontMargin(int size)
{
	int dpi = QApplication::desktop()->physicalDpiX();
	return (dpi * size) / 96;
}

//key & nav's view joystic
void GDemClient::MoveView(int x, int y)
{
	m_renderProxy->GetRenderSrv()->MoveView(x , y);
}

//nav's look joystic
void GDemClient::LookView(int x, int y)
{
	m_renderProxy->GetRenderSrv()->LookView(x , y);
}

//key & nav's zoom slider
void GDemClient::ZoomView(int speed)
{
	m_renderProxy->GetRenderSrv()->MoveForward(speed , 1);
}

//key & navigation
void GDemClient::ResetView()
{
	m_renderProxy->GetRenderSrv()->ResetView();
}

//key
void GDemClient::ResetTilt()
{
	m_renderProxy->GetRenderSrv()->ResetTilt();
}

//key
void GDemClient::ResetAll()
{
	m_renderProxy->GetRenderSrv()->ResetAll();
}

//key
void GDemClient::StopView()
{
	m_renderProxy->GetRenderSrv()->StopView();
}

//nav's ring
void GDemClient::ChangedCompassAngle(double angle)
{
	MainWindow *mainWnd = g_app->GetMainWindow();
	NavBar *navBar = mainWnd->GetRenderFrame()->GetNavBar();
	if (navBar)
		navBar->SetCompassAngle(angle);

	double rad = degree2radian(angle);
	m_renderProxy->SendMessage(PM_CHANGED_COMPASSANGLE, (PrxParam)&rad);
}

void GDemClient::UpdateOptionInfo()
{
	m_renderProxy->SendMessage(PM_CHANGE_DISTMODE, (PrxParam)&m_config->m_distShowMode);
	m_renderProxy->SendMessage(PM_CHANGE_LABLESIZE, (PrxParam)&m_config->m_lableSize);
	m_renderProxy->SendMessage(PM_TOPOGRAPHY_QUALITY, (PrxParam)&m_config->m_topoQuality);
	m_renderProxy->SendMessage(PM_HEIGHT_MAGNIFY, (PrxParam)&m_config->m_heightMagnify);
	m_renderProxy->SendMessage(PM_CHANGE_CACHESIZE,
				(PrxParam)&m_config->m_memCacheSize, (PrxParam)&m_config->m_diskCacheSize);
	m_renderProxy->SendMessage(PM_CAMERA_MOVESPEED, (PrxParam)&m_config->m_moveSpeed);
	m_renderProxy->SendMessage(PM_SHOW_LENGTH_AREA, (PrxParam)&m_config->m_gisInfoChecked);
	m_renderProxy->SendMessage(PM_SHOW_LENGTH_AREA, (PrxParam)&m_config->m_gisInfoChecked);

	m_renderProxy->SendMessage(PM_SUNLIGHT_ENABLED,(PrxParam)&m_config->m_SunLightEnabled);

	double pos[3];

	pos[0]=m_config->m_SunLightLongitude;
	pos[1]=m_config->m_SunLightLatitude;
	pos[2]=m_config->m_SunLightHeight;
	
	m_renderProxy->SendMessage(PM_SUN_POSITION,(PrxParam)pos);
	m_renderProxy->SendMessage(PM_SUNLIGHT_COLOR,(PrxParam)&m_config->m_SunLightColor);
	m_renderProxy->SendMessage(PM_AMBIENT_COLOR,(PrxParam)&m_config->m_AmbientLightColor);
	m_renderProxy->SendMessage(PM_LIGHT_ATTENUATION,(PrxParam)&m_config->m_LightAttenuation);
	m_renderProxy->SendMessage(PM_CAMERA_FOV,(PrxParam)&m_config->m_CameraFOV);
}

/*
 * protected
 */
void GDemClient::CreateUserDataDir()
{
	m_userDataDir = QDesktopServices::storageLocation(QDesktopServices::DataLocation);

	QDir dir;
	bool res = dir.mkpath(m_userDataDir);
#ifdef DEBUG
	qDebug("GDemClient::CreateUserDataDir: %s(%s)", res ? "true" : "false",
		   m_userDataDir.toAscii().data());
#endif
}

QStringListModel* GDemClient::LoadSearchHistory(QString fileName)
{
	QFile file(fileName);
	if (!file.open(QFile::ReadOnly))
		return new QStringListModel;
	QStringList words;
	/*QString data = file.readAll();
	words = data.split(QString("\n"));*/

	while (!file.atEnd())
	{
		QByteArray line = file.readLine();
		line = line.trimmed();
		QString str = QString::fromUtf8(line.data());
		if (!str.isEmpty())
			words << str;
	}

	return new QStringListModel(words);
}

void GDemClient::SaveSearchHistory()
{
	if (m_searchModel == NULL)
		return;

	QString shPath = GetUserDataDir() + APP_SEARCH_HISTORY;
	QFile file(shPath);
	if (!file.open(QIODevice::WriteOnly))
	 return;

	QDataStream out(&file);
	QStringList list = m_searchModel->stringList();
	foreach (QString str, list)
	{
		str = str.trimmed();
		if (!str.isEmpty())
		{
			QByteArray array = str.toUtf8();
			int len = array.length();
			for ( int i = 0; i < len; i++)
			{
				quint8 code = array.at(i);
				out << code;
			}
			quint8 end = 0x0a;
			out << end;
		}
	}
}

void GDemClient::ResetRenderKeys()
{
	GetMainWindow()->GetRenderFrame()->ResetRenderKey();
}

void GDemClient::FullExtent()
{
#define LATI_KOREA		39.02 
#define LONGI_KOREA		125.75 
#define DST			    (10500000)
#define TIME            4000

	GDM_CAMERA_INFO cameraInfo;	
	memset(&cameraInfo , 0 , sizeof(GDM_CAMERA_INFO));
	cameraInfo.m_loc.m_dDist=DST;
	cameraInfo.m_loc.m_dLongitude=LONGI_KOREA*gdm_DEGTORAD;
	cameraInfo.m_loc.m_dLatitude=LATI_KOREA*gdm_DEGTORAD;

	m_renderProxy->GetRenderSrv()->m_pCameraController->AutoMoveTo(0, cameraInfo, TIME);

}

QString GDemClient::GetFontName(bool blKorean)
{
	QString fontName;

	if(blKorean)
		fontName = "PRK P Gothic";
	else
		fontName = "Arial";

	return fontName;
}

QString GDemClient::GetFontFileName(bool blKorean)
{
	QString fontName;

	if(blKorean)
		fontName = GetResource("/font/Pwpuchme.TTF");
	else
		fontName = GetResource("/font/Arialuni.ttf");

	return fontName;
}
