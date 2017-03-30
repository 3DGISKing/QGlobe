#ifndef OPTIONDLG_H
#define OPTIONDLG_H

#include <QtGui/QDialog>

namespace Ui {
    class OptionDlg;
}

class WorldMapScene;

class OptionDlg : public QDialog {
	friend class WorldMapScene;
    Q_OBJECT
public:
    OptionDlg(QWidget *parent = 0);
    ~OptionDlg();

protected:
    void changeEvent(QEvent *e);

protected:
	unsigned int	m_tabID;
	bool			m_toolTipChecked;
	bool			m_gisInfoChecked;
	unsigned int	m_lableSize;
	unsigned int	m_llShowMode;
	QFont			m_mainFont;
	unsigned int	m_distShowMode;
	unsigned int	m_topoQuality;
	double			m_heightMagnify;
	unsigned int	m_memCacheSize;
	unsigned int	m_diskCacheSize;
	double			m_tourTiltAngle;
	double			m_tourRange;
	double			m_tourSpeed;
	double			m_moveSpeed;
	unsigned int	m_wheelSpeed;
	bool			m_wheelDirection;

	bool            m_LightingEnabled;
	double          m_LightLongitude;  // in degree
	double          m_LightLatitude;   // in degree
	double          m_LightHeight;     // in kilometer

	QColor          m_LightColor;
	QColor          m_AmbientLightColor;
	double          m_LightAttenuation;

	double          m_CameraFOV;

	void InitDefaultValue();
	void GetParameter();
	void SetParameter();
	void UpdateOutputInfo();
	void SendFontInfo();

private:
    Ui::OptionDlg *m_ui;

	WorldMapScene *m_WorldMapScene;

private slots:
	virtual void accept();
	virtual void reject();

	void SetFocusOkButton();
	void SetToolTipChecked();
	void SetGISInfoChecked();
	void SetLableSize();
	void SetLLShowMode();
	void SetDistShowMode();
	//void SetMainFont();
	void SetTopoQuality(int value);
	void SetHeightMagnify(bool update = true);
	void SetMemoryCacheSize(bool update = true);
	void SetDiskCacheSize(bool update = true);
	void DeleteMemoryCache();
	void DeleteDiskCache();
	void SetTourTiltAngle(int value = -1);
	void SetTourRange(int value = -1);
	void SetTourSpeed(int value = -1);
	void SetMoveSpeed(int value = -1);
	void SetWheelSpeed(int value);
	void SetWheelDirection();

	void SetDefaultValue();
	void Apply();

	void SetLightColor();
	void SetLightAmbientColor();

	void SetLightLongitude();
	void SetLightLatitude();
	void SetLightHeight();

	void SetLightAttenuationConstant(double c);
	void SetEnableLighing(int state);

	void SetCameraFOV(int val);
};

#endif // OPTIONDLG_H
