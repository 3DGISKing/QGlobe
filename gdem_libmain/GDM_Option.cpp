#include <string.h>
#include "GDM_DataMgr.h"
#include "GDM_Interface.h"
#include "GDM_TerrainPtCache.h"
#include "GDM_PyramidMgr.h"
#include "GDM_Camera.h"

#define GDM_MAX_QUALITY	100.f

void gdm_UpdateTerrainLayerState(bool state)
{
	if(state == g_pGDMDataMgr->m_sOption.blTerrain)
		return;

	g_pGDMDataMgr->m_sOption.blTerrain = state;

	if(g_pGDMDataMgr->m_sOption.blTerrain)
		g_pGDMDataMgr->m_pTerrainPtCache->SetEnableHeight(true);
	else
		g_pGDMDataMgr->m_pTerrainPtCache->SetEnableHeight(false);
}

void gdm_UpdateContourState(bool state)
{
	g_pGDMDataMgr->m_sOption.blContour = state;
}

void gdm_UpdateLayerTextureState(unsigned int state)
{
	LAYER_TEXTURE_MODE newState = (LAYER_TEXTURE_MODE)state;

	if(newState == g_pGDMDataMgr->m_sOption.layerTextureMode)
		return;

	g_pGDMDataMgr->m_sOption.layerTextureMode = newState;
	
	g_pGDMDataMgr->SetModifyFlag();
}

void gdm_SetContourOptin(ContourStateInfo* contourOptin)
{
	if(g_pGDMDataMgr == NULL)
		return;
	memcpy(&g_pGDMDataMgr->m_sOption.contourOption, contourOptin, sizeof(ContourStateInfo));

	g_pGDMDataMgr->SetModifyFlag();
}

void gdm_SetGridOtion(int gridView, int gridMode)
{
	if(g_pGDMDataMgr == NULL)
		return;

	g_pGDMDataMgr->m_sOption.gridViewInfo.gridView = gridView;
	g_pGDMDataMgr->m_sOption.gridViewInfo.gridMode = gridMode;

	g_pGDMDataMgr->SetModifyFlag();
}

void gdm_SetTopographyQuality(float quality)
{
	if(g_pGDMDataMgr == NULL)
		return;
	float curQuality = quality / GDM_MAX_QUALITY;

	if(curQuality < 0)
		curQuality = 0;
	if(curQuality > 1)
		curQuality = 1;
	
	//if(fabs(g_pGDMDataMgr->m_sOption.dem_quality_rate - curQuality) < 0.1)
	//	return;

	g_pGDMDataMgr->m_sOption.dem_quality_rate = curQuality;
	g_pGDMDataMgr->m_pPyramidMgr->Update();
	g_pGDMDataMgr->SetModifyFlag();
}
void gdm_SetDistShowMode(double distviewmode)
{
	if(g_pGDMDataMgr == NULL)
		return;
	g_pGDMDataMgr->m_sOption.dist_ViewMode = distviewmode;
}

void gdm_SetHeightMagnify(double rate)
{
	if(g_pGDMDataMgr == NULL)
		return;
	if(fabs(rate - g_pGDMDataMgr->m_sOption.dem_detail_rate) < 0.1)
		return;
	g_pGDMDataMgr->m_sOption.dem_detail_rate = rate;
	g_pGDMDataMgr->m_pTerrainPtCache->SetHeightRate(rate);
	g_pGDMDataMgr->SetModifyFlag();
	g_pGDMDataMgr->m_pCamera->UpdateCameraPos();
}
