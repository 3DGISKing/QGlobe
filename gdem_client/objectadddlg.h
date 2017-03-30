#ifndef OBJECTADDDLG_H
#define OBJECTADDDLG_H

#include <QDialog>

#include "GDM_CommandDlg.h"

class CGDM_ObjectAddCmd;

namespace Ui
{
	class ObjectAddDlg;
}


class ObjectAddDlg : public CGDM_CommandDlg
{
	Q_OBJECT

public:
	ObjectAddDlg(QWidget *parent = 0);
	~ObjectAddDlg();

	virtual void UpdateDialog();
private:
	Ui::ObjectAddDlg* ui;
	CGDM_ObjectAddCmd* m_pCommand; 
	
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
	void SetHeight();	   // Ôπ∂Ω§¿‚Ã© Ω¡¡Ï±ÀÃ© ≤⁄ÀÀ∂¶ ∏Í√˘ª§ø÷≥ﬁ.

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
	void SetHeight1(double newval);	 // Ôπ∂Ω§¿‚ªÙæÀÃ© ≤⁄ÀÀ∂¶ ∏Í√˘ª§ø÷≥ﬁ.
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
