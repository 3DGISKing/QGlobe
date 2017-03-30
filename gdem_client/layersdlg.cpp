#include "layersdlg.h"
#include "ui_layersdlg.h"
#include "gdemclient.h"
#include "sidebar.h"
#include "config.h"
#include "layerstreeview.h"
#include "treeitemdelegate.h"
#include "layertreeitem.h"
#include "Layer.h"
#include "../gdem_scene/GIS_TextureIDCache.h"
#include "GDM_DataMgr.h"
#include "GDM_PyramidMgr.h"
#include "RasterRenderer.h"

LayersDlg::LayersDlg(QWidget *parent) :
	QDialog(parent),
	m_ui(new Ui::LayersDlg)
{
	m_ui->setupUi(this);
	m_parent = (SideBar*)parent;
	m_Timer.start(1000,this);
	m_layersView=NULL;
	m_itemModel=NULL;
	m_frameLayout=NULL;
}

LayersDlg::~LayersDlg()
{
	delete m_ui;
}

bool LayersDlg::Init()
{
	setContentsMargins(0,0,0,BOTTOM_MARGIN);
	if (!CreateFrame())
		return false;

	return true;
}

void LayersDlg::Release()
{
	DestroyFrame();
}

void LayersDlg::UpdateDialog()
{
	bool blterrain=g_app->GetConfig()->m_blTerrain;

	if(blterrain)
		m_TerrainItem->PropagateStateToExclusiveChild(Qt::Checked);
	else
		m_TerrainItem->PropagateStateToExclusiveChild(Qt::Unchecked);

	unsigned state = g_app->GetConfig()->m_layerTopography;

	if(state==LTM_TEXTURE)
	{
		m_ImageItem->PropagateStateToExclusiveChild(Qt::Checked);
		//m_ImageItem->PropagateEnabledState(true);
	}
	else
	{
		m_ImageItem->PropagateStateToExclusiveChild(Qt::Unchecked);
		//m_ImageItem->PropagateEnabledState(false);
	}

	if(state==LTM_RASTER)
	{
		m_RasterItem->PropagateStateToExclusiveChild(Qt::Checked);
		//m_ImageItem->PropagateEnabledState(true);
	}
	else
	{
		m_RasterItem->PropagateStateToExclusiveChild(Qt::Unchecked);
		//m_ImageItem->PropagateEnabledState(false);
	}

	g_app->GetRenderProxy()->SendMessage(PM_LAYER_TEXTURE_STATE, (PrxParam)&state);

	g_app->GetRenderProxy()->SendMessage(PM_TERRAIN_LAYER_STATE, (PrxParam)&blterrain);

	bool blcontour=g_app->GetConfig()->m_blContour;

	g_app->GetRenderProxy()->SendMessage(PM_UPDATE_CONTOUR_STATE, (PrxParam)&blcontour);

	gdm_GetGISTextureIDCache()->Clear();
	gdm_GetDataMgr()->SetModifyFlag();
}

void LayersDlg::KeyPressEventOfTree(QModelIndex &index)
{
	LayerTreeItem *item = (LayerTreeItem*)m_itemModel->itemFromIndex(index);
	if (!item)
		return;

	if (item->checkState() == Qt::Checked)
		item->SetState(Qt::Unchecked);
	else
		item->SetState(Qt::Checked);

	OnItemClicked(index);
}

// empty function for ignore esc key

void LayersDlg::reject()
{
}

bool LayersDlg::CreateFrame()
{
	QString path = g_app->GetResource("/sidebar/");

	m_frameLayout = new QVBoxLayout;
	if (!m_frameLayout)
		return false;

	m_layersView = new LayersTreeView(this);
	if (!m_layersView)
		return false;

	m_layersView->setHeaderHidden(true);
	m_layersView->setWordWrap(true);

	m_itemModel = new QStandardItemModel;
	if (!m_itemModel)
		return false;

	m_layersView->setModel(m_itemModel);
	m_layersView->setItemDelegate(new TreeItemDelegate(this));

	m_frameLayout->addWidget(m_layersView);
	m_frameLayout->setContentsMargins(5,0,5,0);

	setLayout(m_frameLayout);
	
	if (!AddItems())
		return false;

	connect(m_layersView, SIGNAL(clicked(QModelIndex)), this, SLOT(OnItemClicked(QModelIndex)));
	connect(m_layersView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(OnItemDoubleClicked(QModelIndex)));
	
	g_app->GetRenderProxy()->SendMessage(PM_DATA_THREAD_INIT);

	return true;
}

void LayersDlg::DestroyFrame()
{
	if (m_layersView)
	{
		delete m_layersView;
		m_layersView = NULL;
	}

	if (m_itemModel)
	{
		delete m_itemModel;
		m_itemModel = NULL;
	}

	if (m_frameLayout)
	{
		delete m_frameLayout;
		m_frameLayout = NULL;
	}
}

bool LayersDlg::AddItems()
{
	if (!AddTerrainItem())
		return false;

	if (!AddRenderItem())
		return false;

	if (!AddRasterItems())
		return false;

	//ÃÔ¼³¼Ú»ª¼è ±ýµ¹,ºã´ª,´ª,»¤,°ë,¶®
	if(!AddAdminCenters())
		return false;

	if(!AddRevolItems())
		return false;

	//º¤,µß °¬,Âöºã ¸Æ³Þ,º¿,¸É´ª,¶í Âß°é,°©·Í,¼«ºã¼è Êñ½ç,ÊÀºã,ÁåÁä ³Þ¶®,Ê·°º,½è³Þ¶®
	if(!AddPlaceNames())
		return false;

	if(!AddSeaPlaceNames())
		return false;

	//±ýµ¹,´ª,»¤,°ë,¶®,°Ò±Ë
	if(!AddAdminBoundary())
		return false;
	
	

	//Ë©ºã¶ö
	if(!AddTransportNetwork())
		return false;

	//ºã±Ë¶ö
	if(!AddWaterNetwork())
		return false;

	//´Ý¼è¶ö
	if(!AddLanduseBoundary())
		return false;

	//3½ÓÌº Êï¹¶½¤Àâ
	if(!Add3dObject())
		return false;

	return true;
}

bool LayersDlg::AddTerrainItem()
{
	LayerTreeItem* root=RootItem();

	QString icon = g_app->GetResource("/sidebar/layers-terrain.png");
	m_TerrainItem=new LayerTreeItem(tr("Terrain"),icon);
	
	root->appendRow(m_TerrainItem);

	TerrainLayer* layer=new TerrainLayer(E_GDM_SUBDATA_TYPE_DEM,("ASTER GDEM 1"));
	layer->SetDataPath("geodem/astergdem1");

	LayerTreeItem *item=new LayerTreeItem(layer);
	m_TerrainItem->appendRow(item);

	m_TerrainItem->SetExclusiveChildItem(item);

	m_TerrainItem->PropagateStateToExclusiveChild(Qt::Checked);
	m_layersView->setExpanded(m_TerrainItem->index(),true);

	return true;
}

bool LayersDlg::AddRenderItem()
{
	LayerTreeItem* root=RootItem();

	QString icon = g_app->GetResource("/sidebar/layers-topography-mode.png");
	m_ImageItem=new LayerTreeItem(tr("Image"),icon);
	
	root->appendRow(m_ImageItem);

	ImageLayer* layer=new ImageLayer(E_GDM_SUBDATA_TYPE_IMG,tr("Google Image 2007"));
	layer->SetDataPath("geotexture/google2007");
	
	LayerTreeItem *item=new LayerTreeItem(layer);
	m_ImageItem->appendRow(item);
	m_ImageItem->SetExclusiveChildItem(item);

	layer=new ImageLayer(E_GDM_SUBDATA_TYPE_IMG2011,tr("Google Image 2011"));
	layer->SetDataPath("geotexture/google2011");

	item=new LayerTreeItem(layer);
	m_ImageItem->appendRow(item);

	layer=new ImageLayer(E_GDM_SUBDATA_TYPE_IMG_NASA,tr("NASA"));
	layer->SetDataPath("geotexture/nasa");

	item=new LayerTreeItem(layer);
	m_ImageItem->appendRow(item);

	m_ImageItem->SetCheckedItemMustBeExist(true);
	m_ImageItem->PropagateStateToExclusiveChild(Qt::Checked);
	m_layersView->setExpanded(m_ImageItem->index(),true);

	return true;
}

bool LayersDlg::AddAdminCenters()
{
	LayerTreeItem* root=RootItem();

	QString iconName = g_app->GetResource("/sidebar/layers-placename_border.png");
	LayerTreeItem* admincenter=new LayerTreeItem(tr("AdminArea"),iconName);
  
	root->appendRow(admincenter);

	PlaceNameLayer* layer=new PlaceNameLayer(E_GDM_SUBDATA_TYPE_NAME_COUNTRY,tr("Country"));
	layer->SetProperty("tb_country");
	LayerTreeItem *item=new LayerTreeItem(layer);
	admincenter->appendRow(item);

	layer=new PlaceNameLayer(E_GDM_SUBDATA_TYPE_NAME_POPULAR,tr("Capital"));
	layer->SetProperty("tb_popular");
	item=new LayerTreeItem(layer);
	admincenter->appendRow(item);

	layer=new PlaceNameLayer(E_GDM_SUBDATA_TYPE_NAME_FIRSTADMIN,tr("First Level Admin Name"));
	layer->SetProperty("tb_firstadmin");
	item=new LayerTreeItem(layer);
	admincenter->appendRow(item);
	
	layer=new PlaceNameLayer(E_GDM_SUBDATA_TYPE_NAME_SECONDADMIN,tr("Second Level Admin Name"));
	layer->SetProperty("tb_secondadmin");
	item=new LayerTreeItem(layer);
	admincenter->appendRow(item);

	layer=new PlaceNameLayer(E_GDM_SUBDATA_TYPE_NAME_THIRDADMIN,tr("Third Level Admin Name"));

	layer->SetProperty("tb_thirdadmin");
	item=new LayerTreeItem(layer);
	admincenter->appendRow(item);

	m_layersView->setExpanded(admincenter->index(),true);

	admincenter->PropagateStateToAllChilds(Qt::Checked);

	return true;
}

bool LayersDlg::AddSocialPlaceNames()
{
	/*LayerTreeItem* root=RootItem();

	QString iconName = g_app->GetResource("/sidebar/layers-placename_border.png");
	LayerTreeItem* admincenter=new LayerTreeItem(tr("SocialEcological"));

	root->appendRow(admincenter);

	iconName = g_app->GetResource("/sidebar/layers-country-name.png");
	PlaceNameLayer* layer=new PlaceNameLayer(E_GDM_SUBDATA_TYPE_NAME_FACTORY,tr("Factory"));

	LayerTreeItem *item=new LayerTreeItem(layer);
	admincenter->appendRow(item);

	iconName = g_app->GetResource("/sidebar/layers-popular-name.png");
	layer=new PlaceNameLayer(E_GDM_SUBDATA_TYPE_NAME_MINE,tr("Mine"));

	item=new LayerTreeItem(layer);
	admincenter->appendRow(item);

	iconName = g_app->GetResource("/sidebar/layers-popular-name.png");
	layer=new PlaceNameLayer(E_GDM_SUBDATA_TYPE_NAME_FARM,tr("Farm"));

	item=new LayerTreeItem(layer);
	admincenter->appendRow(item);

	iconName = g_app->GetResource("/sidebar/layers-1st-name.png");
	layer=new PlaceNameLayer(E_GDM_SUBDATA_TYPE_NAME_SCHOOL,tr("School"));

	item=new LayerTreeItem(layer);
	admincenter->appendRow(item);

	iconName = g_app->GetResource("/sidebar/layers-2nd-name.png");
	layer=new PlaceNameLayer(E_GDM_SUBDATA_TYPE_NAME_HOSPIATL,tr("Hospital"));

	item=new LayerTreeItem(layer);
	admincenter->appendRow(item);

	iconName = g_app->GetResource("/scene/city.png");
	layer=new PlaceNameLayer(E_GDM_SUBDATA_TYPE_NAME_STATION,tr("Station"));

	item=new LayerTreeItem(layer);
	admincenter->appendRow(item);

	layer=new PlaceNameLayer(E_GDM_SUBDATA_TYPE_NAME_AIRFIELD,tr("AirField"));
	layer->SetProperty("tb_airfield");
	item=new LayerTreeItem(layer);

	admincenter->appendRow(item);

	iconName = g_app->GetResource("/scene/city.png");
	layer=new PlaceNameLayer(E_GDM_SUBDATA_TYPE_NAME_SERVICE,tr("Service"));

	item=new LayerTreeItem(layer);
	admincenter->appendRow(item);

	m_layersView->setExpanded(admincenter->index(),true);

	admincenter->PropagateStateToAllChilds(Qt::Checked);*/

	return true;
}

bool LayersDlg::AddPlaceNames()
{
	LayerTreeItem* root=RootItem();

	QString iconName = "/scene/mountains.png";
	LayerTreeItem* placenames=new LayerTreeItem(tr("Place Name"),iconName);
	root->appendRow(placenames);

	PlaceNameLayer* layer=new PlaceNameLayer(E_GDM_SUBDATA_TYPE_NAME_MOUNTAIN,tr("Mountain,Ridge"),iconName);
	layer->SetProperty("tb_mountain");

	LayerTreeItem *item=new LayerTreeItem(layer);
	placenames->appendRow(item);

	layer=new PlaceNameLayer(E_GDM_SUBDATA_TYPE_NAME_SEA,tr("Sea"));
	layer->SetProperty("tb_sea");

	item=new LayerTreeItem(layer);
	placenames->appendRow(item);

	iconName = "/scene/river.png";
	layer=new PlaceNameLayer(E_GDM_SUBDATA_TYPE_NAME_RIVER,tr("River"),iconName);
	layer->SetProperty("tb_river");

	item=new LayerTreeItem(layer);
	placenames->appendRow(item);

	layer=new PlaceNameLayer(E_GDM_SUBDATA_TYPE_NAME_ISLAND,tr("Island"));
	layer->SetProperty("tb_Island");

	item=new LayerTreeItem(layer);
	placenames->appendRow(item);

	layer=new PlaceNameLayer(E_GDM_SUBDATA_TYPE_NAME_AIRPORT,tr("Airport"));
	layer->SetProperty("tb_airport");

	item=new LayerTreeItem(layer);
	placenames->appendRow(item);

	m_layersView->setExpanded(placenames->index(),true);
	
	placenames->PropagateStateToAllChilds(Qt::Checked);

	return true;
}

bool LayersDlg::AddRevolItems()
{
	LayerTreeItem* root=RootItem();

	LayerTreeItem* placenames=new LayerTreeItem(tr("Revolutionary "));
	root->appendRow(placenames);

	QString iconName = "/sidebar/layers-bronze.png";

	PlaceNameLayer* layer=new PlaceNameLayer(E_GDM_SUBDATA_TYPE_NAME_BROMZEITEM,tr("Bronze"),iconName);
	layer->SetProperty("tb_lm_bronze");

	LayerTreeItem *item=new LayerTreeItem(layer);
	placenames->appendRow(item);

	iconName = "/sidebar/layers-historical.png";
	layer=new PlaceNameLayer(E_GDM_SUBDATA_TYPE_NAME_HISTORICSITEITEM,tr("Historical"),iconName);
	layer->SetProperty("tb_lm_historysite");

	item=new LayerTreeItem(layer);
	placenames->appendRow(item);

	iconName = "/sidebar/layers-battle.png";
	layer=new PlaceNameLayer(E_GDM_SUBDATA_TYPE_NAME_BATTLESITEITEM,tr("Battle"),iconName);
	layer->SetProperty("tb_lm_battlesite");

	item=new LayerTreeItem(layer);
	placenames->appendRow(item);

	iconName = "/sidebar/layers-monument.png";
	layer=new PlaceNameLayer(E_GDM_SUBDATA_TYPE_NAME_MONUMENTITEM,tr("Monument"),iconName);
	layer->SetProperty("tb_lm_monument");

	item=new LayerTreeItem(layer);
	placenames->appendRow(item);

	iconName = "/sidebar/layers-museum.png";
	layer=new PlaceNameLayer(E_GDM_SUBDATA_TYPE_NAME_MUSEUMITEM,tr("Museum"),iconName);
	layer->SetProperty("tb_lm_museum");

	item=new LayerTreeItem(layer);
	placenames->appendRow(item);

	iconName = "/sidebar/layers-position.png";
	layer=new PlaceNameLayer(E_GDM_SUBDATA_TYPE_NAME_POSITIONITEM,tr("Position"),iconName);
	layer->SetProperty("tb_lm_positionsite");

	item=new LayerTreeItem(layer);
	placenames->appendRow(item);

	iconName = "/sidebar/layers-guidance.png";
	layer=new PlaceNameLayer(E_GDM_SUBDATA_TYPE_NAME_GUIDANCEITEM,tr("Guidance"),iconName);
	layer->SetProperty("tb_lm_guidance");

	item=new LayerTreeItem(layer);
	placenames->appendRow(item);
	
	m_layersView->setExpanded(placenames->index(),true);

	placenames->PropagateStateToAllChilds(Qt::Checked);
	return true;
}
bool LayersDlg::AddSeaPlaceNames()
{
	LayerTreeItem* root=RootItem();

	LayerTreeItem* placenames=new LayerTreeItem(tr("Sea PlaceNames"));
	root->appendRow(placenames);

	PlaceNameLayer* layer=new PlaceNameLayer(E_GDM_SUBDATA_TYPE_NAME_HARBOUR,tr("Harbour"));
	layer->SetProperty("obj_harbour");
	LayerTreeItem *item=new LayerTreeItem(layer);
	placenames->appendRow(item);

	layer=new PlaceNameLayer(E_GDM_SUBDATA_TYPE_NAME_DOCK,tr("Dock"));
	layer->SetProperty("obj_dock");
	item=new LayerTreeItem(layer);
	placenames->appendRow(item);

	layer=new PlaceNameLayer(E_GDM_SUBDATA_TYPE_NAME_FIREWAY,tr("Fireway"));
	layer->SetProperty("obj_fireway");
	item=new LayerTreeItem(layer);
	placenames->appendRow(item);

	layer=new PlaceNameLayer(E_GDM_SUBDATA_TYPE_NAME_TIDELAND,tr("TideLand"));
	layer->SetProperty("obj_tideland");
	item=new LayerTreeItem(layer);
	placenames->appendRow(item);

	layer=new PlaceNameLayer(E_GDM_SUBDATA_TYPE_NAME_FISHFARM,tr("FishFarm"));
	layer->SetProperty("obj_fishfarm");

	item=new LayerTreeItem(layer);
	placenames->appendRow(item);

	layer=new PlaceNameLayer(E_GDM_SUBDATA_TYPE_NAME_SEAFARM,tr("SeaFarm"));
	layer->SetProperty("obj_seafarm");
	item=new LayerTreeItem(layer);
	placenames->appendRow(item);

	layer=new PlaceNameLayer(E_GDM_SUBDATA_TYPE_NAME_FISHERY,tr("Fishery"));
	layer->SetProperty("obj_fishery");
	item=new LayerTreeItem(layer);
	placenames->appendRow(item);
	
	LayerTreeItem* factoryitem=new LayerTreeItem(tr("Factory"));
	placenames->appendRow(factoryitem);

	layer=new PlaceNameLayer(E_GDM_SUBDATA_TYPE_NAME_MARINEFACOTRY,tr("MarineFactory"));
	layer->SetProperty("obj_marinefactory");
	item=new LayerTreeItem(layer);
	factoryitem->appendRow(item);

	layer=new PlaceNameLayer(E_GDM_SUBDATA_TYPE_NAME_SEAWATERFACTORY,tr("SeaWaterFactory"));
	layer->SetProperty("obj_seawaterfactory");
	item=new LayerTreeItem(layer);
	factoryitem->appendRow(item);
	
	layer=new PlaceNameLayer(E_GDM_SUBDATA_TYPE_NAME_IRRIGATIONFACTORY,tr("IrrigationFactory"));
	layer->SetProperty("obj_irrigationfactroy");
	item=new LayerTreeItem(layer);
	factoryitem->appendRow(item);

	LayerTreeItem* protectionitem=new LayerTreeItem(tr("Protection Object"));
	placenames->appendRow(protectionitem);
	
	layer=new PlaceNameLayer(E_GDM_SUBDATA_TYPE_NAME_BEAUTY,tr("Beauty"));
	layer->SetProperty("obj_beauty");
	item=new LayerTreeItem(layer);
	protectionitem->appendRow(item);

	layer=new PlaceNameLayer(E_GDM_SUBDATA_TYPE_NAME_SEABATHY,tr("SeaBathy Place"));
	layer->SetProperty("obj_seabathy");
	item=new LayerTreeItem(layer);
	protectionitem->appendRow(item);

	layer=new PlaceNameLayer(E_GDM_SUBDATA_TYPE_NAME_SPA,tr("Spa"));
	layer->SetProperty("obj_spa");
	item=new LayerTreeItem(layer);
	protectionitem->appendRow(item);

	LayerTreeItem* miscprotectionitem=new LayerTreeItem(tr("Misc Protection Object"));
	protectionitem->appendRow(miscprotectionitem);

	layer=new PlaceNameLayer(E_GDM_SUBDATA_TYPE_NAME_MISC_PROTECTION_POINT,tr("Misc Point Protection Object"));
	layer->SetProperty("obj_miscprotectionpoint");
	item=new LayerTreeItem(layer);
	miscprotectionitem->appendRow(item);

	layer=new PlaceNameLayer(E_GDM_SUBDATA_TYPE_NAME_MISC_PROTECTION_POLYGON,tr("Misc Polygon Protection Object"));
	layer->SetProperty("obj_miscprotectionpolygon");
	item=new LayerTreeItem(layer);
	miscprotectionitem->appendRow(item);

	layer=new PlaceNameLayer(E_GDM_SUBDATA_TYPE_NAME_ACCIDENT,tr("Accident"));
	layer->SetProperty("obj_accident");
	item=new LayerTreeItem(layer);
	placenames->appendRow(item);

	layer=new PlaceNameLayer(E_GDM_SUBDATA_TYPE_NAME_POLUTANT,tr("Polutant"));
	layer->SetProperty("obj_poluant");
	item=new LayerTreeItem(layer);
	placenames->appendRow(item);

	m_layersView->setExpanded(placenames->index(),true);

	placenames->PropagateStateToAllChilds(Qt::Checked);

	return true;
}
bool LayersDlg::AddAdminBoundary()
{
	LayerTreeItem* root=RootItem();

	QString iconName = g_app->GetResource("/sidebar/layers-boundary.png");
	LayerTreeItem* adminboundarys=new LayerTreeItem(tr("Admin Boundary"),iconName);
	root->appendRow(adminboundarys);

	PolylineLayer* layer=new PolylineLayer(E_GDM_SUBDATA_TYPE_SHAPE_COUNTRY,tr("Country Border"));
	layer->SetDataPath("geoshape/country");
	
	layer->SetMaxLevel(SHAPE_COUNTRY_MAX_LEVEL);

	LayerTreeItem *item=new LayerTreeItem(layer);
	adminboundarys->appendRow(item);

	layer=new PolylineLayer(E_GDM_SUBDATA_TYPE_SHAPE_FIRSTADMIN,tr("1st Level Admin Borders"));
	layer->SetDataPath("geoshape/firstadmin");
	layer->SetLevel(SHAPE_FIRSTADMIN_MIN_LEVEL,SHAPE_FIRSTADMIN_MAX_LEVEL);

	item=new LayerTreeItem(layer);
	adminboundarys->appendRow(item);

	layer=new PolylineLayer(E_GDM_SUBDATA_TYPE_SHAPE_SECONDADMIN,tr("2nd Level Admin Borders"));
	layer->SetDataPath("geoshape/secondadmin");

	layer->SetLevel(SHAPE_SECONDADMIN_MIN_LEVEL,SHAPE_SECONDADMIN_MAX_LEVEL);

	item=new LayerTreeItem(layer);
	adminboundarys->appendRow(item);

	layer=new PolylineLayer(E_GDM_SUBDATA_TYPE_SHAPE_THIRDADMIN,tr("3nd Level Admin Borders"));
	layer->SetDataPath("geoshape/thirdadmin");

	layer->SetLevel(SHAPE_THIRDADMIN_MIN_LEVEL,SHAPE_THIRDADMIN_MAX_LEVEL);
	item=new LayerTreeItem(layer);
	adminboundarys->appendRow(item);

	m_layersView->setExpanded(adminboundarys->index(),true);

	adminboundarys->PropagateStateToAllChilds(Qt::Checked);
	
	return true;
}

bool LayersDlg::AddTransportNetwork()
{
	LayerTreeItem* root=RootItem();

	QString iconName = g_app->GetResource("/sidebar/layers-transport.png");
	LayerTreeItem* transportboudarys=new LayerTreeItem(tr("Transport Network"),iconName);
	root->appendRow(transportboudarys);

	LayerTreeItem* roads=new LayerTreeItem(tr("Road NetWork"));
	transportboudarys->appendRow(roads);

	PolylineLayer* layer=new PolylineLayer(E_GDM_SUBDATA_TYPE_SHAPE_HIGHWAY,tr("HighWay"));
	layer->SetLevel(SHAPE_HIGHWAY_MIN_LEVEL,SHAPE_HIGHWAY_MAX_LEVEL);
	layer->SetDataPath("geoshape/road/highway");

	LayerTreeItem* item=new LayerTreeItem(layer);

	roads->appendRow(item);

	layer=new PolylineLayer(E_GDM_SUBDATA_TYPE_SHAPE_1STLOAD,tr("1st Road"));

	layer->SetLevel(SHAPE_1STLOAD_MIN_LEVEL,SHAPE_1STLOAD_MAX_LEVEL);
	layer->SetDataPath("geoshape/road/1");
	item=new LayerTreeItem(layer);

	roads->appendRow(item);

	layer=new PolylineLayer(E_GDM_SUBDATA_TYPE_SHAPE_2STLOAD,tr("2st Road"));
	layer->SetLevel(SHAPE_2STLOAD_MIN_LEVEL,SHAPE_2STLOAD_MAX_LEVEL);
	layer->SetDataPath("geoshape/road/2");
	item=new LayerTreeItem(layer);

	roads->appendRow(item);

	layer=new PolylineLayer(E_GDM_SUBDATA_TYPE_SHAPE_3STLOAD,tr("3st Road"));
	layer->SetLevel(SHAPE_3STLOAD_MIN_LEVEL,SHAPE_3STLOAD_MAX_LEVEL);
	layer->SetDataPath("geoshape/road/3");
	item=new LayerTreeItem(layer);

	roads->appendRow(item);

	layer=new PolylineLayer(E_GDM_SUBDATA_TYPE_SHAPE_4STLOAD,tr("4st Road"));
	layer->SetLevel(SHAPE_4STLOAD_MIN_LEVEL,SHAPE_4STLOAD_MAX_LEVEL);
	layer->SetDataPath("geoshape/road/4");
	item=new LayerTreeItem(layer);

	roads->appendRow(item);

	layer=new PolylineLayer(E_GDM_SUBDATA_TYPE_SHAPE_5STLOAD,tr("5st Road"));
	layer->SetLevel(SHAPE_5STLOAD_MIN_LEVEL,SHAPE_5STLOAD_MAX_LEVEL);
	layer->SetDataPath("geoshape/road/5");

	item=new LayerTreeItem(layer);

	roads->appendRow(item);

	LayerTreeItem* railways=new LayerTreeItem(tr("Railway NetWork"));
	transportboudarys->appendRow(railways);

	layer=new PolylineLayer(E_GDM_SUBDATA_TYPE_SHAPE_WIDERAILWAY,tr("Wide Railway"));

	layer->SetLevel(SHAPE_WIDERAILWAY_MIN_LEVEL,SHAPE_WIDERAILWAY_MAX_LEVEL);
	layer->SetDataPath("geoshape/railway/wide");
	item=new LayerTreeItem(layer);

	railways->appendRow(item);

	layer=new PolylineLayer(E_GDM_SUBDATA_TYPE_SHAPE_NARROWRAILWAY,tr("Narrow Railway"));
	layer->SetLevel(SHAPE_NARROWRAILWAY_MIN_LEVEL,SHAPE_NARROWRAILWAY_MAX_LEVEL);
	layer->SetDataPath("geoshape/railway/narrow");
	item=new LayerTreeItem(layer);

	railways->appendRow(item);

	return true;
}

bool LayersDlg::AddWaterNetwork()
{
	LayerTreeItem* root=RootItem();

	QString iconName = g_app->GetResource("/sidebar/layers-water-boundary.png");
	LayerTreeItem* waternetworks=new LayerTreeItem(tr("Water Network"),iconName);
	root->appendRow(waternetworks);

	PolylineLayer* layer=new PolylineLayer(E_GDM_SUBDATA_TYPE_SHAPE_WIDERIVER,tr("Wide River"));
	layer->SetMaxLevel(SHAPE_WIDERIVER_MAX_LEVEL);
	layer->SetDataPath("geoshape/water/wideriver");

	LayerTreeItem* item=new LayerTreeItem(layer);

	waternetworks->appendRow(item);

	layer=new PolylineLayer(E_GDM_SUBDATA_TYPE_SHAPE_RIVER,tr("River"));
	layer->SetMaxLevel(SHAPE_RIVER_MAX_LEVEL);
	layer->SetDataPath("geoshape/water/river");

	item=new LayerTreeItem(layer);

	waternetworks->appendRow(item);

	layer=new PolylineLayer(E_GDM_SUBDATA_TYPE_SHAPE_LAKE,tr("Lake, Reservoir"));
	layer->SetMaxLevel(SHAPE_LAKE_MAX_LEVEL);
	layer->SetDataPath("geoshape/water/lake");

	item=new LayerTreeItem(layer);

	waternetworks->appendRow(item);

	return true;
}

bool LayersDlg::AddLanduseBoundary()
{
	LayerTreeItem* root=RootItem();

	LayerTreeItem* landboundarys=new LayerTreeItem(tr("Land"));
	root->appendRow(landboundarys);

	PolylineLayer* layer=new PolylineLayer(E_GDM_SUBDATA_TYPE_SHAPE_FARMLAND,tr("FarmLand"));
	layer->SetLevel(SHAPE_FARMLAND_MIN_LEVEL,SHAPE_FARMLAND_MAX_LEVEL);
	LayerTreeItem* item=new LayerTreeItem(layer);

	landboundarys->appendRow(item);

	layer=new PolylineLayer(E_GDM_SUBDATA_TYPE_SHAPE_NONFARMLAND,tr("NonFarmland"));
	layer->SetLevel(SHAPE_NONFARMLAND_MIN_LEVEL,SHAPE_NONFARMLAND_MAX_LEVEL);

	item=new LayerTreeItem(layer);

	landboundarys->appendRow(item);

	layer=new PolylineLayer(E_GDM_SUBDATA_TYPE_SHAPE_FORESTLAND,tr("Forest Land"));
	layer->SetLevel(SHAPE_FORESTLAND_MIN_LEVEL,SHAPE_FORESTLAND_MAX_LEVEL);

	item=new LayerTreeItem(layer);

	landboundarys->appendRow(item);
	return true;
}

bool LayersDlg::Add3dObject()
{
	LayerTreeItem* root=RootItem();

	QString iconName = g_app->GetResource("/sidebar/layers-3dobject.png");
	LayerTreeItem* objects=new LayerTreeItem(tr("3d Object"),iconName);
	root->appendRow(objects);

	ObjectLayer* layer=new ObjectLayer(E_GDM_SUBDATA_TYPE_3D_BUILDING,tr("3D Model"));
	layer->SetDataPath("geoobject/3dmodel/geomesh");
	LayerTreeItem* item=new LayerTreeItem(layer);

	objects->appendRow(item);

	layer=new ObjectLayer(E_GDM_SUBDATA_TYPE_BUILDING_SIMPLEBUILDING,tr("2D FootPrint"));

	item=new LayerTreeItem(layer);

	objects->appendRow(item);

	objects->SetCheckState(Qt::Checked);

	return true;
}

void LayersDlg::OnItemClicked(QModelIndex index)
{
	LayerTreeItem* item=(LayerTreeItem*)m_itemModel->itemFromIndex(index);

	item->OnClicked();

	if(m_TerrainItem->checkState()==Qt::Checked)
		g_app->GetConfig()->m_blTerrain=true;
	else 
		g_app->GetConfig()->m_blTerrain=false;

	if(g_app->GetConfig()->m_layerTopography!=LTM_TEXTURE && m_ImageItem->checkState()==Qt::Checked)
	{
		g_app->GetConfig()->m_layerTopography=LTM_TEXTURE;
		m_RasterItem->SetCheckState(Qt::Unchecked);
	}

	if(g_app->GetConfig()->m_layerTopography!=LTM_RASTER && m_RasterItem->checkState()==Qt::Checked)
	{
		g_app->GetConfig()->m_layerTopography=LTM_RASTER;
		m_ImageItem->SetCheckState(Qt::Unchecked);
	}
	UpdateDialog();
}

LayerTreeItem* LayersDlg::RootItem()
{
	LayerTreeItem *root = static_cast<LayerTreeItem*>(m_itemModel->invisibleRootItem());

	Q_ASSERT(root!=NULL);
	return root;
}

void LayersDlg::timerEvent(QTimerEvent *event)
{
	LayerTreeItem* root=RootItem();

	int level=gdm_GetDataMgr()->m_pPyramidMgr->GetMaxLevel();

	SetEnabledState(root,level);
}

void LayersDlg::SetEnabledState(LayerTreeItem* item,int level)
{
	if(item->hasChildren())
	{
		for (int i = 0; i < item->rowCount(); i++)
		{
			LayerTreeItem* child=(LayerTreeItem*)item->child(i);
			SetEnabledState(child,level);
		}
	}
	else
	{
		Layer* layer=item->GetLayer();
		
		if(!layer) return;

		if(layer->GetMainType()==E_GDM_MAINDATA_TYPE_IMG)
			return;

		if(layer->GetMainType()==E_GDM_MAINDATA_TYPE_DEM)
			return;

		if(level <=layer->GetMaxLevel() && level >=layer->GetMinLevel())
		{
			item->setEnabled(true);
		}
		else
		{
			item->setEnabled(false);
			//item->SetCheckState(Qt::Unchecked);
		}
	}
}

bool LayersDlg::AddRasterItems()
{
	LayerTreeItem* root=RootItem();

	m_RasterItem=new LayerTreeItem(tr("Raster"));

	root->appendRow(m_RasterItem);

	RasterLayer* layer=new RasterLayer(E_GDM_SUBDATA_TYPE_RASTER_SUNENERGY,("SunEnergy"));
	layer->SetDataPath("raster/sunenergy");

	LayerTreeItem *item=new LayerTreeItem(layer);
	m_RasterItem->appendRow(item);

	m_RasterItem->SetExclusiveChildItem(item);

	m_RasterItem->SetCheckedItemMustBeExist(true);
	m_RasterItem->PropagateStateToExclusiveChild(Qt::Unchecked);
	m_layersView->setExpanded(m_RasterItem->index(),true);

	return true;
}

#include "colorrampdlg.h"

void LayersDlg::OnItemDoubleClicked(QModelIndex index)
{
	LayerTreeItem* item=(LayerTreeItem*)m_itemModel->itemFromIndex(index);
	
	Layer* layer=item->GetLayer();

	if(!layer)
		return;

	if(layer->IsRasterLayer())
	{
		RasterLayer* rasterlayer=(RasterLayer*)layer;

		ColorRampDlg* dlg=new ColorRampDlg(m_layersView,rasterlayer->GetRenderer()->GetColorRamp());

		int x=m_layersView->GetDblClickedPoint().x()-dlg->width()/2;
		int y=m_layersView->GetDblClickedPoint().y()+dlg->height()/2;

		dlg->move(x,y);
		dlg->exec();

		int res=dlg->result();

		if(res==QDialog::Accepted)
		{
			IColorRamp* colorramp=dlg->GetColorRamp();

			if(colorramp!=rasterlayer->GetRenderer()->GetColorRamp())
			{
				rasterlayer->GetRenderer()->SetColorRamp(colorramp);
				g_app->GetRenderProxy()->SendMessage(PM_CLEAR_RASTER_CACHE);
			}
		}
	
		delete dlg;
	}
}

