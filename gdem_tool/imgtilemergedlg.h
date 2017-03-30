#ifndef IMGTILEMERGEDLG_H
#define IMGTILEMERGEDLG_H

#include <QDialog>
#include "ui_imgtilemergedlg.h"

class ImgTileMergeDlg : public QDialog
{
	Q_OBJECT

public:
	ImgTileMergeDlg(QWidget *parent = 0);
	~ImgTileMergeDlg();

private:
	Ui::ImgTileMergeDlg ui;

	QString _inputPath,_savePath;
	int _minX,_minY,_maxX,_maxY;
	int _level;

private slots:
	void onClickedInput();
	void onClickedSave();
	void onClickedMerge();
private:
	void determineTileInfo(QString);
	void merge();
};

#endif // IMGTILEMERGEDLG_H
