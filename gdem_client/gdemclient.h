#ifndef GDEMCLIENT_H
#define GDEMCLIENT_H

#include <QtGui/QApplication>
#include <QtGui/QStringListModel>
#include <QtCore/QTranslator>

#include "proxy.h"
#include "../gdem_common/gdemclientbase.h"
#include "./qtsingleapplication/qtsingleapplication.h"

#define APP_USE_CUSTOM_DIALOG

class QSplashScreen;
class Config;
class MainWindow;

class GDemClient : public QtSingleApplication
{
public:
	GDemClient(int argc, char **argv);
	~GDemClient();

	QString& GetWorkDir()
	{	return m_workDir; }

	QString& GetUserDataDir()
	{	return m_userDataDir; }

	QTranslator* GetTranslator()
	{	return &m_translator; }

	QStringListModel *GetSearchModel()
	{	return m_searchModel; }

    void SetMainWindow(MainWindow *w)
    {   m_mainWnd = w; }
	
    MainWindow* GetMainWindow()
    {   return m_mainWnd; }

	Config* GetConfig()
	{	return m_config; }

	DataProxy* GetDataProxy()
	{	return m_dataProxy; }

	RenderProxy* GetRenderProxy()
	{	return m_renderProxy; }

	bool Init();
    bool InitConfig();
	bool InitProxy();
	bool InitSearchHistroy();
    void Release();

	QString GetResource(const char *fileName);
	QString GetResource(QString &fileName);
	QString Trans(const char *s, const char *c=0);
	QString Trans(const QString &s);

    //message from render frame
	void InitializeGL();
    void ChangedRenderSize(int w, int h);
	int GetDrawFontSize(int size); // QPainter Draw Font Size via Display DPI
	int GetDrawFontMargin(int size); // Placemark Data Margin

    void MoveView(int x, int y);
	void LookView(int x, int y);
	void ZoomView(int speed); //speed = -5 ~ +5
	void ResetView();
	void ResetTilt();
	void ResetAll();
	void StopView();
	void FullExtent();
	void ResetRenderKeys();

	void ChangedCompassAngle(double angle);

	void UpdateOptionInfo();

	QString GetFontName(bool blKorean);
	QString GetFontFileName(bool blKorean);

protected:
    QString m_workDir;
	QString m_userDataDir;
	QTranslator m_translator;
	QStringListModel *m_searchModel;

	MainWindow		*m_mainWnd;
	Config			*m_config;
	DataProxy		*m_dataProxy;
	RenderProxy		*m_renderProxy;

	void CreateUserDataDir();
	QStringListModel *LoadSearchHistory(QString fileName);
	void SaveSearchHistory();
};

extern GDemClient *g_app;

extern QSplashScreen* g_Splash; 

#endif // GDEMCLIENT_H
