#ifndef MEASUREAREADLG_H
#define MEASUREAREADLG_H

#include "ui_measureareadlg.h"
#include "GDM_CommandDlg.h"
#include "QtCore/QBasicTimer"

class CGDM_MeasureAreaCmd;

class MeasureAreaDlg : public CGDM_CommandDlg
{
	Q_OBJECT

public:
	MeasureAreaDlg(QWidget *parent = 0);
	~MeasureAreaDlg();

	virtual void UpdateDialog(void);
	void OnCheckCommand();
protected:
	virtual void closeEvent(QCloseEvent * e);
	virtual void timerEvent(QTimerEvent* e); 

private slots:
	void onClear();
	void onGroupBoxSimulationToggled(bool);
	void onDialMoveDirectionValueChanged(int);
	void onPushButtonStartClicked();
	void onPushButtonStopClicked();
	void onLineEditMoveVelocityEditingFinished();
	void onLineEditGrowVelocityEditingFinished();

	void btnSearch_click();

private:
	Ui::MeasureAreaDlg ui;
	CGDM_MeasureAreaCmd *m_pCommand;
	QBasicTimer           m_SimulationTimer;
};

#endif // MEASUREAREADLG_H
