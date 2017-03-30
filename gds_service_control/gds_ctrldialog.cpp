#include "gds_ctrldialog.h"
#include "ui_gds_ctrldialog.h"

#include <QtCore/QStringList>
#include <QtCore/QDir>
#include <QtCore/QSettings>
#include "../gds_service_common/gdsservice_common.h"
#include "gds_servicetrayicon.h"

CtrlDialog::CtrlDialog(QWidget *parent)
    : QDialog(parent), ui(new Ui::CtrlDialog)
	, m_show(false)
{
    ui->setupUi(this);
}

CtrlDialog::~CtrlDialog()
{
    delete ui;
}

void CtrlDialog::on_InstallButton_clicked()
{
    GDSServiceController controller(SERVICE_NAME);

    QString strPath = QDir::currentPath();
    strPath = strPath.left(strPath.lastIndexOf("control"));
    strPath += SERVICE_PATH;

    bool bInstalled = controller.install(strPath);

    SetInstallState(bInstalled);
}

void CtrlDialog::on_UninstallButton_clicked()
{
    GDSServiceController controller(SERVICE_NAME);

    bool bInstall = controller.uninstall();

    if(bInstall)
        SetInstallState(!bInstall);
}

void CtrlDialog::SetInstallState(bool a_bInstalled)
{
    if(a_bInstalled)
        ui->InstallLabel->setText(QString::fromLocal8Bit("Install"));
    else
        ui->InstallLabel->setText(QString::fromLocal8Bit("Uninstall"));
}

void CtrlDialog::SetActionState(ESERVICEACTION a_eActionState)
{
    switch(a_eActionState)
    {
    case eACTION_RUNNING:
        {
            ui->ActionLabel->setText(QString::fromLocal8Bit("Start"));
        }
        break;
    case eACTION_STOP:
        {
            ui->ActionLabel->setText(QString::fromLocal8Bit("Stop"));
        }
        break;
    case eACTION_PAUSE:
        {
            ui->ActionLabel->setText(QString::fromLocal8Bit("Pause"));
        }
        break;
    default:
        break;
    }
}

void CtrlDialog::on_StartButton_clicked()
{
    GDSServiceController controller(SERVICE_NAME);
    bool bRun = controller.start();

    if(bRun)
        SetActionState(eACTION_RUNNING);
}

void CtrlDialog::on_StopButton_clicked()
{
    GDSServiceController controller(SERVICE_NAME);
    bool bRun = controller.stop();

    if(bRun)
        SetActionState(eACTION_STOP);
}

void CtrlDialog::on_SuspendButton_clicked()
{
    GDSServiceController controller(SERVICE_NAME);
    bool bRun = controller.pause();

    if(bRun)
        SetActionState(eACTION_PAUSE);
}

void CtrlDialog::on_ResumeButton_clicked()
{
    GDSServiceController controller(SERVICE_NAME);
    bool bRun = controller.resume();

    if(bRun)
        SetActionState(eACTION_PAUSE);
}

void CtrlDialog::on_LogButton_clicked()
{
}

void CtrlDialog::ShowHideWindow(void)
{
	if (m_show)
	{
		hide();
		m_show = false;
	}
	else
	{
		show();
		m_show = true;
	}
}
