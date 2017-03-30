#ifndef RULERDLG_H
#define RULERDLG_H

#include <QtGui/QDialog>
#include "gdemclientbase.h"
#include "GDM_CommandDlg.h"

class CGDM_RulerCmd;

namespace Ui {
    class RulerDlg;
}

class RulerDlg : public CGDM_CommandDlg {
    Q_OBJECT
public:
    RulerDlg(QWidget *parent = 0);
    ~RulerDlg();

public:
	// inherited function
	virtual void UpdateDialog(void);
	void	OnCheckCommand(void);

protected:
	virtual void reject();
    void changeEvent(QEvent *e);
	void closeEvent(QCloseEvent *e);

private:
    Ui::RulerDlg *m_ui;

	int m_measMode;
	MeasureInfo m_lineMode;
	MeasureInfo m_pathMode;
	bool m_blMouseCheck;

	CursorState	m_curState;

	void Init();
	void OutputResult();
	double ConvertLength();
	void UpdateMeasureInfo();
	void ReleaseLastPoint();

private slots:
	void DeleteMeasureInfo();
	void ChangeMeasureMode(int mode);
	void ChangeMeasureUnit(int unit);
	void SetMouseCheck();

protected:

	CGDM_RulerCmd *m_pCommand;
};

#endif // RULERDLG_H
