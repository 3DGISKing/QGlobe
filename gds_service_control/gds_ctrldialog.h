#ifndef CTRLDIALOG_H
#define CTRLDIALOG_H

#include <QtGui/QDialog>

#undef USE_DIALOG
#define USE_DIALOG		0

namespace Ui
{
    class CtrlDialog;
}

enum ESERVICEACTION
{
    eACTION_RUNNING,
    eACTION_STOP,
    eACTION_PAUSE,
};

class CtrlDialog : public QDialog
{
    Q_OBJECT

public:
    CtrlDialog(QWidget *parent = 0);
    ~CtrlDialog();
	void ShowHideWindow(void);

public slots:
    void on_InstallButton_clicked();
    void on_UninstallButton_clicked();
    void on_StartButton_clicked();
    void on_StopButton_clicked();
    void on_SuspendButton_clicked();
    void on_ResumeButton_clicked();
    void on_LogButton_clicked();

private:
    Ui::CtrlDialog *ui;

    void SetInstallState(bool a_bInstalled);
    void SetActionState(ESERVICEACTION a_eActionState);
	bool m_show;
};

#endif // CTRLDIALOG_H
