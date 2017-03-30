#ifndef OBJECTPLACESETTINGDLG_H
#define OBJECTPLACESETTINGDLG_H

#include <QtGui/QDialog>

namespace Ui {
    class ObjectPlaceSettingDlg;
}

class ObjectPlaceSettingDlg : public QDialog {
    Q_OBJECT
public:
	ObjectPlaceSettingDlg(QWidget *parent = 0, bool isAdd = true);
    ~ObjectPlaceSettingDlg();

private:
	Ui::ObjectPlaceSettingDlg	*m_ui;

	int				m_tbtype;
	int				m_shapeType;

	QString			m_lineColor;
	QString			m_fillColor;

	QList<QVariant>	m_savedData;

	void InitDialog();
	bool saveData();

public slots:
	virtual void accept();
	virtual void reject();

private slots:
	void apply();
	void refreshPreview();

	void cmbObject_change(int id);
	void cmbShapeType_change(int id);

	void btnIconPath_click();
	void spnIconSize_change(int val);
	void sldIconSize_change(int val);

	void btnLineColor_click();
	void spnLineThick_change(double val);
	void sldLineThick_change(int val);

	void btnFillColor_click();
	void btnTexturePath_click();
};

#endif // OBJECTPLACESETTINGDLG_H
