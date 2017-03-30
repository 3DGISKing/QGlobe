#include "dbpathsetting.h"
#include "config.h"
#include "gdemclient.h"
#include "msgdata.h"
#include "directorydialog.h"
#include "ui_dbpathsetting.h"

#ifdef STANDARD_ALONE

#include "../gds_common/GDSConfigMgr.h"
#include "../gds_database/GDSDatabaseInterface.h"

#define STR_DBPATH_SETTING     ("Select Directory")

DBPathSetting::DBPathSetting(QWidget *parent) :
    QDialog(parent),m_ui(new Ui::SetDBPathDlg)
{
    m_ui->setupUi(this);

	m_ui->pathEdit->setText(g_ServerConfig.GetFileDBPath());
}

DBPathSetting::~DBPathSetting()
{
	delete m_ui;
}

void DBPathSetting::on_toolButton_clicked()
{    
    QString strDir = g_ServerConfig.GetFileDBPath();//g_app->GetConfig()->m_strDataBasePath;
#ifdef Q_WS_WIN
	strDir = DirectoryDialog::GetDirectory(this, strDir);
#else
	QFileDialog::Options options = QFileDialog::DontResolveSymlinks | QFileDialog::ShowDirsOnly;
	strDir = QFileDialog::getExistingDirectory(this, QString(), strDir,options);
#endif


    if (!strDir.isEmpty())
    {
        m_ui->pathEdit->setText(strDir);
    }
}

void DBPathSetting::on_OKButton_clicked()
{
	wchar_t tmpPath[260];
    QString strPath = m_ui->pathEdit->text();
	int nLen = strPath.length();
	if (nLen > 0
		&& strPath[nLen-1] == '/')
	{
		strPath[nLen-1] = '\0';
		strPath.toWCharArray(tmpPath);
                strPath = QString::fromUtf16((ushort*)tmpPath);
	}	

    g_ServerConfig.SetFileDBPath(strPath);

	g_ServerConfig.WriteServerConfig();

	ReleaseGDSDatabase();

	InitializeGDSDatabase();

    QDialog::accept();
}

void DBPathSetting::on_cancelButton_clicked()
{
    QDialog::reject();
}

#endif
