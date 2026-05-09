
#include "QGlobe_ObjectAddCmd.h"
 
#include "config.h"
#include "qglobeclient.h"
#include "guiuty.h"
#include "mainwindow.h"
#include "renderfrm.h"

#include "QGlobe_3DObjectCache.h"
#include "QGlobe_CacheMgr.h"
#include "QGlobe_DataMgr.h"
 
#include "QGlobe_CommandMgr.h"
#include "GeoMeshUtil.h"
 
#include "QGlobe_SceneManager.h"
#include "Geo3dObjectSceneNode.h" //for using CaiculateScreenRect

#include "../qglobeds_common/QGlobeDSConfigMgr.h"
#include "QGlobe_RequestMgr.h"

#define INIT_HEIGHT                 1000

inline double GetDistance(double x1,double y1,double x2,double y2)
{
	return ((x1-x2)+(y1-y2));
}

CQGlobe_ObjectAddCmd::CQGlobe_ObjectAddCmd(CQGlobe_CommandDlg *pCmdDlg )
: CQGlobe_Command(pCmdDlg)
{	
	_GeoInfo.Longitude=0.0;
	_GeoInfo.Latitude=0.0;
	_GeoInfo.Azimuth=0.0;
	_GeoInfo.Height=0.0;
	_sampleMesh=NULL;
	_selectedGeoMesh=NULL;
	_toolState=SetPosition;
	_driver=g_app->GetRenderProxy()->GetRenderSrv()->m_pSceneMgr->VideoDriver();
	_pSampleMeshSceneNode=g_app->GetRenderProxy()->GetRenderSrv()->m_pSceneMgr->m_pSampleMeshNode;
	MakeDirectory(g_ServerConfig.GetFileDBPath()+GEOOBJECT_TEXTURE_PATH);

	_buildingProperty._ID=-1;
	_buildingProperty._Loc_ID=-1;
	_buildingProperty._Ban=1;
	_buildingProperty._Floor=1;
	_buildingProperty._MinLongitude=-180.0f;
	_buildingProperty._MaxLongitude=180.0f;
	_buildingProperty._MinLatitude=-90.0f;
	_buildingProperty._MaxLatitude=90.0f;

	_objectType=Building;  
	_SetPositionImmediatelySave=false;
	_selectedGeoMeshUpdated=false;
	_MultiSelect=false;
}

CQGlobe_ObjectAddCmd::~CQGlobe_ObjectAddCmd(void)
{
}

// get sample mesh from 3d file such as *.3ds, *.md2 etc

bool CQGlobe_ObjectAddCmd::GetSampleMesh(QString filename)
{
	char name[1024];
	ConvertToCharFromQString(filename, name);

	SAnimatedMesh *meshs=(SAnimatedMesh*)this->_driver->createMeshFromFile(name);
	
	if(meshs==NULL )              return false;

	if(meshs->Meshes.size()==0) 
	{
		meshs->drop();
		return false;
	}
	
	_sampleMesh=_driver->getMeshManipulator()->createMeshCopy(meshs->Meshes[0]);

	meshs->drop();

	_sampleMesh->recalculateBoundingBox();
	NormalizeMesh(_sampleMesh);

	return true;
}

bool CQGlobe_ObjectAddCmd::Open(QString studiofilename)
{
	_buildingProperty._ID=-1;
	_buildingProperty._Name="";
	_buildingProperty._PostName="";
	_buildingProperty._Ban=1;
	_buildingProperty._Floor=1;
	_buildingProperty._MinLongitude=-180.0f;
	_buildingProperty._MaxLongitude=180.0f;
	_buildingProperty._MinLatitude=-90.0f;
	_buildingProperty._MaxLatitude=90.0f;

	UpdateDailog();

	if(_sampleMesh)
		InvalidateSampleMesh();

	if(_selectedGeoMesh && !_selectedGeoMeshUpdated)
	{
		SetSelectState(false);
	    _SelectedGeoMeshs.clear();
		_selectedGeoMeshUpdated=false;
		_selectedGeoMesh=NULL;
		this->_sampleMesh=NULL;
		UpdateDailog();
	}

	if(!this->GetSampleMesh(studiofilename))	return false;

	RotateMinus90AroundXAxis();
	InvertXAxis();
	
	for(u32 i=0;i<_sampleMesh->getMeshBufferCount();i++)
		_sampleMesh->getMeshBuffer(i)->getMaterial().EmissiveColor.set(255,150,150,150);

	this->_pSampleMeshSceneNode->setLocalMesh(this->_sampleMesh);

	int windowx=g_app->GetMainWindow()->GetRenderFrame()->rect().width()/2;
	int windowy=g_app->GetMainWindow()->GetRenderFrame()->rect().height()/2;

	this->SetLocation(windowx,windowy);
	this->SetTopViewState();

	return true;
}


void CQGlobe_ObjectAddCmd::SetToolState(ToolState state)
{
	this->_toolState=state;

	if(_toolState==Select && _sampleMesh)
	{
		InvalidateSampleMesh();
	}
}

void CQGlobe_ObjectAddCmd::SetTopViewState()
{
   if(g_pQGlobeDataMgr->m_pCamera->m_location.m_dDist>INIT_HEIGHT)
   {
		QGlobe_CAMERA_INFO cameraInfo;	
		memset(&cameraInfo , 0 , sizeof(QGlobe_CAMERA_INFO));
		cameraInfo.m_loc.m_dDist=INIT_HEIGHT;
		cameraInfo.m_loc.m_dLongitude=this->_GeoInfo.Longitude;
		cameraInfo.m_loc.m_dLatitude=this->_GeoInfo.Latitude;

		g_app->GetRenderProxy()->GetRenderSrv()->m_pCameraController->AutoMoveTo(0, cameraInfo, 4000);
   }
   else
		m_pCommandMgr->GetCameraController()->SetTopViewState(this->_GeoInfo.Longitude,_GeoInfo.Latitude);	
}
  
void CQGlobe_ObjectAddCmd::SetSampleMeshLocation()
{
	QGlobe_LOCATION loc;

	loc.m_dLongitude=_GeoInfo.Longitude;
	loc.m_dLatitude=_GeoInfo.Latitude;
	loc.m_dDist =_GeoInfo.Height ;

	QGlobe_Coord3D standCoord;
	qglobe_GetStandardCoord(&loc, &standCoord);

	qglobe_RotateAroundZAxis(&standCoord,_GeoInfo.Azimuth*qglobe_DEGTORAD);

	CQGlobe_Matrix4 mat;
	qglobe_MatrixTCStoSCS(&standCoord, &mat );

	this->_pSampleMeshSceneNode->Transform=mat;
	this->_pSampleMeshSceneNode->Coord=standCoord;
}

bool CQGlobe_ObjectAddCmd::SetLocation(int cx,int cy)
{
	CQGlobe_Location3D tmp;
	if(qglobe_GetHitLocation(cx,cy,&tmp)==QGlobe_FAIL) return false;

	QGlobe_LOCATION loc;

	_GeoInfo.Longitude=loc.m_dLongitude =tmp.m_tX;
	_GeoInfo.Latitude=loc.m_dLatitude  =tmp.m_tY;
	_GeoInfo.Height=loc.m_dDist = tmp.m_tZ;

	if(this->_sampleMesh)
		this->SetSampleMeshLocation();
	else if(this->_selectedGeoMesh)
		this->SetSelectedGeoMeshLocation();
	
	this->UpdateDailog();

	return true;
}

void  CQGlobe_ObjectAddCmd::SelectGeoMesh(int x,int y)
{
	const QGlobe_LOGIC_TILE *ptile;
	QGlobe_TILE* meshtile;
	SAnimatedMesh* meshs;
	SGeoMesh* mesh;

	if(!_MultiSelect)
	{
		SetSelectState(false);

		if(_selectedGeoMesh && _selectedGeoMeshUpdated)
		{	
			int ret = gu_MessageBox((QWidget*)g_app->GetMainWindow(), tr("QGLOBE Client"), tr("Exist modified data! Are you sure to save?"), MSGBOX_IDYESNO|MSGBOX_ICONQUESTION);
			if(ret == MSGBOX_IDYES) 
			{
				QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));

				foreach(SGeoMesh* mesh,_SelectedGeoMeshs)
					SaveRevisedGeoMesh(mesh);
				QApplication::restoreOverrideCursor();
			}
			g_pQGlobeDataMgr->Clear3DCache();
			g_pQGlobeDataMgr->m_pRequestMgr->ClearRequest(E_QGlobe_MAINDATA_TYPE_3D);
			g_pQGlobeDataMgr->m_pRequestMgr->UpdateRequest(E_QGlobe_MAINDATA_TYPE_3D);

			_SelectedGeoMeshs.clear();
			_selectedGeoMeshUpdated=false;
			_selectedGeoMesh=NULL;
			return;
		}

		_SelectedGeoMeshs.clear();
		_selectedGeoMeshUpdated=false;
		_selectedGeoMesh=NULL;
	}
	
	const QGlobe_LOGIC_TILE_ARR *render_tile_list= qglobe_GetRenderTiles();

	for(int i=0;i<render_tile_list->nCount;i++)
	{
		ptile=render_tile_list->pData+i;

		for(int j=0;j<ptile->n3dMeshTileCnt;j++)
		{
			meshtile=ptile->sArr3DMeshTile[j];
			meshs=(SAnimatedMesh*)g_pQGlobeDataMgr->m_p3DObjectCache->GetMeshs(meshtile->m_sInfo.nX,
				meshtile->m_sInfo.nY,
				meshtile->m_sInfo.nLevel,
				meshtile->m_pBuffer);

			for (unsigned int k=0;k<meshs->Meshes.size();k++)
			{
				mesh=(SGeoMesh*)meshs->Meshes[k];

				if(mesh->GeoInfo.Longitude < ptile->minLongitude || mesh->GeoInfo.Longitude > ptile->maxLongitude
					|| mesh->GeoInfo.Latitude < ptile->minLatitude || mesh->GeoInfo.Latitude > ptile->maxLatitude)
					continue;

				core::rect<s32> screenrect;

				GeoMeshUtil::CalculateScreenRect(mesh,screenrect);

				if(screenrect.isPointInside(core::position2di(x,y)))	
				{
					if(_MultiSelect)
						mesh->SetSelected(!mesh->GetSelected());
					else
					{
						mesh->SetSelected(true);
					}

					if(mesh->GetSelected())
					{
						_GeoInfo=mesh->GeoInfo;
						_selectedGeoMesh=mesh;
						
						bool ret;
						ret=GeoMeshUtil::GetGeoBuildingData(_selectedGeoMesh->GeoInfo.ID,&_buildingProperty);

						if(ret==true)
							UpdateDailog();
						else
						{	
							_buildingProperty._Address="";
							_buildingProperty._Ban="";
							_buildingProperty._Floor=0;
							_buildingProperty._ID=_GeoInfo.ID;
							_buildingProperty._Loc_ID=0;
							_buildingProperty._Name="";
							_buildingProperty._PostName="";
							_buildingProperty._Misc="";
							//this mesh is not building or 
							UpdateDailog();
							//gu_MessageBox(NULL,tr("Information"),tr("Exist not property of selected object in database"),MSGBOX_ICONINFO);
				
						}

						if(_SelectedGeoMeshs.count(_selectedGeoMesh->GeoInfo.ID)==0)
							_SelectedGeoMeshs.insert(_selectedGeoMesh->GeoInfo.ID,_selectedGeoMesh);
					}
					else
					{
						if(_SelectedGeoMeshs.count(mesh->GeoInfo.ID)!=0)
							_SelectedGeoMeshs.remove(mesh->GeoInfo.ID);
					}  //mesh->GetSelected()
				}
				else
				{
					if(!_MultiSelect)
						mesh->SetSelected(false);
				}
			}
		}
	}

	if(_selectedGeoMesh)
		UpdateDailog();
}

void  CQGlobe_ObjectAddCmd::SetSelectState(bool val)
{
	const QGlobe_LOGIC_TILE *ptile;
	QGlobe_TILE* meshtile;
	SAnimatedMesh* meshs;
	SGeoMesh* mesh;

	const QGlobe_LOGIC_TILE_ARR *render_tile_list= qglobe_GetRenderTiles();

	for(int i=0;i<render_tile_list->nCount;i++)
	{
		ptile=render_tile_list->pData+i;

		for(int j=0;j<ptile->n3dMeshTileCnt;j++)
		{
			meshtile=ptile->sArr3DMeshTile[j];
			meshs=(SAnimatedMesh*)g_pQGlobeDataMgr->m_p3DObjectCache->GetMeshs(meshtile->m_sInfo.nX,
				meshtile->m_sInfo.nY,
				meshtile->m_sInfo.nLevel,
				meshtile->m_pBuffer);

			for (unsigned int k=0;k<meshs->Meshes.size();k++)
			{
				mesh=(SGeoMesh*)meshs->Meshes[k];

				if(mesh->GeoInfo.Longitude < ptile->minLongitude || mesh->GeoInfo.Longitude > ptile->maxLongitude
					|| mesh->GeoInfo.Latitude < ptile->minLatitude || mesh->GeoInfo.Latitude > ptile->maxLatitude)
					continue;
				mesh->SetSelected(val);
			}
		}
	}
}


bool CQGlobe_ObjectAddCmd::OnMouseRelease(CursorState* mouseEvent)
{
	
	return false;
}

bool CQGlobe_ObjectAddCmd::OnMousePress(CursorState* mouseEvent)
{
	if(mouseEvent->buttons==1)/*left button*/
	{
		this->_prevScreenX=mouseEvent->x;
		this->_prevScreenY=mouseEvent->y;

		switch(this->_toolState)
		{
			case SetPosition:
			{
				 this->SetLocation(mouseEvent->x,mouseEvent->y); 
				
				 if(_sampleMesh && _SetPositionImmediatelySave)
				 {
					 core::rect<f32> boundingrect; // in radian

					 GeoMeshUtil::CalculateBoundingRect(_sampleMesh,_GeoInfo,boundingrect);

					 _buildingProperty._MinLongitude=boundingrect.UpperLeftCorner.X;
					 _buildingProperty._MinLatitude=boundingrect.UpperLeftCorner.Y;
					 _buildingProperty._MaxLongitude=boundingrect.LowerRightCorner.X;
					 _buildingProperty._MaxLatitude=boundingrect.LowerRightCorner.Y;

					 double centerLongi = (boundingrect.UpperLeftCorner.X + boundingrect.LowerRightCorner.X)/2.0;
					 double centerLati  = (boundingrect.UpperLeftCorner.Y + boundingrect.LowerRightCorner.Y)/2.0;

					 _GeoInfo.ID=GeoMeshUtil::GenerateGeoMeshID(centerLongi, centerLati);
					 _buildingProperty._ID=_GeoInfo.ID;

					 if(_objectType==Building && !GeoMeshUtil::AddGeoBuildingData(&_buildingProperty))
					 {
						 gu_MessageBox(NULL,tr("Error"),tr("Failed to add property of building to database"),MSGBOX_ICONCRITICAL);
						 return false;
					 }

					 for(int i=THREED_MIN_LEVEL;i<=THREED_MAX_LEVEL;i++)
						 Save(i);
					
					g_pQGlobeDataMgr->m_pRequestMgr->ClearRequest(E_QGlobe_MAINDATA_TYPE_3D);
					g_pQGlobeDataMgr->m_pRequestMgr->UpdateRequest(E_QGlobe_MAINDATA_TYPE_3D);
				 }				 
				 break;
			}	
			case Select:
			{
				SelectGeoMesh(mouseEvent->x,mouseEvent->y);
				break;
			}
			
		}
	}

	this->UpdateDailog();
	return false;
}

void CQGlobe_ObjectAddCmd::OnCancel()
{
	if(this->_sampleMesh)
	{
		InvalidateSampleMesh();
		this->_sampleMesh=NULL;
	}

	SetSelectState(false);

	if(_selectedGeoMesh && _selectedGeoMeshUpdated)
	{	
		int ret = gu_MessageBox((QWidget*)g_app->GetMainWindow(), tr("QGLOBE Client"), tr("Exist modified data! Are you sure to save?"), MSGBOX_IDYESNO|MSGBOX_ICONQUESTION);
		if(ret == MSGBOX_IDYES) 
		{
			QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
		
			foreach(SGeoMesh* mesh,_SelectedGeoMeshs)
				SaveRevisedGeoMesh(mesh);
			QApplication::restoreOverrideCursor();
		}
		g_pQGlobeDataMgr->Clear3DCache();
		g_pQGlobeDataMgr->m_pRequestMgr->ClearRequest(E_QGlobe_MAINDATA_TYPE_3D);
		g_pQGlobeDataMgr->m_pRequestMgr->UpdateRequest(E_QGlobe_MAINDATA_TYPE_3D);
	}

	_SelectedGeoMeshs.clear();
	_selectedGeoMeshUpdated=false;
	_selectedGeoMesh=NULL;

	CQGlobe_Command::OnCancel();
}

void CQGlobe_ObjectAddCmd::OnEnd()
{
	if(this->_sampleMesh)
	{
		InvalidateSampleMesh();
		this->_sampleMesh=NULL;
	}

	SetSelectState(false);

	if(_selectedGeoMesh && _selectedGeoMeshUpdated)
	{	
		int ret = gu_MessageBox((QWidget*)g_app->GetMainWindow(), tr("QGLOBE Client"), tr("Exist modified data! Are you sure to save?"), MSGBOX_IDYESNO|MSGBOX_ICONQUESTION);
		if(ret == MSGBOX_IDYES) 
		{
			QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));

			foreach(SGeoMesh* mesh,_SelectedGeoMeshs)
				SaveRevisedGeoMesh(mesh);
			QApplication::restoreOverrideCursor();
		}
		g_pQGlobeDataMgr->Clear3DCache();
		g_pQGlobeDataMgr->m_pRequestMgr->ClearRequest(E_QGlobe_MAINDATA_TYPE_3D);
		g_pQGlobeDataMgr->m_pRequestMgr->UpdateRequest(E_QGlobe_MAINDATA_TYPE_3D);
	}

	_SelectedGeoMeshs.clear();
	_selectedGeoMeshUpdated=false;
	_selectedGeoMesh=NULL;
	CQGlobe_Command::OnEnd();
}

//precondition
//opend must be setted true

void CQGlobe_ObjectAddCmd::InvalidateSampleMesh()
{
	this->_sampleMesh->drop();
	this->_sampleMesh=NULL;

	this->_pSampleMeshSceneNode->clear();
}

//precondition
// _sampleMesh or _selectedGeoMesh is not NULL

void CQGlobe_ObjectAddCmd::SetLength(double length)
{
	SMesh* mesh;
	if(_sampleMesh) 
		mesh=_sampleMesh;
	else if(_selectedGeoMesh)
		mesh=_selectedGeoMesh;

	core::aabbox3df box=mesh->getBoundingBox();
	double oldlength=box.MaxEdge.X-box.MinEdge.X;

	double scale=length/oldlength;

	core::matrix4 trans;
	trans.setScale(core::vector3df(scale,1.0,1.0));
	_driver->getMeshManipulator()->transform(mesh,trans);
	mesh->recalculateBoundingBox();
	UpdateDailog();
}

//precondition
// _sampleMesh or _selectedGeoMesh is not NULL

void CQGlobe_ObjectAddCmd::SetWidth(double width)
{
	SMesh* mesh;
	if(_sampleMesh) 
		mesh=_sampleMesh;
	else if(_selectedGeoMesh)
	{
		mesh=_selectedGeoMesh;
		_selectedGeoMeshUpdated=true;
	}

	core::aabbox3df box=mesh->getBoundingBox();
	double oldwidth=box.MaxEdge.Y-box.MinEdge.Y;

	double scale=width/oldwidth;

	core::matrix4 trans;
	trans.setScale(core::vector3df(1.0,scale,1.0));
	_driver->getMeshManipulator()->transform(mesh,trans);
	mesh->recalculateBoundingBox();
	UpdateDailog();
}

//precondition
// _sampleMesh or _selectedGeoMesh is not NULL
void CQGlobe_ObjectAddCmd::SetHeight(double height)
{
	SMesh* mesh;
	if(_sampleMesh) 
		mesh=_sampleMesh;
	else if(_selectedGeoMesh)
	{
		mesh=_selectedGeoMesh;
		_selectedGeoMeshUpdated=true;
	}

	core::aabbox3df box=mesh->getBoundingBox();
	double oldheight=box.MaxEdge.Z-box.MinEdge.Z;

	double scale=height/oldheight;

	core::matrix4 trans;
	trans.setScale(core::vector3df(1.0,1.0,scale));
	_driver->getMeshManipulator()->transform(mesh,trans);
	mesh->recalculateBoundingBox();
	UpdateDailog();
}

bool CQGlobe_ObjectAddCmd::OnMouseMove(CursorState* mouseEvent)
{
	SMesh* mesh=NULL;

	if(_sampleMesh) 
		mesh=_sampleMesh;
	else if(_selectedGeoMesh)
	{
		mesh=_selectedGeoMesh;
	}
	
	if(!mesh) return false;

	bool ret=false;

	if(mouseEvent->buttons!=1)
		return false;

	core::matrix4 trans;

	switch(this->_toolState)
	{
		case SetPosition:
		{
			this->SetLocation(mouseEvent->x,mouseEvent->y); 
			break;
		}
		case SetAltitude:
		{
			_GeoInfo.Height=_GeoInfo.Height+GetDistance(mouseEvent->x,mouseEvent->y,this->_prevScreenX,this->_prevScreenY);
			if(this->_sampleMesh)
				this->SetSampleMeshLocation();
			else if(this->_selectedGeoMesh)
				this->SetSelectedGeoMeshLocation();
			ret=true;
			break;
		}
		case SetDirection:
		{
			_GeoInfo.Azimuth=_GeoInfo.Azimuth+GetDistance(mouseEvent->x,mouseEvent->y,this->_prevScreenX,this->_prevScreenY);
			if(_GeoInfo.Azimuth>360)
				_GeoInfo.Azimuth=0;
			if(_GeoInfo.Azimuth<0)
				_GeoInfo.Azimuth=360;
				
			if(this->_sampleMesh)
				this->SetSampleMeshLocation();
			else if(this->_selectedGeoMesh)
				this->SetSelectedGeoMeshLocation();
			ret=true;
			break;
		}
		case RoateXAxis:
		{
			float delta= GetDistance(mouseEvent->x,mouseEvent->y,this->_prevScreenX,this->_prevScreenY)*0.1f;
			
			trans.setRotationDegrees(core::vector3df(delta,0.0f,0.0f));
			_driver->getMeshManipulator()->transform(mesh,trans);

			mesh->recalculateBoundingBox();
			ret=true;
			break;
		}
		case RoateYAxis:
		{
			float delta= GetDistance(mouseEvent->x,mouseEvent->y,this->_prevScreenX,this->_prevScreenY)*0.1f;

			trans.setRotationDegrees(core::vector3df(0.0f,delta,0.0f));
			_driver->getMeshManipulator()->transform(mesh,trans);

			mesh->recalculateBoundingBox();
			ret=true;
			break;
		}

		case ScaleAllAxis:
		{
			float deltascale= GetDistance(mouseEvent->x,mouseEvent->y,this->_prevScreenX,this->_prevScreenY)*0.01;

			if(1.0+deltascale>0.1)
			{
				trans.setScale(core::vector3df(1.0+deltascale,1+deltascale,1+deltascale));
				_driver->getMeshManipulator()->transform(mesh,trans);
			}
			mesh->recalculateBoundingBox();
			ret=true;
			break;
		}
		case ScaleXAxis:
		{
			float deltascale= GetDistance(mouseEvent->x,mouseEvent->y,this->_prevScreenX,this->_prevScreenY)*0.01;

			if(1.0+deltascale>0.1)
			{
				trans.setScale(core::vector3df(1.0+deltascale,1.0f,1.0f));
				_driver->getMeshManipulator()->transform(mesh,trans);
			}
			ret=true;
			mesh->recalculateBoundingBox();
			break;
		}

		case ScaleXYAxis:
		{
			float deltascale= GetDistance(mouseEvent->x,mouseEvent->y,this->_prevScreenX,this->_prevScreenY)*0.01;

			if(1.0+deltascale>0.1)
			{
				trans.setScale(core::vector3df(1.0+deltascale,1.0+deltascale,1));
				_driver->getMeshManipulator()->transform(mesh,trans);
			}
			ret=true;
			mesh->recalculateBoundingBox();
			break;
		}

		case ScaleYAxis:
		{
			float deltascale= GetDistance(mouseEvent->x,mouseEvent->y,this->_prevScreenX,this->_prevScreenY)*0.01;

			if(1.0+deltascale>0.1)
			{
				trans.setScale(core::vector3df(1.0,1+deltascale,1));
				_driver->getMeshManipulator()->transform(mesh,trans);
			}
			ret=true;
			mesh->recalculateBoundingBox();
			break;
		}

		case ScaleZAxis:
		{
			core::matrix4 trans;
			float deltascale= GetDistance(mouseEvent->x,mouseEvent->y,this->_prevScreenX,this->_prevScreenY)*0.01;

			if(1.0+deltascale>0.1)
			{
				trans.setScale(core::vector3df(1.0,1,1+deltascale));
				_driver->getMeshManipulator()->transform(mesh,trans);
			}
			mesh->recalculateBoundingBox();
			ret=true;
			break;
		}
		case BackgroundMove:
		{
			ret=false;
			break;
		}
	}

	if(_selectedGeoMesh )
	{
		if(_toolState!=Select && _toolState!=BackgroundMove)
			_selectedGeoMeshUpdated=true;
	}
	
	this->_prevScreenX=mouseEvent->x;
	this->_prevScreenY=mouseEvent->y;
	this->UpdateDailog();
	return ret;
}

bool CQGlobe_ObjectAddCmd::OnMouseWheelEvent(CursorState* mouseEvent)
{
	SetSelectState(false);

	if(_selectedGeoMesh && _selectedGeoMeshUpdated)
	{	
		int ret = gu_MessageBox((QWidget*)g_app->GetMainWindow(), tr("QGLOBE Client"), tr("Exist modified data! Are you sure to save?"), MSGBOX_IDYESNO|MSGBOX_ICONQUESTION);
		if(ret == MSGBOX_IDYES) 
		{
			QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));

			foreach(SGeoMesh* mesh,_SelectedGeoMeshs)
				SaveRevisedGeoMesh(mesh);
			QApplication::restoreOverrideCursor();
		}
		g_pQGlobeDataMgr->Clear3DCache();
		g_pQGlobeDataMgr->m_pRequestMgr->ClearRequest(E_QGlobe_MAINDATA_TYPE_3D);
		g_pQGlobeDataMgr->m_pRequestMgr->UpdateRequest(E_QGlobe_MAINDATA_TYPE_3D);
	}

	_SelectedGeoMeshs.clear();
	_selectedGeoMeshUpdated=false;
	_selectedGeoMesh=NULL;
	return  CQGlobe_Command::OnMouseWheelEvent(mouseEvent);
}

//수정된 결과를 보존한다.
void CQGlobe_ObjectAddCmd::SaveRevisedGeoMesh(SGeoMesh* givenMesh)
{
	int x,y;
	QString oldfilename,newfilename;

	qint64 oldid=givenMesh->GeoInfo.ID;

	core::rect<f32> boundingrect; // in radian

	GeoMeshUtil::CalculateBoundingRect(givenMesh,givenMesh->GeoInfo,boundingrect);

	_buildingProperty._MinLongitude=boundingrect.UpperLeftCorner.X;
	_buildingProperty._MinLatitude=boundingrect.UpperLeftCorner.Y;
	_buildingProperty._MaxLongitude=boundingrect.LowerRightCorner.X;
	_buildingProperty._MaxLatitude=boundingrect.LowerRightCorner.Y;

	double centerLongi = (boundingrect.UpperLeftCorner.X + boundingrect.LowerRightCorner.X)/2.0;
	double centerLati  = (boundingrect.UpperLeftCorner.Y + boundingrect.LowerRightCorner.Y)/2.0;

	givenMesh->GeoInfo.ID=GeoMeshUtil::GenerateGeoMeshID(centerLongi, centerLati);

	if(oldid==givenMesh->GeoInfo.ID)
	{
		_buildingProperty._ID=oldid;
		
		if(GeoMeshUtil::ExistGeoBuildingData(oldid))
			GeoMeshUtil::UpdateGeoBuildingData(&_buildingProperty);
		else
		{
			if(!GeoMeshUtil::AddGeoBuildingData(&_buildingProperty))
			{
				gu_MessageBox(NULL,tr("Error"),tr("Failed to add property of building to database"),MSGBOX_ICONCRITICAL);
				return;
			}
		}
	}
	else
	{
		GeoMeshUtil::DeleteGeoBuildingData(oldid);
		_buildingProperty._ID=givenMesh->GeoInfo.ID;
		GeoMeshUtil::AddGeoBuildingData(&_buildingProperty);
	}
	
	double oldlongitude=givenMesh->OldLongitude;
	double oldlatitude=givenMesh->OldLatitude;

	for(int level=THREED_MIN_LEVEL;level<=THREED_MAX_LEVEL;level++)
	{
		GetXY(oldlongitude,oldlatitude,level,x,y);
		oldfilename=GeoMeshUtil::GetFileNameOfGeoMeshTile(x,y,level);
		
		Q_ASSERT(QFile::exists(oldfilename));

		if(oldid==givenMesh->GeoInfo.ID)
		{
			GeoMeshUtil::UpdateGeoMeshFile(oldfilename,oldid,givenMesh);
		}
		else
		{
			GetXY(givenMesh->GeoInfo.Longitude,givenMesh->GeoInfo.Latitude,level,x,y);
			newfilename=GeoMeshUtil::GetFileNameOfGeoMeshTile(x,y,level);

			if(newfilename==oldfilename)
			{
				GeoMeshUtil::UpdateGeoMeshFile(oldfilename,oldid,givenMesh);
			}
			else
			{
				GeoMeshUtil::DeleteGeoMeshFromFile(oldfilename,oldid);
				GeoMeshUtil::AppendGeoMeshToGeoMeshFile(newfilename,givenMesh,givenMesh->GeoInfo,level);
			}
		}
	}

	givenMesh->OldLongitude=givenMesh->GeoInfo.Longitude;
	givenMesh->OldLatitude=givenMesh->GeoInfo.Latitude;
}

void CQGlobe_ObjectAddCmd::Save()
{
	//check precondition
	Q_ASSERT(_sampleMesh);

	core::rect<f32> boundingrect; // in radian

	GeoMeshUtil::CalculateBoundingRect(_sampleMesh,_GeoInfo,boundingrect);

	_buildingProperty._MinLongitude=boundingrect.UpperLeftCorner.X;
	_buildingProperty._MinLatitude=boundingrect.UpperLeftCorner.Y;
	_buildingProperty._MaxLongitude=boundingrect.LowerRightCorner.X;
	_buildingProperty._MaxLatitude=boundingrect.LowerRightCorner.Y;

	double centerLongi = (boundingrect.UpperLeftCorner.X + boundingrect.LowerRightCorner.X)/2.0;
	double centerLati  = (boundingrect.UpperLeftCorner.Y + boundingrect.LowerRightCorner.Y)/2.0;

	_GeoInfo.ID=GeoMeshUtil::GenerateGeoMeshID(centerLongi, centerLati);
	_buildingProperty._ID=_GeoInfo.ID;
	
	if(_objectType==Building && !GeoMeshUtil::AddGeoBuildingData(&_buildingProperty))
	{
		gu_MessageBox(NULL,tr("Error"),tr("Failed to add property of building to database"),MSGBOX_ICONCRITICAL);
		return;
	}
				
	for(int i=THREED_MIN_LEVEL;i<=THREED_MAX_LEVEL;i++)
		Save(i);
}

void CQGlobe_ObjectAddCmd::Save(int level)
{
	int x,y;

	GetXY(_GeoInfo.Longitude,_GeoInfo.Latitude,level,x,y);
	QString file_name=GeoMeshUtil::GetFileNameOfGeoMeshTile(x,y,level);

	if(!QFile::exists(file_name))      //this Scene is added firstly
	{
		GeoMeshUtil::SaveToGeoMeshFile(this->_sampleMesh,_GeoInfo,level);
	}
	else
	{
		QString backupfilename=file_name+".backup";

		QFile file(file_name);

		file.copy(backupfilename);

		GeoMeshUtil::AppendGeoMeshToGeoMeshFile(file_name,this->_sampleMesh,this->_GeoInfo,level);

		if(GeoMeshUtil::IsTooBigFileSize(file_name))
		{
			gu_MessageBox(NULL,("Error"),"The size of tile is greater than default size! Tile is can not be saved!",MSGBOX_ICONCRITICAL);

			if(!QFile::remove(file_name))
			{
				gu_MessageBox(NULL,("Error"),"Original tile can not be restored, because existed file can not be deleted.The file is being used by another person or program!",MSGBOX_ICONCRITICAL);
				return;
			}

			QFile::rename(backupfilename,file_name);
		}
		else
			QFile::remove(backupfilename);

		g_pQGlobeDataMgr->Delete3DData(x,y,level);
	}
}

void CQGlobe_ObjectAddCmd::OnKeyPressEvent(KeyState keyState)
{
	switch(keyState.key1)
	{
#ifdef DEBUG
	   case Qt::Key_F1	:
		   g_app->GetConfig()->m_isShowTerrainWireframe=!(g_app->GetConfig()->m_isShowTerrainWireframe);
		   break;
	   case Qt::Key_F2	:
		   g_app->GetConfig()->m_isShowRenderTileName=!(g_app->GetConfig()->m_isShowRenderTileName);
		   break;
	   case Qt::Key_F3	:
		   g_app->GetConfig()->m_isShowRenderTileBoundry=!(g_app->GetConfig()->m_isShowRenderTileBoundry);
		   break;
#endif
	   case Qt::Key_F4:
		   SetTopViewState();
		   break;
	   case Qt::Key_F5:
		   this->Save();
	   case Qt::Key_Delete:
		   DeleteSelectedGeoMeshs();
		   break;
	}
}

//지정된 매쉬를 모든 준위에서 지운다.
//this function is not delete texture of selectedmesh

void CQGlobe_ObjectAddCmd::DeleteGeoMesh(SGeoMesh* selectedmesh)
{
	//first we delete building property record from sql database

	if(!GeoMeshUtil::DeleteGeoBuildingData(selectedmesh->GeoInfo.ID))
	{
		gu_MessageBox(NULL,tr("Error"),tr("Failed to delete building property from database! This is Error or selected mesh may not building or "),MSGBOX_ICONINFO);
		
		//return;
	}

	int x,y;
	QString filename;

	for(int level=THREED_MIN_LEVEL;level<=THREED_MAX_LEVEL;level++)
	{
		GetXY(selectedmesh->OldLongitude,selectedmesh->OldLatitude,level,x,y);
		filename=GeoMeshUtil::GetFileNameOfGeoMeshTile(x,y,level);
		
		Q_ASSERT(QFile::exists(filename));
	
		GeoMeshUtil::DeleteGeoMeshFromFile(filename,selectedmesh->GeoInfo.ID);
	}
}

void CQGlobe_ObjectAddCmd::DeleteSelectedGeoMeshs()
{
	const QGlobe_LOGIC_TILE *ptile;
	QGlobe_TILE* meshtile;
	SAnimatedMesh* meshs;
	SGeoMesh* mesh;

	const QGlobe_LOGIC_TILE_ARR *render_tile_list= qglobe_GetRenderTiles();

	for(int i=0;i<render_tile_list->nCount;i++)
	{
		ptile=render_tile_list->pData+i;

		for(int j=0;j<ptile->n3dMeshTileCnt;j++)
		{
			meshtile=ptile->sArr3DMeshTile[j];
			meshs=(SAnimatedMesh*)g_pQGlobeDataMgr->m_p3DObjectCache->GetMeshs(meshtile->m_sInfo.nX,
				meshtile->m_sInfo.nY,
				meshtile->m_sInfo.nLevel,
				meshtile->m_pBuffer);

			for (unsigned int k=0;k<meshs->Meshes.size();k++)
			{
				mesh=(SGeoMesh*)meshs->Meshes[k];

				if(mesh->GeoInfo.Longitude < ptile->minLongitude || mesh->GeoInfo.Longitude > ptile->maxLongitude
					|| mesh->GeoInfo.Latitude < ptile->minLatitude || mesh->GeoInfo.Latitude > ptile->maxLatitude)
					continue;

				 if(mesh->GetSelected())
				 	DeleteGeoMesh(mesh);
			}
		}
	}
	_SelectedGeoMeshs.clear();
}

void CQGlobe_ObjectAddCmd::SetMaterialType(int val)
{
	SMesh* mesh;
	if(_sampleMesh) 
		mesh=_sampleMesh;
	else if(_selectedGeoMesh)
	{
		mesh=_selectedGeoMesh;
		_selectedGeoMeshUpdated=true;
	}
	else
		return ;

	for(int i=0;i<mesh->getMeshBufferCount();i++)
	{
		IMeshBuffer* buffer=mesh->getMeshBuffer(i);

		video::SMaterial& material=buffer->getMaterial();
		material.MaterialType=(video::E_MATERIAL_TYPE)val;
	}

	if(_sampleMesh)
	{
		IMesh* mesh= this->_pSampleMeshSceneNode->getWorldMesh();

		for(int i=0;i<mesh->getMeshBufferCount();i++)
		{
			IMeshBuffer* buffer=mesh->getMeshBuffer(i);

			video::SMaterial& material=buffer->getMaterial();
			material.MaterialType=(video::E_MATERIAL_TYPE)val;
		}
	}
}

void CQGlobe_ObjectAddCmd::SetDiffuseColor(QColor color)
{
	SMesh* mesh;
	if(_sampleMesh) 
		mesh=_sampleMesh;
	else if(_selectedGeoMesh)
	{
		mesh=_selectedGeoMesh;
		_selectedGeoMeshUpdated=true;
	}
	else
		return;

	for(int i=0;i<mesh->getMeshBufferCount();i++)
	{
		IMeshBuffer* buffer=mesh->getMeshBuffer(i);

		video::SMaterial& material=buffer->getMaterial();
		material.DiffuseColor.set(color.alpha(),color.red(),color.green(),color.blue());
	}

	if(_sampleMesh)
	{
		IMesh* mesh= this->_pSampleMeshSceneNode->getWorldMesh();

		for(int i=0;i<mesh->getMeshBufferCount();i++)
		{
			IMeshBuffer* buffer=mesh->getMeshBuffer(i);

			video::SMaterial& material=buffer->getMaterial();
			material.DiffuseColor.set(color.alpha(),color.red(),color.green(),color.blue());
		}
	}
}

void CQGlobe_ObjectAddCmd::SetAmbientColor(QColor color)
{
	SMesh* mesh;
	if(_sampleMesh) 
		mesh=_sampleMesh;
	else if(_selectedGeoMesh)
	{
		mesh=_selectedGeoMesh;
		_selectedGeoMeshUpdated=true;
	}
	else
		return;

	for(int i=0;i<mesh->getMeshBufferCount();i++)
	{
		IMeshBuffer* buffer=mesh->getMeshBuffer(i);

		video::SMaterial& material=buffer->getMaterial();
		material.AmbientColor.set(color.alpha(),color.red(),color.green(),color.blue());
	}

	if(_sampleMesh)
	{
		IMesh* mesh= this->_pSampleMeshSceneNode->getWorldMesh();

		for(int i=0;i<mesh->getMeshBufferCount();i++)
		{
			IMeshBuffer* buffer=mesh->getMeshBuffer(i);

			video::SMaterial& material=buffer->getMaterial();
			material.AmbientColor.set(color.alpha(),color.red(),color.green(),color.blue());
		}
	}
}

void CQGlobe_ObjectAddCmd::SetSpecularColor(QColor color)
{
	SMesh* mesh;
	if(_sampleMesh) 
		mesh=_sampleMesh;
	else if(_selectedGeoMesh)
	{
		mesh=_selectedGeoMesh;
		_selectedGeoMeshUpdated=true;
	}
	else
		return;

	for(int i=0;i<mesh->getMeshBufferCount();i++)
	{
		IMeshBuffer* buffer=mesh->getMeshBuffer(i);

		video::SMaterial& material=buffer->getMaterial();
		material.SpecularColor.set(color.alpha(),color.red(),color.green(),color.blue());
	}

	if(_sampleMesh)
	{
		IMesh* mesh= this->_pSampleMeshSceneNode->getWorldMesh();

		for(int i=0;i<mesh->getMeshBufferCount();i++)
		{
			IMeshBuffer* buffer=mesh->getMeshBuffer(i);

			video::SMaterial& material=buffer->getMaterial();
			material.SpecularColor.set(color.alpha(),color.red(),color.green(),color.blue());
		}
	}
}

void CQGlobe_ObjectAddCmd::SetEmissiveColor(QColor color)
{
	SMesh* mesh;
	if(_sampleMesh) 
		mesh=_sampleMesh;
	else if(_selectedGeoMesh)
	{
		mesh=_selectedGeoMesh;
		_selectedGeoMeshUpdated=true;
		qDebug("wsef");
	}
	else
		return;

	for(int i=0;i<mesh->getMeshBufferCount();i++)
	{
		IMeshBuffer* buffer=mesh->getMeshBuffer(i);

		video::SMaterial& material=buffer->getMaterial();
		material.EmissiveColor.set(color.alpha(),color.red(),color.green(),color.blue());
	}

	if(_sampleMesh)
	{
		IMesh* mesh= this->_pSampleMeshSceneNode->getWorldMesh();

		for(int i=0;i<mesh->getMeshBufferCount();i++)
		{
			IMeshBuffer* buffer=mesh->getMeshBuffer(i);

			video::SMaterial& material=buffer->getMaterial();
			material.EmissiveColor.set(color.alpha(),color.red(),color.green(),color.blue());
		}
	}
}


void CQGlobe_ObjectAddCmd::SetEnableLighting(bool e)
{
	SMesh* mesh;
	if(_sampleMesh) 
		mesh=_sampleMesh;
	else if(_selectedGeoMesh)
	{
		mesh=_selectedGeoMesh;
		_selectedGeoMeshUpdated=true;
	}
	else
		return;

	for(int i=0;i<mesh->getMeshBufferCount();i++)
	{
		IMeshBuffer* buffer=mesh->getMeshBuffer(i);

		video::SMaterial& material=buffer->getMaterial();
		material.Lighting=e;
	}

	if(_sampleMesh)
	{
		IMesh* mesh= this->_pSampleMeshSceneNode->getWorldMesh();

		for(int i=0;i<mesh->getMeshBufferCount();i++)
		{
			IMeshBuffer* buffer=mesh->getMeshBuffer(i);

			video::SMaterial& material=buffer->getMaterial();
			material.Lighting=e;
		}
	}
}

void CQGlobe_ObjectAddCmd::SetEnableFog(bool e)
{
	SMesh* mesh;
	if(_sampleMesh) 
		mesh=_sampleMesh;
	else if(_selectedGeoMesh)
		mesh=_selectedGeoMesh;
	else
		return;

	for(int i=0;i<mesh->getMeshBufferCount();i++)
	{
		IMeshBuffer* buffer=mesh->getMeshBuffer(i);

		video::SMaterial& material=buffer->getMaterial();
		material.FogEnable=e;
	}

	if(_sampleMesh)
	{
		IMesh* mesh= this->_pSampleMeshSceneNode->getWorldMesh();

		for(int i=0;i<mesh->getMeshBufferCount();i++)
		{
			IMeshBuffer* buffer=mesh->getMeshBuffer(i);

			video::SMaterial& material=buffer->getMaterial();
			material.FogEnable=e;
		}
	}
}

void CQGlobe_ObjectAddCmd::SetShininess(float f)
{
	SMesh* mesh;
	if(_sampleMesh) 
		mesh=_sampleMesh;
	else if(_selectedGeoMesh)
	{
		mesh=_selectedGeoMesh;
		_selectedGeoMeshUpdated=true;
	}
	else
		return;

	for(int i=0;i<mesh->getMeshBufferCount();i++)
	{
		IMeshBuffer* buffer=mesh->getMeshBuffer(i);

		video::SMaterial& material=buffer->getMaterial();
		material.Shininess=f;
	}

	if(_sampleMesh)
	{
		IMesh* mesh= this->_pSampleMeshSceneNode->getWorldMesh();

		for(int i=0;i<mesh->getMeshBufferCount();i++)
		{
			IMeshBuffer* buffer=mesh->getMeshBuffer(i);

			video::SMaterial& material=buffer->getMaterial();
			material.Shininess=f;
		}
	}
}


//precondition 
//_selectedGeoMesh is not null
void CQGlobe_ObjectAddCmd::SetSelectedGeoMeshLocation()
{
	_selectedGeoMesh->GeoInfo.Longitude=_GeoInfo.Longitude;
	_selectedGeoMesh->GeoInfo.Latitude=_GeoInfo.Latitude;
	_selectedGeoMesh->GeoInfo.Height=_GeoInfo.Height;
	_selectedGeoMesh->GeoInfo.Azimuth=_GeoInfo.Azimuth;
	_selectedGeoMeshUpdated=true;
}

//precondition _sampleMesh is not null

//for example value between 0 and 100 convert value between -50 and 50
//always z value be converted to positive value
void CQGlobe_ObjectAddCmd::NormalizeMesh(SMesh* mesh)
{
next:
	for(unsigned int i=0;i<mesh->getMeshBufferCount();i++)
	{
		IMeshBuffer* meshbuffer=mesh->getMeshBuffer(i);
		if(meshbuffer->getVertexCount()==0)
		{
			mesh->MeshBuffers.erase(i);
			goto next;
		}
	}

	core::aabbox3df boundingbox=mesh->getBoundingBox();

	core::vector3df org=(boundingbox.MinEdge+boundingbox.MaxEdge)/2;
	org=org*-1;
	org.Z = org.Z + (boundingbox.MaxEdge.Z - boundingbox.MinEdge.Z)/2;

	core::matrix4 mat;
	mat.setTranslation(org);

	mesh->recalculateBoundingBox();

	_driver->getMeshManipulator()->transformMesh(mesh,mat);
}

//flip face 
void CQGlobe_ObjectAddCmd::Flip()
{
	SMesh* mesh;
	if(_sampleMesh) 
		mesh=_sampleMesh;
	else if(_selectedGeoMesh)
	{
		mesh=_selectedGeoMesh;
		_selectedGeoMeshUpdated=true;
	}
	else
		return ;
	
	_driver->getMeshManipulator()->flipSurfaces(mesh);

	if(_sampleMesh)
		_driver->getMeshManipulator()->flipSurfaces(_pSampleMeshSceneNode->getWorldMesh());
}

void CQGlobe_ObjectAddCmd::RotateMinus90AroundXAxis()
{
	SMesh* mesh;
	if(_sampleMesh) 
		mesh=_sampleMesh;
	else if(_selectedGeoMesh)
	{
		mesh=_selectedGeoMesh;
		_selectedGeoMeshUpdated=true;
	}
	else
		return ;

	for(int i=0;i<mesh->getMeshBufferCount();i++)
	{
		SMeshBuffer *meshbuffer=(SMeshBuffer*)mesh->getMeshBuffer(i);
		
		for(int j=0;j<meshbuffer->getVertexCount();j++)
		{
			core::vector3df& vertex=meshbuffer->getPosition(j);

			float temp=vertex.Y;
			vertex.Y=-vertex.Z;
			vertex.Z=temp;
		}
	}

	for(int i=0;i<mesh->MeshBuffers.size();i++)
	{
		IMeshBuffer* meshbuffer=mesh->MeshBuffers[i];
		meshbuffer->recalculateBoundingBox();
	}

	mesh->recalculateBoundingBox();
	_driver->getMeshManipulator()->recalculateNormals(mesh);

	NormalizeMesh(mesh);
}

void CQGlobe_ObjectAddCmd::Rotate90AroundYAxis()
{
	SMesh* mesh;
	if(_sampleMesh) 
		mesh=_sampleMesh;
	else if(_selectedGeoMesh)
	{
		mesh=_selectedGeoMesh;
		_selectedGeoMeshUpdated=true;
	}
	else
		return ;

	for(int i=0;i<mesh->getMeshBufferCount();i++)
	{
		SMeshBuffer *meshbuffer=(SMeshBuffer*)mesh->getMeshBuffer(i);

		for(int j=0;j<meshbuffer->getVertexCount();j++)
		{
			core::vector3df& vertex=meshbuffer->getPosition(j);

			float temp=vertex.Z;
			vertex.Z=vertex.X;
			vertex.X=-temp;
		}
	}

	for(int i=0;i<mesh->MeshBuffers.size();i++)
	{
		IMeshBuffer* meshbuffer=mesh->MeshBuffers[i];
		meshbuffer->recalculateBoundingBox();
	}

	mesh->recalculateBoundingBox();
	_driver->getMeshManipulator()->recalculateNormals(mesh);
	NormalizeMesh(mesh);
}

void CQGlobe_ObjectAddCmd::InvertXAxis()
{
	SMesh* mesh;
	if(_sampleMesh) 
		mesh=_sampleMesh;
	else if(_selectedGeoMesh)
	{
		mesh=_selectedGeoMesh;
		_selectedGeoMeshUpdated=true;
	}
	else
		return ;

	for(int i=0;i<mesh->getMeshBufferCount();i++)
	{
		SMeshBuffer *meshbuffer=(SMeshBuffer*)mesh->getMeshBuffer(i);

		for(int j=0;j<meshbuffer->getVertexCount();j++)
		{
			core::vector3df& vertex=meshbuffer->getPosition(j);

			vertex.X=-vertex.X;
		}
	}

	for(int i=0;i<mesh->MeshBuffers.size();i++)
	{
		IMeshBuffer* meshbuffer=mesh->MeshBuffers[i];
		meshbuffer->recalculateBoundingBox();
	}
	
	Flip();
	mesh->recalculateBoundingBox();
	_driver->getMeshManipulator()->recalculateNormals(mesh);
	NormalizeMesh(mesh);
}


void CQGlobe_ObjectAddCmd::Rotate90AroundXAxis()
{
	SMesh* mesh;
	if(_sampleMesh) 
		mesh=_sampleMesh;
	else if(_selectedGeoMesh)
	{
		mesh=_selectedGeoMesh;
		_selectedGeoMeshUpdated=true;
	}
	else
		return ;

	for(int i=0;i<mesh->getMeshBufferCount();i++)
	{
		SMeshBuffer *meshbuffer=(SMeshBuffer*)mesh->getMeshBuffer(i);

		for(int j=0;j<meshbuffer->getVertexCount();j++)
		{
			core::vector3df& vertex=meshbuffer->getPosition(j);

			float temp=vertex.Y;
			vertex.Y=vertex.Z;
			vertex.Z=-temp;
		}
	}

	for(int i=0;i<mesh->MeshBuffers.size();i++)
	{
		IMeshBuffer* meshbuffer=mesh->MeshBuffers[i];
		meshbuffer->recalculateBoundingBox();
	}

	mesh->recalculateBoundingBox();
	_driver->getMeshManipulator()->recalculateNormals(mesh);
	NormalizeMesh(mesh);
}

