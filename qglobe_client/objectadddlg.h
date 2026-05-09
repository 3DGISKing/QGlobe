#ifndef OBJECTADDDLG_H
#define OBJECTADDDLG_H

#include <QDialog>

#include "QGlobe_CommandDlg.h"

class CQGlobe_ObjectAddCmd;

namespace Ui
{
	class ObjectAddDlg;
}


class ObjectAddDlg : public CQGlobe_CommandDlg
{
	Q_OBJECT

public:
	ObjectAddDlg(QWidget *parent = 0);
	~ObjectAddDlg();

	virtual void UpdateDialog();
private:
	Ui::ObjectAddDlg* ui;
	CQGlobe_ObjectAddCmd* m_pCommand; 
	
protected:
	virtual void reject();
	virtual void keyPressEvent(QKeyEvent * event );
	void closeEvent(QCloseEvent *e);
private slots:
	void Open();
	void Flip();
	void Close();
	void Save();
	void copyDataFromNode();
	void copyDataToNode();
	void UpdateOutputInfo();
	void SetAzimuth();
	void SetHeight();	   //�ﹶ����̩ �����̩ ���˶� �������ֳ�.

	void SetLongitude(double newval);
	void SetLatitude(double newval);
	void SetPositionState();
	void SetDirectionState();
	void SetHeightState();
	void SetScalingAllAxis();
	void SetScalingXAxis();
	void SetScalingYAxis();
	void SetScalingZAxis();
	void SetScalingXYAxis();

	void SetRotateXAxis();
	void SetRotateYAxis();

	void SetLength(double newval);
	void SetHeight1(double newval);	 //�ﹶ�������̩ ���˶� �������ֳ�.
	void SetWidth(double newval);
	void SetBackGroundMove();
	void SetSelect();

	void SetMaterialType(int val);
	void SetDiffuseColor();
	void SetAmbientColor();
	void SetSpecularColor();
	void SetEmissiveColor();
	void SetEnableLighting(int state);
	void SetEnableFog(int state);
	void SetShininess(int val);


	void Rotate90AroundXAxis();
	void Rotate90AroundYAxis();
	void InvertXAxis();
	void Normalize();

	//Building Property
	void SetBuildingName(const QString& newtext);
	void SetPostName(const QString& newtext);
	void SetBan();
	void SetFloor();
	void SetMisc();

	void SetObjectType(int type);

	void OnProvinceChange(int id);
	void OnCountyChange(int id);
	void OnVilliageChange(int id);
	void DeleteObj();
	void SetImmediatelySave(int state);
	void SetMultiSelect(int state);
private:
	QColor m_DiffuseColor;
	QColor m_AmbientColor;
	QColor m_SpecularColor;
	QColor m_EmissiveColor;

	float  m_Shiniess;
	bool   m_LightingEnable;
	bool   m_FogEnable;

};

#endif // OBJECTADDDLG_H
