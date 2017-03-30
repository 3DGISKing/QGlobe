#ifndef OBJECTPLACEEDITDLG_H
#define OBJECTPLACEEDITDLG_H

#include <QtGui/QDialog>
#include "gdemclientbase.h"
#include "GDM_CommandDlg.h"

namespace Ui {
    class ObjectPlaceEditDlg;
}

class PointData;
class CGDM_DrawObjectCmd;
class PListTableModel;
//class PlaceMarkMgr;

class ObjectPlaceEditDlg : public CGDM_CommandDlg {
    Q_OBJECT
public:
	ObjectPlaceEditDlg(QWidget *parent = 0);
    ~ObjectPlaceEditDlg();
	Ui::ObjectPlaceEditDlg	*m_ui;

	bool IsEditMode() { return m_EditMode; }
	bool IsPropertyMode() { return m_PropertyMode; }
	void setModifyData(int tb_type, int id);
	void setPropertyData(int tb_type, int id);

private:
	bool					m_EditMode;
	int						m_shapeType;
	QStringList				m_tableNameList;
	QList<int>				m_typeList;

	int						m_objectID;
	CGDM_DrawObjectCmd		*m_pathCommand;

	PListTableModel			*m_pointListModel;

	QByteArray				m_editingPoints;

	bool					m_gisInfo;

	bool					m_PropertyMode;
	bool					m_EditStart;

public:
	virtual void UpdateDialog();
	virtual void showEvent(QShowEvent *event);

private:
	void InitDialog();
	void copyDataToNode(bool bRefresh = true);
	void copyDataFromNode();
	void GetNewPointData(PointData &pd, int curRow);
	void SetPlaceLatLong();
	QList<double> getMBR();
	QByteArray pointsWrite();
	void pointsRead(QByteArray &arr);
	bool checkInputData();
	bool saveObjectPlace();

	void setState(int state);
	void editClear();
	void renderFormRefresh();

	QString ValueToString(double value); //asd 2014.6.4
	double StringToValue(QString &str); //asd 2014.6.4

private slots:
	void btnModify_click();
	void btnNewAdd_click();
	void btnDelete_click();

	void cmbCategory_change(int idx);
	void cmbObjectPlace_change(int idx);
	void edtHtmlPath_change();
	void edtVideoPath_change();

	void AddPointData();
	void EditPointData();
	void ChangedPointData();
	void DelPointData();

	void btnSave_click();
	void btnCancel_click();
	void btnExit_click();

	void btnCenterPoint_click(bool isCenterPointEdit);

	void tabIndex_change(int id);

	void SetCenterMark();
};

#endif // OBJECTPLACEEDITDLG_H
