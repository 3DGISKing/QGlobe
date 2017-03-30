#ifndef SMPBUILDINGPRODLG_H
#define SMPBUILDINGPRODLG_H

#include <QDialog>
#include <QSqlDatabase>
#include "ui_smpbuildingprodlg.h"

#include <QList>
#include "../gdem_tile/GDM_TileInfo.h"

class OGRFeature;
class OGRLayer;

class SmpBuildingProDlg : public QDialog
{
	Q_OBJECT

public:
	SmpBuildingProDlg(QWidget *parent = 0);
	~SmpBuildingProDlg();

private:
	Ui::SmpBuildingProDlg ui;
	
	QSqlDatabase      _sqliteDb;

	QList<OGRFeature*> _featureList;
	OGRLayer*          _layer;       
	QString            _savePath;
	int                _tolerancePixelNum;
	int                _locid;
	bool               _blCreatSQL;
	bool               _blUpdateBuildingTable;

private slots:
	void onClickedBrowserInput();
	void onClickedBrowserSave();
	void onClickedBrowserSqlite();
	void onClickedGenerate();
	void onToggleUpdateBuildingTb(bool);
	void onToggleCreateSql(bool);
	void onTextChangedTolerancePixel(const QString &);
	void onTextChangedLocID();
	
private:
	int  exportOneShapeTile(QList<OGRFeature*> & featurelist,GDM_TILE_INFO tileInfo);
	quint64  generateID(OGRFeature*);
	void updateBuildingTable();
	bool checkValidity();
	void createSql();
	void generateShpTile();
	void reportDuplicateIDError(QList<OGRFeature*> & featurelist);
	void customUpdateBuildingTableForPyongSong();
	void customCreateSqlForPyongSong();
};

#endif // SMPBUILDINGPRODLG_H
