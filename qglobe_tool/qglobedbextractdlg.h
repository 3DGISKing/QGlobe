#ifndef QGLOBE_DB_EXTRACT_DLG_H
#define QGLOBE_DB_EXTRACT_DLG_H

#include <QDialog>
#include "ui_qglobedbextractdlg.h"

class QGlobeDbExtractDlg : public QDialog
{
	enum DataType
	{
		Image,
		Dem,
		Shape,
	};
	Q_OBJECT

public:
	QGlobeDbExtractDlg(QWidget *parent = 0);
	~QGlobeDbExtractDlg();

private:
	Ui::QGlobeDbExtractDlg ui;

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
	bool extractOneQGlobeFile(QString  filename);
	bool intersectQGlobeFile(int x,int y,int level);
	bool intersectTile(int col,int row,int level);	

	void setRunnningState(bool state);
			
};

#endif // QGLOBE_DB_EXTRACT_DLG_H
