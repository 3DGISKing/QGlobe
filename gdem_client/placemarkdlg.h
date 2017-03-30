#ifndef PLACEMARKDLG_H
#define PLACEMARKDLG_H

#include <QtGui>
#include "gdemclientbase.h"
#include "GDM_CommandDlg.h"



namespace Ui {
    class PlaceMarkDlg;
}

class PlaceMarkMgr;
class CGDM_PointCmd;

class PlaceMarkDlg : public CGDM_CommandDlg {
    Q_OBJECT
public:
	PlaceMarkDlg(QWidget *parent = 0, bool isAdd = true);
    ~PlaceMarkDlg();

	virtual void accept();
	virtual void reject();

	void UpdateData(PLACEMARK_DATA *pmd);
	bool IsCheckedViewCenter();

	// inherited function
	virtual void UpdateDialog(void);


protected:
    void changeEvent(QEvent *e);
	virtual void showEvent(QShowEvent *event);

	void EnableViewControl(bool enable);
	void UpdateOutputInfo();

	PlaceMarkMgr	*m_placeMarkMgr;
	PLACEMARK_DATA	m_placeMarkData;
	bool			m_isAdd;
	unsigned int	m_state;
	bool			m_viewcenter;

private:
	Ui::PlaceMarkDlg	*m_ui;
	CGDM_PointCmd		*m_pCommand;
	void			CopyDataToNode();
	void			CopyDataFromNode();
	void			MoveCenterPlaceMark();

private slots:
	void SetFocusOkButton();
	void SetMarkName();
	void SetMarkLongitude();
	void SetMarkLatitude();
	void btnGo_click(); //asd 2014.6.4
	void SetMarkDescription();
	void SetViewLongitude();
	void SetViewLatitude();
	void SetViewRange();
	void SetViewHeading();
	void SetViewTilt();
	void SetCenterView();
	void SnapshotView();
	void ResetView();
	void IconChange();//by RSH 2013.7.25
	void IconScaleChange(int);
	void VideoPathChange();// 2014 2 6 by ugi
	void HtmlPathChange();// 2014 2 7 by ugi
	void SetIconPath();
	void SetVideoPath();
	void SetHtmlPath();
};

#endif // PLACEMARKDLG_H
