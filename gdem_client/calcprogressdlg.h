#ifndef CALCPROGRESSDLG_H
#define CALCPROGRESSDLG_H

#include <QtGui/QDialog>
#include "IGIS_Progress.h"

namespace Ui {
    class CalcProgressDlg;
}

class CalcProgressDlg : public QDialog, IGIS_Progress {
    Q_OBJECT
public:
	CalcProgressDlg(QWidget *parent = 0);
    ~CalcProgressDlg();

private:
	Ui::CalcProgressDlg	*m_ui;

	int		m_succecced; //1: end, 0: stop, -1: calculating

public:
	virtual void setProgress(double percent);
	virtual bool isStop();

private slots:
	void StopCalc();

};

#endif // CALCPROGRESSDLG_H
