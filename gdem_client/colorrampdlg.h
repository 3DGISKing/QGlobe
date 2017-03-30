#ifndef COLORRAMPDLG_H
#define COLORRAMPDLG_H

#include <QDialog>
#include "ui_colorrampdlg.h"

class IColorRamp;

class ColorRampDlg : public QDialog
{
	Q_OBJECT

public:
	ColorRampDlg(QWidget *parent,IColorRamp* colorramp);
	~ColorRampDlg();

	IColorRamp* GetColorRamp(){return _ColorRamp;}
private slots:
	void onClickedPushButtonOk();
	void onClickedPushButtonCancel(); 
	void onComboBoxChanged(int);

private:
	Ui::ColorRampDlg ui;
	
	IColorRamp*    _ColorRamp;
};

#endif // COLORRAMPDLG_H
