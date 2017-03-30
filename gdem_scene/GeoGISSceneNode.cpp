// Copyright (C) 2002-2008 Nikolaus Gebhardt
// This file is part of the "Geo Engine".
// For conditions of distribution and use, see copyright notice in geoEngine.h

#include "GeoGISSceneNode.h"
#include "CSceneManager.h"
#include "ISceneManager.h"
#include "IVideoDriver.h"
#include "GLee.h"
#include "GIS_Doc.h"
#include "GDM_DataMgr.h"
#include "GDM_Camera.h"
#include "GDM_Mem.h"
#include "config.h"
#include "gdemclient.h"
#include "GIS_Dataset.h"
#include "GDM_DataMgr.h"
#include "GDM_GISDataCache.h"
#include "GDM_PyramidMgr.h"
#include "../gds_common/GDSCommon.h"

#define		RULER_LINE_WIDE			2
#define		RULER_POINT_WIDE		4

#define POLYGON_MAX_OFFSET_RADIUS			2000

namespace geo
{
namespace scene
{

//! constructor
CGeoGISSceneNode::CGeoGISSceneNode(ISceneNode* parent, ISceneManager* mgr, s32 id)
: ISceneNode(parent, mgr, id)
{
	mat.Lighting = false;
	mat.Wireframe = true;
	mat.ZBuffer = true;
	mat.Thickness = 1;
	mat.ZWriteEnable = true;
}

//! returns the axis aligned bounding box of this node
const core::aabbox3d<f32>& CGeoGISSceneNode::getBoundingBox() const
{
	return m_Box;
}

//! pre render event
void CGeoGISSceneNode::OnRegisterSceneNode()
{
	SceneManager->registerNodeForRendering(this);
}

static bool	CheckPolygonVisible(CGIS_Polygon* pNode)
{
	CGDM_Camera *pCamera = gdm_GetDataMgr()->m_pCamera;
	if(pNode == NULL)
		return false;

	// check box
	if(pNode->m_cBound.m_cMaxPt.m_tY < pCamera->m_dMinLati || 
		pNode->m_cBound.m_cMinPt.m_tY > pCamera->m_dMaxLati)
		return false;

	if((pNode->m_cBound.m_cMaxPt.m_tX < pCamera->m_dMinLong1 || 
		pNode->m_cBound.m_cMinPt.m_tX > pCamera->m_dMaxLong1) && 
		(pNode->m_cBound.m_cMaxPt.m_tX < pCamera->m_dMinLong2 || 
		pNode->m_cBound.m_cMinPt.m_tX > pCamera->m_dMaxLong2))
		return false;

	// get area
	double area2D = pCamera->GetAreaInScreen(pNode->m_cBound.m_cMinPt.m_tX, pNode->m_cBound.m_cMaxPt.m_tX, 
		pNode->m_cBound.m_cMinPt.m_tY, pNode->m_cBound.m_cMaxPt.m_tY);

	if(area2D < 100)
		return false;

	return true;
}

//! render

void CGeoGISSceneNode::render()
{
	if(g_app->GetConfig()->m_IsLicence==false)
		return;
	CGIS_Doc *pGISDoc = gdm_GetGISDoc();
	CGIS_Node *pNode = NULL;

	if(pGISDoc == NULL)
		return;

	unsigned int index, nCnt = pGISDoc->GetCount();

	for(index = 0; index < nCnt; index++)
	{
		pNode = pGISDoc->GetNodeFromIndex(index);
		if(pNode->Get_GeoType() != E_GEONODE_TYPE_POLYGON)
			continue;

		bool visible = CheckPolygonVisible((CGIS_Polygon*)pNode);
		pNode->SetVisible(visible);

		if(!pNode->IsVisible())
			continue;

		DrawPolyNode((CGIS_Polygon*)pNode);
	}


	QList<E_GDM_SUBDATA_TYPE>		arrTypes;

	arrTypes=g_pGDMDataMgr->GetTypes(E_GDM_MAINDATA_TYPE_NAME);

	GISLayer* layer;

	for(int i=0;i< arrTypes.size();i++)
	{
		if(arrTypes[i]!=E_GDM_SUBDATA_TYPE_NAME_ACCIDENT)
		{
			if(!g_pGDMDataMgr->IsVisible(arrTypes[i]))
				continue;

			if(g_pGDMDataMgr->IsInvalidLevel(arrTypes[i],g_pGDMDataMgr->m_pPyramidMgr->GetMaxLevel()))
				continue;
        }

		if(arrTypes[i]==E_GDM_SUBDATA_TYPE_NAME_ACCIDENT)
		{
			if(!g_pGDMDataMgr->IsVisible(E_GDM_SUBDATA_TYPE_NAME_POLUTANT))
				continue;
			if(g_pGDMDataMgr->IsInvalidLevel(E_GDM_SUBDATA_TYPE_NAME_POLUTANT,g_pGDMDataMgr->m_pPyramidMgr->GetMaxLevel()))
				continue;
		}

		layer=g_pGDMDataMgr->m_pGISDataCache->GetLayer(0,0,0,arrTypes[i]);

		if(layer==NULL) continue;

		if(layer->GetType()!=GISLayer::GISPath)
			continue;

		for(int k=0;k<layer->FeatureCount();k++)
		{
			CGIS_Area* pNode=(CGIS_Area*)layer->GetFeature(k);

			bool visible = CheckPolygonVisible((CGIS_Polygon*)pNode);
			pNode->SetVisible(visible);

			if(!pNode->IsVisible())
				continue;

			DrawPolyNode((CGIS_Polygon*)pNode);
		}
	}

	DrawTrackPts();
}

double CGeoGISSceneNode::GetOffsetRadius(CGIS_Node *pNode)
{
	CGIS_Polygon *pPolyNode = NULL;
	if(pNode == NULL)
		return 0;

	if(pNode->Get_GeoType() != E_GEONODE_TYPE_POLYGON)
		return 0;

	pPolyNode = (CGIS_Polygon*)pNode;

	if(pPolyNode->m_nHeightType != E_GISPOLYHEIGHT_CLAMPED_TO_GROUND)
		return 0;

	double offset = 0;
	// get offset
	offset = gdm_GetDataMgr()->m_pCamera->m_location.m_dDist / 200.0;

	if(offset > POLYGON_MAX_OFFSET_RADIUS)
		offset = POLYGON_MAX_OFFSET_RADIUS;

	return offset;
}

void CGeoGISSceneNode::DrawTrackPts()
{
	// draw track pts
	CGIS_Doc *pGISDoc = gdm_GetGISDoc();
	if(pGISDoc == NULL)
		return;

	CGIS_Node *pNode = pGISDoc->GetSelectNode();
	if(!pNode || !pNode->IsVisible()) 	
		return;

	if(pNode->Get_GeoType() != E_GEONODE_TYPE_POLYGON)
		return;

	video::SColor clr;
	video::IVideoDriver* driver = SceneManager->getVideoDriver();

	glDisable(GL_DEPTH_TEST);

	unsigned int id, count = pNode->Get_TrackCount();
	for(id = 0; id < count; id++)
	{
		// set track color
		if(id == pNode->Get_FocusTrackID())
			clr = video::SColor(255, 0,255, 0);
		else
			clr = video::SColor(255, 255, 0, 0);

		// get screen pos
		float offset = GIS_SELECT_RANGE;
		CGDM_Location3D pt;
		CGDM_Point3DF pos;
		GDM_LOCATION loc;
		pNode->Get_TrackPt(id, &pt);
		loc.m_dLongitude = pt.m_tX;
		loc.m_dLatitude = pt.m_tY;
		loc.m_dDist = pt.m_tZ;
		gdm_GetPosition(&loc, &pos);
		if(gdm_IsBackFace(pos.m_tX, pos.m_tY, pos.m_tZ))
			continue;
		gdm_WorldToScreen(&pos);
		core::rect<s32> aRc = core::rect<s32>((s32)(pos.m_tX - offset), (s32)(pos.m_tY - offset), 
			(s32)(pos.m_tX + offset), (s32)(pos.m_tY + offset));
		driver->draw2DRectangle(clr, aRc);
	}
	glEnable(GL_DEPTH_TEST);

}

void CGeoGISSceneNode::DrawPolyNode(CGIS_Polygon *pPolyNode)
{
	video::IVideoDriver* driver = SceneManager->getVideoDriver();
	mat.Thickness	= pPolyNode->m_sPen.width;
	mat.Wireframe	= true;
	driver->setMaterial(mat);

	// enable transparent
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	CGDM_LocationArr locArr;

	core::array<core::vector3df> vertexArray;
	core::vector3df pt, centerPt;
	GDM_POINT3D	pos;
	int index, count;

	// get offset
	m_fOffsetRadius = GetOffsetRadius(pPolyNode);

	if(pPolyNode->m_nDrawType == E_GISDRAWPOLY_NONE)
		return;

	if(pPolyNode->GetCount() < 2)
		return;

	video::SColor lineColor(pPolyNode->m_sPen.color);
	video::SColor fillColor(pPolyNode->m_sBrush.color);

	pPolyNode->GetDrawSegPoints(locArr, gdm_GetDataMgr()->m_pCamera);

	count = locArr.size();

	for(index = 0; index < count; index++)
	{
		gdm_get3DBodyPoint(locArr[index].m_tX, locArr[index].m_tY, pos, locArr[index].m_tZ + m_fOffsetRadius);
		pt.X = pos.m_X;
		pt.Y = - pos.m_Y;
		pt.Z = pos.m_Z;
		vertexArray.push_back(pt);
	}
	
	centerPt.X = -g_OffsetBodyPt.m_tX;
	centerPt.Y = g_OffsetBodyPt.m_tY;
	centerPt.Z = -g_OffsetBodyPt.m_tZ;

	// draw line
	if(pPolyNode->IsDrawVectorLine())
	{
		driver->draw3DPolyline(vertexArray, pPolyNode->m_blClose, lineColor);
		if(pPolyNode->m_blHeight)
		{
			for(index = 0; index < count; index++)
			{
				driver->draw3DLine(vertexArray[index], centerPt, lineColor);
			}
		}
	}

	mat.Wireframe	= false;
	driver->setMaterial(mat);

	// fill side
	if(pPolyNode->m_blHeight && pPolyNode->IsFill())
	{
		DrawPolySideSurface(vertexArray, centerPt, fillColor);
	}

	// fill top surface of polygon
	CGDM_MeshArr *pMeshArr = pPolyNode->GetDrawCoverMesh();
	if(pMeshArr->size() > 0)
		DrawCoverPolygon(*pMeshArr, fillColor);
}

void CGeoGISSceneNode::DrawPolySideSurface(core::array<core::vector3df> arrPt, core::vector3df &centerPt, video::SColor clr)
{
	unsigned int i, count = arrPt.size();
	unsigned int vertexCnt = count + 1;
	unsigned int indexCnt  = (count - 1) * 3;

	if(count < 2)
		return;

	video::S3DVertex *	pVetex = (video::S3DVertex *)gdmMemMalloc(vertexCnt * sizeof(video::S3DVertex));
	u16*				pIndex	= (u16*)gdmMemMalloc(indexCnt * sizeof(u16));

	// set vertex buf
	memset(pVetex, 0, vertexCnt * sizeof(video::S3DVertex));

	pVetex[0].Pos = centerPt;
	for(i = 0; i < count; i++)
	{
		pVetex[i+1].Pos = arrPt[i];
		pVetex[i+1].Color = clr;
	}

	// set index buf
	for(i = 0; i < count - 1; i++)
	{
		pIndex[i * 3]		= 0;
		pIndex[i * 3 + 1]	= i + 1;
		pIndex[i * 3 + 2]	= i + 2;
	}

	glDisable(GL_CULL_FACE);
	glDisable(GL_TEXTURE);
	video::IVideoDriver* driver = SceneManager->getVideoDriver();
	driver->drawVertexPrimitiveList(pVetex, vertexCnt, pIndex, (count - 1) , video::EVT_STANDARD, scene::EPT_TRIANGLES, video::EIT_16BIT);
	
	gdmMemFree(pVetex);
	gdmMemFree(pIndex);

}

void CGeoGISSceneNode::DrawCoverPolygon(CGDM_MeshArr &meshs, video::SColor clr)
{
	unsigned int i, count = meshs.size();
	unsigned int vertexCnt = count * 3;
	unsigned int indexCnt  = count * 3;	
	GDM_POINT3D	pos;

	video::S3DVertex *	pVetex = (video::S3DVertex *)gdmMemMalloc(vertexCnt * sizeof(video::S3DVertex));
	u16*				pIndex	= (u16*)gdmMemMalloc(indexCnt * sizeof(u16));

	// set vertex buf
	memset(pVetex, 0, vertexCnt * sizeof(video::S3DVertex));

	for(i = 0; i < count; i++)
	{
		gdm_get3DBodyPoint(meshs[i].pt1.m_tX, meshs[i].pt1.m_tY, pos, meshs[i].pt1.m_tZ + m_fOffsetRadius);
		pVetex[i * 3].Pos.X = pos.m_X;
		pVetex[i * 3].Pos.Y = - pos.m_Y;
		pVetex[i * 3].Pos.Z = pos.m_Z;
		pVetex[i * 3].Color = clr;
		gdm_get3DBodyPoint(meshs[i].pt2.m_tX, meshs[i].pt2.m_tY, pos, meshs[i].pt2.m_tZ + m_fOffsetRadius);
		pVetex[i * 3 + 1].Pos.X = pos.m_X;
		pVetex[i * 3 + 1].Pos.Y = - pos.m_Y;
		pVetex[i * 3 + 1].Pos.Z = pos.m_Z;
		pVetex[i * 3 + 1].Color = clr;
		gdm_get3DBodyPoint(meshs[i].pt3.m_tX, meshs[i].pt3.m_tY, pos, meshs[i].pt3.m_tZ + m_fOffsetRadius);
		pVetex[i * 3 + 2].Pos.X = pos.m_X;
		pVetex[i * 3 + 2].Pos.Y = - pos.m_Y;
		pVetex[i * 3 + 2].Pos.Z = pos.m_Z;
		pVetex[i * 3 + 2].Color = clr;
	}

	// set index buf
	for(i = 0; i < count; i++)
	{
		pIndex[i * 3]		= i * 3;
		pIndex[i * 3 + 1]	= i * 3 + 1;
		pIndex[i * 3 + 2]	= i * 3 + 2;
	}

	glDisable(GL_CULL_FACE);
	glDisable(GL_TEXTURE);
	video::IVideoDriver* driver = SceneManager->getVideoDriver();
	driver->drawVertexPrimitiveList(pVetex, vertexCnt, pIndex, count, video::EVT_STANDARD, scene::EPT_TRIANGLES, video::EIT_16BIT);

	gdmMemFree(pVetex);
	gdmMemFree(pIndex);
}

}
}
