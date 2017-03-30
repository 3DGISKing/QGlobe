#include "smpbuildingprodlg.h"

#include <QFileDialog>
#include <QSqlQueryModel>
#include <QTextStream>
#include <QSqlQuery>

#include <ogr/ogr_api.h>
#include <ogr/ogrsf_frmts/ogrsf_frmts.h>

#include "common.h"
#include "../gdem_base/GDM_Rect.h"
#include "../gdem_base/GDM_Math.h"
#include "../gds_common/GDSDefine.h"	
#include "../gdem_libmain/building.h"	

#define DEFAULT_TOLERLANCE_PIXEL_NUMBER  40
#define ID_TOLERANCE                     0.00000001
#define SIMPLEBUILDING_DIR_PATH          "/geoobject/building/simple/%d/%d_%d_%d"

SmpBuildingProDlg::SmpBuildingProDlg(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	_tolerancePixelNum=DEFAULT_TOLERLANCE_PIXEL_NUMBER;
	_savePath=QDir::tempPath();
	ui.lineEditSavePath->setText(_savePath);
	ui.checkBoxUpdateBuildingTb->setEnabled(false);
	_sqliteDb=QSqlDatabase::addDatabase("QSQLITE");
	_locid=10000;
	ui.lineEditTolerancePixel->setText(QString::number(_tolerancePixelNum));
	ui.lineEditLocID->setText(QString::number(_locid));
	_blCreatSQL=false;
	_blUpdateBuildingTable=false;
	ui.pushButtonCancel->setVisible(false);
	ui.progressBar->setVisible(false);
	ui.labelInfo->setVisible(false);

	OGRRegisterAll();

	connect(ui.pushButtonBrowserInput,SIGNAL(clicked()),this,SLOT(onClickedBrowserInput()));
	connect(ui.pushButtonBrowserSave,SIGNAL(clicked()),this,SLOT(onClickedBrowserSave()));
	connect(ui.pushButtonBrowserSqlite,SIGNAL(clicked()),this,SLOT(onClickedBrowserSqlite()));

	connect(ui.checkBoxUpdateBuildingTb,SIGNAL(toggled(bool)),this,SLOT(onToggleUpdateBuildingTb(bool)));
	connect(ui.checkBoxCreateSql,SIGNAL(toggled(bool)),this,SLOT(onToggleCreateSql(bool)));

	connect(ui.pushButtonGenerate,SIGNAL(clicked()),this,SLOT(onClickedGenerate()));
	connect(ui.lineEditTolerancePixel,SIGNAL(textChanged(const QString&)),SLOT(onTextChangedTolerancePixel(const QString &)));
	connect(ui.lineEditLocID,SIGNAL(editingFinished()),SLOT(onTextChangedLocID()));
}
SmpBuildingProDlg::~SmpBuildingProDlg()
{

}

void SmpBuildingProDlg::onToggleUpdateBuildingTb(bool checked)
{
	if(checked)
		_blUpdateBuildingTable=true;
	else
		_blUpdateBuildingTable=false;
}

void SmpBuildingProDlg::onToggleCreateSql(bool checked)
{
	if(checked)
		_blCreatSQL=true;
	else
		_blCreatSQL=false;
}

void SmpBuildingProDlg::onClickedBrowserInput()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("Select Esri Shape File!"),
													QDir::rootPath(),
													tr("Esri Shape Files (*.shp)"));
	int pos=fileName.lastIndexOf("/");
	QString path=fileName.left(pos);

	OGRSFDriverH drv=OGRGetDriverByName("ESRI Shapefile");

	OGRDataSourceH datasrc;

	datasrc=OGROpen(path.toAscii(),0,&drv);
	if(!datasrc)
		return ;

	OGRDataSource* data=(OGRDataSource*)datasrc;

	QString layername=fileName.mid(pos+1,fileName.length()-pos-5);

	_layer=data->GetLayerByName(layername.toAscii());

	if(!checkValidity())
	{
		delete _layer;
		_layer=NULL;
		delete datasrc;
		return;
	}

	OGRFeature *feature;

	foreach(feature,_featureList)
		delete feature;

	_featureList.clear();

	for(int i=0;i<_layer->GetFeatureCount();i++)
	{
		OGRFeature* feature=_layer->GetFeature(i);
		_featureList.push_back(feature);
	}

	reportDuplicateIDError(_featureList);
	ui.lineEditInputPath->setText(fileName);
}

void SmpBuildingProDlg::onClickedBrowserSave()
{
	QString tmp=QFileDialog::getExistingDirectory(this,	tr("Select folder where will save result!"), _savePath);

	if (!tmp.isEmpty()) 
	{
	_savePath = tmp;
		ui.lineEditSavePath->setText(_savePath);
	}
}

void SmpBuildingProDlg::onClickedBrowserSqlite()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("Select Sqlite Db file!"),
														  QDir::rootPath(),
													      tr("All Files (*.*)"));
	_sqliteDb.setDatabaseName(fileName);

	if(!_sqliteDb.open())
	{
		messageBox("Failed to open sqlite db!");
		return;
	}
	else
	{
		ui.checkBoxUpdateBuildingTb->setEnabled(true);
		ui.lineEditSqliteDbPath->setText(fileName);
	}
}

int SmpBuildingProDlg::exportOneShapeTile(QList<OGRFeature*> & featurelist,GDM_TILE_INFO tileInfo)
{
	double tileWidth = gdm_TwoPI / (1 << tileInfo.nLevel);//À°ËÎÌ© ²¸¹¾¼¹¼³

	CGDM_Rect2D tileBox; //À°ËÎÌ© °Ò±Ë4°¢Âô°Î¼³
	tileBox.m_cMinPt.m_tX = tileWidth * tileInfo.nX - gdm_PI;
	tileBox.m_cMinPt.m_tY = tileWidth * tileInfo.nY - gdm_PI;
	tileBox.m_cMaxPt.m_tX = tileBox.m_cMinPt.m_tX + tileWidth;
	tileBox.m_cMaxPt.m_tY = tileBox.m_cMinPt.m_tY + tileWidth;

	unsigned int m = tileInfo.nX / TILE_ROW_COUNT_IN_BLOCK;
	unsigned int n = tileInfo.nY / TILE_ROW_COUNT_IN_BLOCK;

	QString path,filename;//´Ðµâ¹¢Ì® ÃùËÎËË¶§ÊÐ±¨
	path=_savePath+path.sprintf(SIMPLEBUILDING_DIR_PATH, tileInfo.nLevel, m, n,tileInfo.nLevel);
	filename=_savePath+filename.sprintf(SIMPLEBUILDING_FILE_PATH, tileInfo.nLevel, m, n,tileInfo.nLevel,tileInfo.nX,tileInfo.nY, tileInfo.nLevel);

	QDir dir;

	if(!dir.exists(path))//´Ðµâ¹¢°¡ ¼Á¼õÂ×¼è Ê±Ëº·² ´Ðµâ¹¢»¶ºÂ
		dir.mkpath(path);

	QFile file(filename);           
	if(!file.open(QIODevice::WriteOnly)) 	return 0;

	QDataStream out(&file);
	out.setByteOrder(QDataStream::LittleEndian);

	OGRFeature* feature;
	OGRMultiPolygon *multipolygon;
	OGRGeometry * geometry;
	OGREnvelope featureenvelop;

	int count=0;

	//write file header
	out<<SHP_FILE_CODE;  
	out<<0x00000000; //count of feature yet can not calculate!

	for(unsigned index = 0; index < featurelist.size(); index++) 
	{
		feature=featurelist[index];
		geometry=feature->GetGeometryRef();

		geometry->getEnvelope(&featureenvelop);

		double x=featureenvelop.MaxX+featureenvelop.MinX;
		double y=featureenvelop.MaxY+featureenvelop.MinY;

		x=x/2.0; y=y/2.0;

		if(!tileBox.isPointInside(x*gdm_DEGTORAD,y*gdm_DEGTORAD))
			continue;

		double area=(featureenvelop.MaxX-featureenvelop.MinX)*(featureenvelop.MaxY-featureenvelop.MinY)*gdm_DEGTORAD*gdm_DEGTORAD;

		double pixelarea=(tileWidth/256.0)*(tileWidth/256);
		double numpixels=area/(pixelarea);

		if(tileInfo.nLevel<THREED_MAX_LEVEL && numpixels<_tolerancePixelNum)
			continue;

		count++; 

		out<<featureenvelop.MinX*gdm_DEGTORAD;
		out<<featureenvelop.MinY*gdm_DEGTORAD;
		out<<featureenvelop.MaxX*gdm_DEGTORAD;
		out<<featureenvelop.MaxY*gdm_DEGTORAD;

		int floor=feature->GetFieldAsInteger("Floor");

		if(floor==0)	floor=1;

		out<<(long)floor;					   //write floor
		out<<(long)0;					     //write unused
		out<<(quint64)(generateID(feature)); //write id

		int pointnum=0;


		//don not remember that first point is the same last point in ring

		if(geometry->getGeometryType()==wkbMultiPolygon25D || geometry->getGeometryType()==wkbMultiPolygon)
		{
			multipolygon=(OGRMultiPolygon*)geometry;

			for(int i=0;i<multipolygon->getNumGeometries();i++)
			{
				OGRGeometry* geometry=multipolygon->getGeometryRef(i);

				OGRLinearRing* ring=NULL;

				if(geometry->getGeometryType()==wkbPolygon25D || geometry->getGeometryType()==wkbPolygon)
				{
					OGRPolygon*  poly=(OGRPolygon*)geometry;
					ring=poly->getExteriorRing();
				}
				else if(geometry->getGeometryType()==wkbLineString25D)
					ring=(OGRLinearRing*)geometry;
				else
					ring=NULL;

				pointnum+=ring->getNumPoints();
				pointnum--;
			}

			out<<(long)multipolygon->getNumGeometries();    //count of part
			out<<(long)pointnum;

			pointnum=0;

			//write part index
			for(int i=0;i<multipolygon->getNumGeometries();i++)
			{  
				out<<(long)pointnum;//start point index array  in each part
				OGRGeometry* geometry=multipolygon->getGeometryRef(i);

				OGRLinearRing* ring=NULL;

				if(geometry->getGeometryType()==wkbPolygon25D || geometry->getGeometryType()==wkbPolygon)
				{
					OGRPolygon*  poly=(OGRPolygon*)geometry;
					ring=poly->getExteriorRing();
				}
				if(geometry->getGeometryType()==wkbLineString25D)
					ring=(OGRLinearRing*)geometry;

				pointnum+=ring->getNumPoints();	
				pointnum--;
			}

			//write point data 
			for(int i=0;i<multipolygon->getNumGeometries();i++)
			{  
				OGRGeometry* geometry=multipolygon->getGeometryRef(i);

				OGRLinearRing* ring=NULL;

				if(geometry->getGeometryType()==wkbPolygon25D || geometry->getGeometryType()==wkbPolygon)
				{
					OGRPolygon*  poly=(OGRPolygon*)geometry;
					ring=poly->getExteriorRing();
				}

				if(geometry->getGeometryType()==wkbLineString25D)
					ring=(OGRLinearRing*)geometry;

				OGRPoint point;
				for(int j=0;j<ring->getNumPoints()-1;j++)
				{
					ring->getPoint(j,&point);

					out<<point.getX()*gdm_DEGTORAD;
					out<<point.getY()*gdm_DEGTORAD;
				}
			}//int i=0;i<multipolygon->getNumGeometries();i++
		}//if(geometry->getGeometryType()==wkbMultiPolygon25D)
		else if(geometry->getGeometryType()==wkbPolygon25D || geometry->getGeometryType()==wkbPolygon)
		{
			out<<(long)1;    //count of part

			OGRPolygon*  poly=(OGRPolygon*)geometry;

			OGRLinearRing* ring;
			ring=poly->getExteriorRing();

			out<<(long)ring->getNumPoints()-1;

			out<<(long)0;

			OGRPoint point;
			for(int j=0;j<ring->getNumPoints()-1;j++)
			{
				ring->getPoint(j,&point);

				out<<point.getX()*gdm_DEGTORAD;
				out<<point.getY()*gdm_DEGTORAD;
			}
		} 
		else
			qDebug("Error UnKnown Feature Type");

	}//index = 0; index < nCnt; index++

	if(count==0)
	{
		file.close();
		QFile::remove(filename);
		dir.rmdir(path);
		return 0;
	}

	file.seek(4);
	out<<count;	//write count of feature
	file.close();
	qDebug("count=%d X=%d,Y=%d,Level=%d \n",count,tileInfo.nX,tileInfo.nY,tileInfo.nLevel );
	return count;	
}


void SmpBuildingProDlg::onClickedGenerate()
{
	ui.labelInfo->setVisible(true);
	ui.pushButtonGenerate->setEnabled(false);
	ui.progressBar->setVisible(true);

	if(_blUpdateBuildingTable && _sqliteDb.open())
		updateBuildingTable();
		//customUpdateBuildingTableForPyongSong();

	generateShpTile();

	if(_blCreatSQL)
		createSql();
		//customCreateSqlForPyongSong();

	ui.labelInfo->setVisible(false);
	ui.pushButtonGenerate->setEnabled(true);
	ui.progressBar->setVisible(false);
	
}

bool SmpBuildingProDlg::checkValidity()
{
	OGRwkbGeometryType geomtype=_layer->GetGeomType();

	if(geomtype!=wkbPolygon && geomtype!=wkbPolygon25D)
	{
		messageBox("This layer is not polygon layer!");
		return false;
	}

	if(_layer->GetFeatureCount()==0)
	{
		messageBox("No feature exists in this layer!");
		return false; 
	}

	OGRFeatureDefn* fdn=_layer->GetLayerDefn();

	if(fdn->GetFieldIndex("name")==-1)
	{
		messageBox("There no exist name field!");
		return false;
	}

	if(fdn->GetFieldIndex("postname")==-1)
	{
		messageBox("There no exist postname field!");
		return false;
	}

	if(fdn->GetFieldIndex("floor")==-1)
	{
		messageBox("There no exist floor field!");
		return false;
	}
	
	if(fdn->GetFieldIndex("ban")==-1)
	{
		messageBox("There no exist ban field!");
		return false;
	}

	if(fdn->GetFieldIndex("misc")==-1)
	{
		messageBox("There no exist misc field!");
		return false;
	}

	return true;
}


void SmpBuildingProDlg::updateBuildingTable()
{
	for(int i=0;i<_featureList.size();i++)
	{
		OGRFeature* feature=_featureList[i];

		quint64 ID=generateID(feature);

		QString name=QString::fromLocal8Bit(feature->GetFieldAsString("name"));
		QString postname=QString::fromLocal8Bit(feature->GetFieldAsString("postname"));
		int floor=feature->GetFieldAsInteger("floor");
		QString banlist=feature->GetFieldAsString("ban");
		QString misc=QString::fromLocal8Bit(feature->GetFieldAsString("misc"));
		QString startdate="1980-01-01";
		OGREnvelope envelope;

		feature->GetGeometryRef()->getEnvelope(&envelope);

		QString strQuery;

		strQuery="INSERT INTO tb_building ( id , minlongitude,maxlongitude,minlatitude,maxlatitude,name, postname,loc_id,ban ,floor,startdate)";
		strQuery+=" VALUES                 ( %lld , %f          ,%f          ,%f         ,%f          ,'%s', '%s'    ,%d,   '%s',%d   ,'%s')";
		
		strQuery.sprintf(strQuery.toUtf8().data(),
			ID,
			envelope.MinX*gdm_DEGTORAD,
			envelope.MaxX*gdm_DEGTORAD,
			envelope.MinY*gdm_DEGTORAD,
			envelope.MaxY*gdm_DEGTORAD,
			name.toUtf8().data(),
			postname.toUtf8().data(),
			_locid,
			banlist.toUtf8().data(),
			floor,
			//misc.toUtf8().data());	 
			startdate.toUtf8().data());

			QSqlQueryModel oSqlQueryModel;
			oSqlQueryModel.setQuery(strQuery, _sqliteDb);
	}
}



void SmpBuildingProDlg::customUpdateBuildingTableForPyongSong()
{
	for(int i=0;i<_featureList.size();i++)
	{
		OGRFeature* feature=_featureList[i];

		quint64 ID=generateID(feature);

		QString name=QString::fromLocal8Bit(feature->GetFieldAsString("name"));
		QString postname=QString::fromLocal8Bit(feature->GetFieldAsString("postname"));
		int floor=feature->GetFieldAsInteger("floor");
		QString  misc=QString::fromLocal8Bit(feature->GetFieldAsString("misc"));

		//get address
		QString address=QString::fromLocal8Bit(feature->GetFieldAsString("Address"));
		address.trimmed();

		int loc_id=20100;//for PyongSong
		QString banlist;

		if(address.isEmpty())
		{
			qDebug("Edit Error: address is not specified FID=%d: ",i);
			loc_id=20100;//for PyongSong
		}
		else
		{
			QRegExp rx("\\s+");
			QStringList strlist=address.split(rx);

			QString full_name;

			if(strlist.size()>3)
			{
				for(int j=3;j<strlist.size();j++)
					banlist=banlist+" "+strlist[j];

				full_name=strlist[0]+" "+strlist[1]+" "+strlist[2];
			}
			else if(strlist.size()==3)
			{
				qDebug("Edit Error: ban is not specified FID=%d: ",i);
				full_name=strlist[0]+" "+strlist[1]+" "+strlist[2];
			}
			else if(strlist.size()==2)
			{
				qDebug("Edit Error: address is not correct FID=%d: ",i);
				full_name=strlist[0]+" "+strlist[1];
			}
			else
			{
				qDebug("Error in Address field FID=%d",i);
				continue;
			}

			QSqlQuery qry;

			QString query;
			query.sprintf("SELECT loc_id FROM tb_location_name WHERE full_name LIKE '%s'",full_name.toUtf8().data());

			qry.exec(query);

			if(qry.next())
				loc_id=qry.value(0).toInt();
			else
			{
				qDebug("Error can not find address %s",full_name.toUtf8().data());
				continue;
			}
		}

		OGREnvelope envelope;

		feature->GetGeometryRef()->getEnvelope(&envelope);

		QString strQuery;

		strQuery="INSERT INTO tb_building ( id , minlongitude,maxlongitude,minlatitude,maxlatitude,name, postname,loc_id,ban ,floor,misc)";
		strQuery+=" VALUES                 ( %lld , %f          ,%f          ,%f         ,%f          ,'%s', '%s'    ,%d,   '%s',%d   ,'%s')";

		strQuery.sprintf(strQuery.toUtf8().data(),
						ID,
						envelope.MinX*gdm_DEGTORAD,
						envelope.MaxX*gdm_DEGTORAD,
						envelope.MinY*gdm_DEGTORAD,
						envelope.MaxY*gdm_DEGTORAD,
						name.toUtf8().data(),
						postname.toUtf8().data(),
						loc_id,
						banlist.toUtf8().data(),
						floor,
						misc.toUtf8().data());	 

		QSqlQueryModel oSqlQueryModel;
		oSqlQueryModel.setQuery(strQuery, _sqliteDb);
	}
}

void SmpBuildingProDlg::customCreateSqlForPyongSong()
{
	QFile file(_savePath+"/sql.txt");

	if (!file.open(QFile::WriteOnly))
	{
		messageBox("Faild to creat sql.txt");
		return;
	}

	QTextStream out(&file);

	for(int i=0;i<_featureList.size();i++)
	{
		OGRFeature* feature=_featureList[i];

		quint64 ID=generateID(feature);

		QString name=QString::fromLocal8Bit(feature->GetFieldAsString("name"));
		QString postname=QString::fromLocal8Bit(feature->GetFieldAsString("postname"));
		int floor=feature->GetFieldAsInteger("floor");
		QString  misc=QString::fromLocal8Bit(feature->GetFieldAsString("misc"));

		//get address
		QString address=QString::fromLocal8Bit(feature->GetFieldAsString("Address"));
		address.trimmed();

		int loc_id=20100;//for PyongSong
		QString banlist;

		if(address.isEmpty())
		{
			qDebug("Edit Error: address is not specified FID=%d: ",i);
			loc_id=20100;//for PyongSong
		}
		else
		{
			QRegExp rx("\\s+");
			QStringList strlist=address.split(rx);

			QString full_name;

			if(strlist.size()>3)
			{
				for(int j=3;j<strlist.size();j++)
					banlist=banlist+" "+strlist[j];

				full_name=strlist[0]+" "+strlist[1]+" "+strlist[2];
			}
			else if(strlist.size()==3)
			{
				qDebug("Edit Error: ban is not specified FID=%d: ",i);
				full_name=strlist[0]+" "+strlist[1]+" "+strlist[2];
			}
			else if(strlist.size()==2)
			{
				qDebug("Edit Error: address is not correct FID=%d: ",i);
				full_name=strlist[0]+" "+strlist[1];
			}
			else
			{
				qDebug("Error in Address field FID=%d",i);
				continue;
			}

			QSqlQuery qry;

			QString query;
			query.sprintf("SELECT loc_id FROM tb_location_name WHERE full_name LIKE '%s'",full_name.toUtf8().data());

			qry.exec(query);

			if(qry.next())
				loc_id=qry.value(0).toInt();
			else
			{
				qDebug("Error can not find address %s",full_name.toUtf8().data());
				continue;
			}
		}

		OGREnvelope envelope;

		feature->GetGeometryRef()->getEnvelope(&envelope);

		QString strQuery;

		strQuery="INSERT INTO tb_building ( id , minlongitude,maxlongitude,minlatitude,maxlatitude,name, postname,loc_id,ban ,floor,misc)";
		strQuery+=" VALUES                 ( %lld , %f          ,%f          ,%f         ,%f          ,'%s', '%s'    ,%d,   '%s',%d   ,'%s')";

		strQuery.sprintf(strQuery.toUtf8().data(),
							ID,
							envelope.MinX*gdm_DEGTORAD,
							envelope.MaxX*gdm_DEGTORAD,
							envelope.MinY*gdm_DEGTORAD,
							envelope.MaxY*gdm_DEGTORAD,
							name.toUtf8().data(),
							postname.toUtf8().data(),
							loc_id,
							banlist.toUtf8().data(),
							floor,
							misc.toUtf8().data());	 

		out<<strQuery<<"\n";
	}

	file.close();
}

void SmpBuildingProDlg::createSql()
{
	QFile file(_savePath+"/sql.txt");

	if (!file.open(QFile::WriteOnly))
	{
		messageBox("Faild to creat sql.txt");
		return;
	}

	QTextStream out(&file);

	for(int i=0;i<_featureList.size();i++)
	{
		OGRFeature* feature=_featureList[i];

		quint64 ID=generateID(feature);

		QString name=QString::fromLocal8Bit(feature->GetFieldAsString("name"));
		QString postname=QString::fromLocal8Bit(feature->GetFieldAsString("postname"));
		int floor=feature->GetFieldAsInteger("floor");
		QString banlist=QString::fromLocal8Bit(feature->GetFieldAsString("ban"));
		QString misc=QString::fromLocal8Bit(feature->GetFieldAsString("misc"));
		QString startdate="1980-01-01";//in sunchon
		OGREnvelope envelope;

		feature->GetGeometryRef()->getEnvelope(&envelope);

		QString strQuery;

		strQuery="INSERT INTO tb_building ( id , minlongitude,maxlongitude,minlatitude,maxlatitude,name, postname,loc_id,ban ,floor,startdate)";
		strQuery+=" VALUES                 ( %lld , %f          ,%f          ,%f         ,%f          ,'%s', '%s'    ,%d,   '%s',%d   ,'%s')";

		strQuery.sprintf(strQuery.toUtf8().data(),
			ID,
			envelope.MinX*gdm_DEGTORAD,
			envelope.MaxX*gdm_DEGTORAD,
			envelope.MinY*gdm_DEGTORAD,
			envelope.MaxY*gdm_DEGTORAD,
			name.toUtf8().data(),
			postname.toUtf8().data(),
			_locid,
			banlist.toUtf8().data(),
			floor,
			startdate.toUtf8().data());
			//misc.toUtf8().data());	 

		out<<strQuery<<"\n";
	}
	file.close();
}
void SmpBuildingProDlg::generateShpTile()	
{
	double tileWidth;
	GDM_TILE_INFO	startInfo, endInfo,tileInfo;
	int tilecount,exportedcount;

	for(int level=THREED_MIN_LEVEL;level<=THREED_MAX_LEVEL;level++)
	{
		ui.labelInfo->setText(QString("Proccessing level %1...").arg(level));

		tileWidth = gdm_TwoPI / (1 << level);

		OGREnvelope Bound;   

		_layer->GetExtent(&Bound);

		startInfo.nX = (int)((Bound.MinX*gdm_DEGTORAD + gdm_PI) / tileWidth);
		startInfo.nY = (int)((Bound.MinY*gdm_DEGTORAD + gdm_PI) / tileWidth);
		endInfo.nX = (int)((Bound.MaxX*gdm_DEGTORAD + gdm_PI) / tileWidth);
		endInfo.nY = (int)((Bound.MaxY*gdm_DEGTORAD + gdm_PI) / tileWidth);

		tilecount = (endInfo.nX - startInfo.nX + 1) * (endInfo.nY - startInfo.nY + 1);

		exportedcount=0;

		ui.progressBar->setRange(0,tilecount-1);

		for(int nTile = 0; nTile < tilecount; nTile++)
		{
			qApp->processEvents();
			ui.progressBar->setValue(nTile);

			tileInfo.nLevel = level;
			tileInfo.nX     = nTile % (endInfo.nX - startInfo.nX + 1) + startInfo.nX;
			tileInfo.nY		= nTile / (endInfo.nX - startInfo.nX + 1) + startInfo.nY;

			exportedcount+=exportOneShapeTile(_featureList,tileInfo);

		} //int nTile = 0;

		qDebug("Exported count %d at Level %d",exportedcount,level);
	} //int level
}
void SmpBuildingProDlg::onTextChangedLocID()
{
	bool ok;

	QString text=ui.lineEditLocID->text();
	int tmp=text.toInt(&ok);

	if(!ok)
	{
		ui.lineEditLocID->selectAll();
		messageBox("Invalid!");
		return;
	}

	if(tmp<10000 || tmp>130211)
	{

		ui.lineEditLocID->selectAll();
		messageBox("Invalid!");
		return;
	}

	_locid=tmp;
}

void SmpBuildingProDlg::onTextChangedTolerancePixel(const QString & text)
{
	bool ok;

	int tmp=text.toInt(&ok);

	if(!ok)
	{
		ui.lineEditTolerancePixel->selectAll();
		messageBox("Invalid!");
		return;
	}

	if(tmp<0 || tmp>256*256)
	{
		ui.lineEditTolerancePixel->selectAll();
		messageBox("Invalid!");
		return;
	}
	_tolerancePixelNum=tmp;
}
quint64 SmpBuildingProDlg::generateID(OGRFeature* feature)
{
	OGREnvelope featureenvelop;
	OGRGeometry *  geometry=feature->GetGeometryRef();
	geometry->getEnvelope(&featureenvelop);

	if(geometry==NULL)
	{
		//insert exception code
	}

	double x=featureenvelop.MaxX+featureenvelop.MinX;
	x=x/2;

	double y=featureenvelop.MaxY+featureenvelop.MinY;
	y=y/2;

	x=x*gdm_DEGTORAD;
	y=y*gdm_DEGTORAD;

	quint64 ix=x/ID_TOLERANCE;
	quint64 iy=y/ID_TOLERANCE;

	quint64 id;

	id=ix*pow(10.0,9);
	id=id+iy;

	return id;
}

void SmpBuildingProDlg::reportDuplicateIDError(QList<OGRFeature*> & featurelist)
{
	QList <quint64> idlist; 
	for(unsigned index = 0; index < featurelist.size(); index++) 
	{
		OGRFeature* feature;
		feature=featurelist[index];

		quint64 id=generateID(feature);

		idlist.push_back(id);

		bool writed=false;

		for(int i=0;i<idlist.size()-1;i++)
		{
			if(id==idlist[i])
			{
				if(!writed)
				{
					qDebug("FID=%d ,generated id=%lld",index, id);
					writed=true;
				}
				qDebug("     Error: founded duplicated id with FID=%d",i);
			}
		}
	}
}

