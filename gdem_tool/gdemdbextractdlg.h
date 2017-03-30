#ifndef GDEMDBEXTRACTDLG_H
#define GDEMDBEXTRACTDLG_H

#include <QDialog>
#include "ui_gdemdbextractdlg.h"

class GDemDbExtractDlg : public QDialog
{
	enum DataType
	{
		Image,
		Dem,
		Shape,
	};
	Q_OBJECT

public:
	GDemDbExtractDlg(QWidget *parent = 0);
	~GDemDbExtractDlg();

private:
	Ui::GDemDbExtractDlg ui;

	QString   _strInputPath;
	QString   _strSavePath;

	double    _left,_right,_top,_bottom;
	bool      _blWriteJgw;
	bool      _blBounding;
	DataType  _dataType;
	QString   _fileExtension;
	bool      _isRunning;

private slots:
	void browserInput();
	void browserSave();

	void extract();
	void cancel();

	void onToggleImage(bool);
	void onToggleDem(bool);
	void onToggleShape(bool);
	void onToggleJgw(bool);

	void onToggleBounding(bool);
	void onLeftChanged(const QString&);
	void onRightChanged(const QString&);
	void onTopChanged(const QString&);
	void onBottomChanged(const QString&);

	void onInputPathChanged(const QString&);
	void onSavePathChanged(const QString&);
private:
	bool extractOneGDemFile(QString  filename);
	bool intersectGDemFile(int x,int y,int level);
	bool intersectTile(int col,int row,int level);	

	void setRunnningState(bool state);
			
};

#endif // GDEMDBEXTRACTDLG_H
