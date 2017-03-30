#ifndef PATHDLG_H
#define PATHDLG_H

#include <QtGui/QDialog>
#include "gdemclientbase.h"
#include "GDM_CommandDlg.h"

#define PATHDLG_STR_MAX_SIZE	254

namespace Ui {
    class PathDlg;
}

class PlaceMarkMgr;
class CGDM_DrawPolygonCmd;
class PListTableModel;
class PointData;

class PathDlg : public CGDM_CommandDlg {
    Q_OBJECT
public:
	PathDlg(QWidget *parent = 0, bool isAdd = true);
    ~PathDlg();

private:
	Ui::PathDlg	*m_ui;

	unsigned short	m_pathName[PATHDLG_STR_MAX_SIZE];
	unsigned short	m_pathDesc[PATHDLG_STR_MAX_SIZE];
	
	bool			m_lineChecked;
	QColor			m_lineColor;
	float			m_lineThick;
	float			m_lineOpacity;
	bool			m_fillChecked;
	QColor			m_fillColor;
	float			m_fillOpacity;
	unsigned int	m_heightMode;
	double			m_heightFromEarth;
	bool			m_extended;
	PListTableModel	*m_pointListModel;

	double			m_length;
	bool			m_calcWithDem;
	double			m_lengthWithDem;
	double			m_maxHeight;
	double			m_maxHtLati;
	double			m_maxHtLongi;
	double			m_minHeight;
	double			m_minHtLati;
	double			m_minHtLongi;
	double			m_maxRate;
	double			m_maxRateLati;
	double			m_maxRateLongi;

private:
	PLACEMARK_DATA			m_placeMarkData;
	PlaceMarkMgr			*m_placeMarkMgr;
	bool					m_isAdd;
	unsigned int			m_state;
	CGDM_DrawPolygonCmd*	m_pCommand;
	bool					m_blNoDataCopy;

public:
	virtual void UpdateDialog();
	virtual void showEvent(QShowEvent *event);

private:
	void InitDialog();
	void UpdateOutputInfo();
	void EnableViewControl(bool enable);
	void copyDataToNode(bool bRefresh = true);
	void copyDataFromNode();
	void GetNewPointData(PointData &pd, int curRow);

public slots:
	virtual void accept();
	virtual void reject();

private slots:
	void SetPathName();
	void SetPathDescription();

	void SetFocusOkButton();
	void SetLatitude();
	void SetLongitude();
	void SetRange();
	void SetHeading();
	void SetTilt();
	void ViewCurrentSnapshot();
	void ResetView();

	void SetLineCheck();
	void SetLineColor();
	void SetLineThick();
	void SetLineOpacity();
	void SetFillCheck();
	void SetFillColor();
	void SetFillOpacity();
	void SetLineHeightMode(int mode);
	void SetLineHeight();
	void SetLineHeightWith(int val);
	void SetExtended();
	void AddPointData();
	void EditPointData();
	void ChangedPointData();
	void DelPointData();

	void SetCalcWithDem();
	void StartCalc();
	void UpdateLookAt();
	void OnSliderAct(int a_nAct);
};

#endif // PATHDLG_H
