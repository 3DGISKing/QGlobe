#include <string.h>
#include "QGlobe_DataMgr.h"
#include "QGlobe_Interface.h"
#include "QGlobe_TerrainPtCache.h"
#include "QGlobe_PyramidMgr.h"
#include "QGlobe_Camera.h"

#define QGlobe_MAX_QUALITY	100.f

void qglobe_UpdateTerrainLayerState(bool state)
{
	if(state == g_pQGlobeDataMgr->m_sOption.blTerrain)
		return;

	g_pQGlobeDataMgr->m_sOption.blTerrain = state;

	if(g_pQGlobeDataMgr->m_sOption.blTerrain)
		g_pQGlobeDataMgr->m_pTerrainPtCache->SetEnableHeight(true);
	else
		g_pQGlobeDataMgr->m_pTerrainPtCache->SetEnableHeight(false);
}

void qglobe_UpdateContourState(bool state)
{
	g_pQGlobeDataMgr->m_sOption.blContour = state;
}

void qglobe_UpdateLayerTextureState(unsigned int state)
{
	LAYER_TEXTURE_MODE newState = (LAYER_TEXTURE_MODE)state;

	if(newState == g_pQGlobeDataMgr->m_sOption.layerTextureMode)
		return;

	g_pQGlobeDataMgr->m_sOption.layerTextureMode = newState;
	
	g_pQGlobeDataMgr->SetModifyFlag();
}

void qglobe_SetContourOptin(ContourStateInfo* contourOptin)
{
	if(g_pQGlobeDataMgr == NULL)
		return;
	memcpy(&g_pQGlobeDataMgr->m_sOption.contourOption, contourOptin, sizeof(ContourStateInfo));

	g_pQGlobeDataMgr->SetModifyFlag();
}

void qglobe_SetGridOtion(int gridView, int gridMode)
{
	if(g_pQGlobeDataMgr == NULL)
		return;

	g_pQGlobeDataMgr->m_sOption.gridViewInfo.gridView = gridView;
	g_pQGlobeDataMgr->m_sOption.gridViewInfo.gridMode = gridMode;

	g_pQGlobeDataMgr->SetModifyFlag();
}

void qglobe_SetTopographyQuality(float quality)
{
	if(g_pQGlobeDataMgr == NULL)
		return;
	float curQuality = quality / QGlobe_MAX_QUALITY;

	if(curQuality < 0)
		curQuality = 0;
	if(curQuality > 1)
		curQuality = 1;
	
	//if(fabs(g_pQGlobeDataMgr->m_sOption.dem_quality_rate - curQuality) < 0.1)
	//	return;

	g_pQGlobeDataMgr->m_sOption.dem_quality_rate = curQuality;
	g_pQGlobeDataMgr->m_pPyramidMgr->Update();
	g_pQGlobeDataMgr->SetModifyFlag();
}
void qglobe_SetDistShowMode(double distviewmode)
{
	if(g_pQGlobeDataMgr == NULL)
		return;
	g_pQGlobeDataMgr->m_sOption.dist_ViewMode = distviewmode;
}

void qglobe_SetHeightMagnify(double rate)
{
	if(g_pQGlobeDataMgr == NULL)
		return;
	if(fabs(rate - g_pQGlobeDataMgr->m_sOption.dem_detail_rate) < 0.1)
		return;
	g_pQGlobeDataMgr->m_sOption.dem_detail_rate = rate;
	g_pQGlobeDataMgr->m_pTerrainPtCache->SetHeightRate(rate);
	g_pQGlobeDataMgr->SetModifyFlag();
	g_pQGlobeDataMgr->m_pCamera->UpdateCameraPos();
}
