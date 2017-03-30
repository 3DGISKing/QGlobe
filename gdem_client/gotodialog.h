#ifndef GOTODIALOG_H
#define GOTODIALOG_H

#include <QtGui/QDialog>
#include "ui_gotodialog.h"
#include <QtCore/QMetaType>

class GoToDialog : public QDialog
{
	Q_OBJECT

public:
	enum Unit
	{
		DecimalDegree,
		DegreeDecimalMinute,
		DegreeMinuteSecond
	};

	GoToDialog(QWidget *parent = 0);
	~GoToDialog();

private slots:
	void onClickedPushButtonClose();
	void onClickedPushButtonGoTo();
	void onEditingFinishedLongitude();
	void onEditingFinishedLatitude();
	void onEditingFinishedHeight();
	bool getGeographicValue(QString text,unsigned int type,double& val);
private:
	Ui::GoToDialog ui;

	double _longitude;
	double _latitude;
	double _height;
};

Q_DECLARE_METATYPE(GoToDialog::Unit)
#endif // GOTODIALOG_H
