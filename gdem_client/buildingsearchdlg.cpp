
#include "buildingsearchdlg.h"

#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlField>
#include <QSqlQueryModel>

#include "Building.h"
#include "gdemclient.h"
#include "guiuty.h"
#include "mainwindow.h"

#include "GDM_SceneManager.h"
#include "Geo3dObjectSceneNode.h"

#define GOTO_HEIGHT 300

class BuildingQueryModel :public QSqlQueryModel
{
public:
	QVariant data( const QModelIndex & index, int role = Qt::DisplayRole ) const
	{
		if(!index.isValid() || (role != Qt::DisplayRole && role != Qt::EditRole))
			return QVariant();

		if(index.column()==4)//Ban Field
		{
			QString banlist=QSqlQueryModel::data(index,role).toString();
			banlist.trimmed();

			QStringList list=banlist.split("|");

			QString res;

			for(int i=0;i<list.size();i++)
			{
				list[i].trimmed();
				if(list[i].isEmpty()) continue;
				if(!res.isEmpty() && i<list.size()-1)
					res=res+","+list[i];
				else
					res=res+list[i];
			}
			
			return QVariant(res);
		}
		else
			return QSqlQueryModel::data(index,role);
	}
};

BuildingSearchDlg::BuildingSearchDlg(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	
	model=NULL;
	connect(ui.pushButtonFind,SIGNAL(clicked()),this,SLOT(Find()));
	
	connect(ui.tableView,SIGNAL(doubleClicked(const QModelIndex&)),this,SLOT(Goto(const QModelIndex&)));
	connect(ui.comboBoxProvince,SIGNAL(currentIndexChanged(int)),this,SLOT(OnProvinceChange(int)));
	connect(ui.comboBoxCounty,SIGNAL(currentIndexChanged(int)),this,SLOT(OnCountyChange(int)));
	
	ui.spinBoxBan->setRange(0,MAX_BAN);
	ui.spinBoxFloor->setRange(0,BUILDING_MAX_FLOOR);
	ui.spinBoxFloor->setValue(0);


	QSqlQuery qry;

	qry.exec("SELECT loc_id, part1 FROM lst_location_name where level<=1 ORDER BY loc_id");
	while (qry.next())
		ui.comboBoxProvince->addItem(qry.value(1).toString(), qry.value(0).toInt());

	QPoint mainPos = g_app->GetMainWindow()->pos();
	move(mainPos.x()+20, mainPos.y()+50);
}

BuildingSearchDlg::~BuildingSearchDlg()
{
	if(model)
		delete model;
}

void BuildingSearchDlg::OnProvinceChange(int id)
{
	ui.comboBoxCounty->clear();
	int fromId = ui.comboBoxProvince->itemData(id).toInt();
	int toId = fromId + 9999;

	QSqlQuery qry;
	qry.exec("SELECT loc_id, part2 FROM lst_location_name where level<=2 and loc_id>=" + QString("%1").arg(fromId) + " and loc_id<=" + QString("%1").arg(toId) + " ORDER BY loc_id");
	while (qry.next())
		ui.comboBoxCounty->addItem(qry.value(1).toString(), qry.value(0).toInt());

	ui.comboBoxCounty->setCurrentIndex(0);
}

void BuildingSearchDlg::OnCountyChange(int id)
{
	ui.comboBoxVilliage->clear();
	int fromId = ui.comboBoxCounty->itemData(id).toInt();
	int toId = fromId + 99;

	QSqlQuery qry;
	qry.exec("SELECT loc_id, part3 FROM lst_location_name where loc_id>=" + QString("%1").arg(fromId) + " and loc_id<=" + QString("%1").arg(toId) + " ORDER BY loc_id");
	while (qry.next())
		ui.comboBoxVilliage->addItem(qry.value(1).toString(), qry.value(0).toInt());

	ui.comboBoxVilliage->setCurrentIndex(0);
}

void BuildingSearchDlg::Goto(const QModelIndex& index)
{
	int row=index.row();

	QSqlRecord record=model->record(row);

	double minlati=record.field("minlatitude").value().toDouble();
	double maxlati=record.field("maxlatitude").value().toDouble();
	double minlong=record.field("minlongitude").value().toDouble();
	double maxlong=record.field("maxlongitude").value().toDouble();

	double lat=(minlati+maxlati)/2.0;
	double longi=(minlong+maxlong)/2.0;

	longi=longi;
	lat=lat;

	PLACEMARK_DATA data;

	memset(&data,0,sizeof(PLACEMARK_DATA));

	data.view_longitude=longi;
	data.view_latitude=lat;
	data.orgDist=GOTO_HEIGHT;

	g_app->GetRenderProxy()->GetRenderSrv()->FastMoveToPlace(&data);

	qint64 id=record.field("id").value().toLongLong();
	g_app->GetRenderProxy()->GetRenderSrv()->m_pSceneMgr->m_pGeo3dObjectNode->SetSelectedID(id);
}

void BuildingSearchDlg::Find()
{
	QString filter = "";

	QString strVal=ui.lineEditName->text();

	if(!strVal.isEmpty()) {
		
		filter="(tb_building.name LIKE '%"+strVal+"%'";
		filter+=" OR tb_building.postname LIKE '%"+ strVal+"%')";
	}
	
	if(!FindByFloor().isEmpty())
		if(filter.isEmpty())
			filter=FindByFloor();
		else
			filter=filter+" AND " +FindByFloor();

	if(!FindByLocation().isEmpty())
		if(filter.isEmpty())
			filter=FindByLocation();
		else
			filter=filter +" AND "+FindByLocation();
	
	if(filter.isEmpty())
	{
		gu_MessageBox(this,tr("Error"),tr("empty search word!"),MSGBOX_ICONCRITICAL);
		return;
	}

	QString sql = "SELECT tb_building.id, tb_building.name, tb_building.postname,lst_location_name.full_name,";
				sql += "  tb_building.ban,tb_building.floor,tb_building.minlatitude, tb_building.minlongitude,";
				sql += "  tb_building.maxlatitude, tb_building.maxlongitude, tb_building.misc ";
	            sql += "  FROM tb_building, lst_location_name";
	            sql += "  WHERE " + filter + " AND (tb_building.loc_id = lst_location_name.loc_id)";
	
	Refresh(sql);
}

QString BuildingSearchDlg::FindByFloor()
{
	int floor =ui.spinBoxFloor->value();

	if(floor==0)
		return "";

	QString filter="(tb_building.floor="+QString::number(floor)+")";

	return filter; 
}

QString BuildingSearchDlg::FindByLocation()
{
	int villiageid=ui.comboBoxVilliage->itemData(ui.comboBoxVilliage->currentIndex()).toInt();
	int countyid=ui.comboBoxCounty->itemData(ui.comboBoxCounty->currentIndex()).toInt();
	int proviceid=ui.comboBoxProvince->itemData(ui.comboBoxProvince->currentIndex()).toInt();

	int ban=ui.spinBoxBan->value();

	QString filter;

	if(ui.comboBoxVilliage->currentIndex()!=0)
		filter="tb_building.loc_id="+QString::number(villiageid);	
	else if(ui.comboBoxCounty->currentIndex()!=0)
	{
		QString startid=QString::number(countyid);
		QString endid=QString::number(countyid+100);

		filter="tb_building.loc_id >="+startid+" AND tb_building.loc_id<="+endid;
	}
	else if(ui.comboBoxProvince->currentIndex()!=0)
	{
		QString startid=QString::number(proviceid);
		QString endid=QString::number(proviceid+10000);

		filter="tb_building.loc_id >="+startid+" AND tb_building.loc_id<="+endid;
	}

	if(ban>0 )
		if(!filter.isEmpty())
			filter=filter+" AND (tb_building.ban LIKE "+"'%|"+QString::number(ban)+"|%' OR tb_building.ban="+"'"+QString::number(ban)+"')" ;
		else
		{
			filter="(tb_building.ban LIKE '%|";
			filter+=QString::number(ban)+"|%' OR tb_building.ban="+"'"+QString::number(ban)+"')" ;

		}
	if(!filter.isEmpty())
		filter="("+filter+")";
	

	return filter;
}
  
void BuildingSearchDlg::Refresh(QString sql)
{
	ui.tableView->setModel(NULL);

	if(model)
		delete model;

	model = new BuildingQueryModel();
	model->setQuery(sql);

	int rowcount=model->rowCount();

	if(rowcount==0)
	{
		gu_MessageBox(this,tr("Search Result"),tr("No Result"),MSGBOX_ICONINFO);
		delete model;
		model=NULL;
		return;
	}

	ui.tableView->setModel(model);

	connect(ui.tableView->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
		this, SLOT(CurrentChanged()));

    // don't show the ID,minlongi,malongi,minlati,maxlat
	ui.tableView->setColumnHidden(0,true);
	ui.tableView->setColumnHidden(6,true);
	ui.tableView->setColumnHidden(7,true);
	ui.tableView->setColumnHidden(8,true);
	ui.tableView->setColumnHidden(9,true);

	model->setHeaderData(1, Qt::Horizontal, tr("Name"));
	model->setHeaderData(2, Qt::Horizontal, tr("PostNames"));
	model->setHeaderData(3, Qt::Horizontal, tr("Location"));
	model->setHeaderData(4, Qt::Horizontal, tr("Ban"));
	model->setHeaderData(5, Qt::Horizontal, tr("Floor"));
	model->setHeaderData(10,Qt::Horizontal,tr("Misc")); 
   
	ui.tableView->resizeColumnsToContents();

	g_app->GetRenderProxy()->GetRenderSrv()->m_pSceneMgr->m_pGeo3dObjectNode->ClearSerachedIDs();

	QList<qint64>& searchedlist=g_app->GetRenderProxy()->GetRenderSrv()->m_pSceneMgr->m_pGeo3dObjectNode->GetSearchedIds();

	for(int i=0;i<model->rowCount();i++)
	{
		qint64 id=model->record(i).field("id").value().toLongLong();
		searchedlist.push_back(id);
	}
}

void BuildingSearchDlg::reject()
{
	g_app->GetRenderProxy()->GetRenderSrv()->m_pSceneMgr->m_pGeo3dObjectNode->ClearSerachedIDs();
	g_app->GetMainWindow()->UpdatePlaceDlg(true);
	g_app->GetRenderProxy()->GetRenderSrv()->m_pSceneMgr->m_pGeo3dObjectNode->SetSelectedID(-1);
	QDialog::reject();
}
