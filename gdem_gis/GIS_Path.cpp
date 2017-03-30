#include "GIS_Path.h"
#include "GDM_DataMgr.h"
#include "GIS_funcs.h"
#include "IGIS_Progress.h"
#include <QMetaType>

CGIS_Path::CGIS_Path(void)
{
	m_nGisType		= E_GISNODE_TYPE_PATH;
	m_sPen.color	= 0xff00ff00; // green
	m_sPen.width	= 2;
	m_sBrush.color	= 0x77333333; // gray
	m_blClose		= false;

	m_dLengthOnSurface			= 0;
	m_sMaxRatePos.longitude 	= 0;
	m_sMaxRatePos.latitude  	= 0;
	m_sMaxRatePos.height		= 0;
	m_sMaxRatePos.rate			= 0;
	m_sMaxHeightPos.longitude	= 0;
	m_sMaxHeightPos.latitude	= 0;
	m_sMaxHeightPos.height		= 0;
	m_sMaxHeightPos.rate		= 0;
	m_sMinHeightPos.longitude	= 0;
	m_sMinHeightPos.latitude	= 0;
	m_sMinHeightPos.height		= 0;
	m_sMinHeightPos.rate		= 0;

}

CGIS_Path::~CGIS_Path(void)
{
}

static void SetHeightFromDemTile(CGDM_LocationArr &i_pts, GDM_DEM_TILE* pTile)
{
	unsigned i, count = i_pts.size();
	for(i = 0; i < count; i++)
	{
		i_pts[i].m_tZ = gdem_GetHeightFromDemTile(i_pts[i], pTile);
	}
}


void CGIS_Path::Refresh(bool clearAll)
{
	if(clearAll)
	{
		m_dLengthOnSurface		= 0;
		memset(&m_sMaxRatePos, 0, sizeof(m_sMaxRatePos));
		memset(&m_sMaxHeightPos, 0, sizeof(m_sMaxHeightPos));
		memset(&m_sMinHeightPos, 0, sizeof(m_sMinHeightPos));
	}

	CGIS_Polygon::Refresh(clearAll);
}


void CGIS_Path::OnCalculateGISInfo(void)
{
	int count = 0;
	int nTile, nX, nY;
	CGDM_Rect2D cellBox;
	GDM_POSITION pos, minPos, maxPos;
	GDM_TILE_INFO	startInfo, endInfo;
	GDM_TILE_INFO	tileInfo;

	GDM_DEM_TILE	*pDemTile	  = NULL;
	
	if(m_pProgress)
		m_pProgress->setProgress(0);

	if(GetCount() == 0)
		return;

	// stop auto loading
	gdm_GetDataMgr()->StopDataLoad(true);

	// init member
	double	dLengthOnSurface		= 0;
	GDM_POSITION sMaxRatePos;
	GDM_POSITION sMaxHeightPos;
	GDM_POSITION sMinHeightPos;

	sMaxRatePos.longitude 	= 0;
	sMaxRatePos.latitude  	= 0;
	sMaxRatePos.height		= 0;
	sMaxRatePos.rate		= 0;
	sMaxHeightPos.longitude	= 0;
	sMaxHeightPos.latitude	= 0;
	sMaxHeightPos.height	= -100000;
	sMaxHeightPos.rate		= 0;
	sMinHeightPos.longitude	= 0;
	sMinHeightPos.latitude	= 0;
	sMinHeightPos.height	= 100000;
	sMinHeightPos.rate		= 0;

	// get tile range
	double tileWidth = gdm_TwoPI / (1 << GDM_MAX_DEM_LEVEL);
	startInfo.nX = (int)((m_cBound.m_cMinPt.m_tX + gdm_PI) / tileWidth);
	startInfo.nY = (int)((m_cBound.m_cMinPt.m_tY + gdm_PI) / tileWidth);
	endInfo.nX = (int)((m_cBound.m_cMaxPt.m_tX + gdm_PI) / tileWidth);
	endInfo.nY = (int)((m_cBound.m_cMaxPt.m_tY + gdm_PI) / tileWidth);

	count = (endInfo.nX - startInfo.nX + 1) * (endInfo.nY - startInfo.nY + 1);

	for(nTile = 0; nTile < count; nTile++)
	{
		tileInfo.nLevel = GDM_MAX_DEM_LEVEL;
		tileInfo.nX     = nTile % (endInfo.nX - startInfo.nX + 1) + startInfo.nX;
		tileInfo.nY		= nTile / (endInfo.nX - startInfo.nX + 1) + startInfo.nY;

		double tileWidth = gdm_TwoPI / (1 << GDM_MAX_DEM_LEVEL);

		cellBox.m_cMinPt.m_tX = tileWidth * tileInfo.nX - gdm_PI;
		cellBox.m_cMinPt.m_tY = tileWidth * tileInfo.nY - gdm_PI;
		cellBox.m_cMaxPt.m_tX = cellBox.m_cMinPt.m_tX + tileWidth;
		cellBox.m_cMaxPt.m_tY = cellBox.m_cMinPt.m_tY + tileWidth;

		if(!IsIntersect(cellBox))
			continue;
		if(IsContain(cellBox))
			continue;

		pDemTile = gdm_GetDataMgr()->GetDemTileForCalcu(tileInfo.nX, tileInfo.nY, GDM_MAX_DEM_LEVEL);
		if(pDemTile == NULL)
		{
			OnCalculateGISInfoWithoutTerrian(tileInfo.nX, tileInfo.nY, GDM_MAX_DEM_LEVEL, dLengthOnSurface,
											sMaxRatePos, sMaxHeightPos, sMinHeightPos);
			if(m_pProgress)
				m_pProgress->setProgress((nTile + 1.0) * 100 / count);
			continue;
		}

		for(nX = 0; nX < GDEM_MAX_DEM_DIVISION; nX++)
		{
			for(nY = 0; nY < GDEM_MAX_DEM_DIVISION; nY++)
			{
				cellBox.m_cMinPt.m_tX = pDemTile->minLongitude + pDemTile->gridWidth * nX;
				cellBox.m_cMinPt.m_tY = pDemTile->minLatitude  + pDemTile->gridWidth * nY;
				cellBox.m_cMaxPt.m_tX = cellBox.m_cMinPt.m_tX + pDemTile->gridWidth;
				cellBox.m_cMaxPt.m_tY = cellBox.m_cMinPt.m_tY + pDemTile->gridWidth;

				if(!IsIntersect(cellBox))
					continue;

				if(IsContain(cellBox))
					continue;

				CGDM_LocationArr cGridPoly;
				CGDM_Location3D pt;
				pt.m_tX = cellBox.m_cMinPt.m_tX;
				pt.m_tY = cellBox.m_cMinPt.m_tY;
				pt.m_tZ = pDemTile->m_pData[nY * GDEM_MAX_DEM_PT_COUNT + nX];
				cGridPoly.push_back(pt);
				pt.m_tX = cellBox.m_cMaxPt.m_tX;
				pt.m_tY = cellBox.m_cMinPt.m_tY;
				pt.m_tZ = pDemTile->m_pData[nY * GDEM_MAX_DEM_PT_COUNT + nX + 1];
				cGridPoly.push_back(pt);
				pt.m_tX = cellBox.m_cMaxPt.m_tX;
				pt.m_tY = cellBox.m_cMaxPt.m_tY;
				pt.m_tZ = pDemTile->m_pData[(nY + 1) * GDEM_MAX_DEM_PT_COUNT + nX + 1];
				cGridPoly.push_back(pt);
				pt.m_tX = cellBox.m_cMinPt.m_tX;
				pt.m_tY = cellBox.m_cMaxPt.m_tY;
				pt.m_tZ = pDemTile->m_pData[(nY + 1) * GDEM_MAX_DEM_PT_COUNT + nX];
				cGridPoly.push_back(pt);

				// 최대경사점 계산
				gdem_GetMaxRatePos(cGridPoly, pos);
				if(sMaxRatePos.rate < pos.rate)
					sMaxRatePos = pos;

				// 최대/최소높이 계산
				gdem_GetMinMaxHeightPos(cGridPoly, minPos, maxPos);
				if(sMaxHeightPos.height < maxPos.height)
					sMaxHeightPos = maxPos;
				if(sMinHeightPos.height > minPos.height)
					sMinHeightPos = minPos;

				CGDM_LocationPolygonArr polyPolyArr;
				gdem_GetCrossLineArrWithGrid(m_PtArr, cellBox.m_cMinPt.m_tX, cellBox.m_cMaxPt.m_tX, 
					cellBox.m_cMinPt.m_tY, cellBox.m_cMaxPt.m_tY, polyPolyArr, false);

				for(unsigned int nPoly = 0; nPoly < polyPolyArr.size(); nPoly++)
				{
					CGDM_LocationArr &cPoly = polyPolyArr[nPoly];

					// set height value
					SetHeightFromDemTile(cPoly, pDemTile);

					// 길이계산
					dLengthOnSurface +=  gdem_GetLengthWithPolygon(cPoly, false);
				}
			}
		}

		if(m_pProgress)
			m_pProgress->setProgress((nTile + 1.0) * 100 / count);
		if(m_pProgress && m_pProgress->isStop())		
			break;
	}

	if(m_pProgress && !(m_pProgress->isStop()))
	{
		m_dLengthOnSurface = dLengthOnSurface;		
		m_sMaxRatePos = sMaxRatePos;
		m_sMaxHeightPos = sMaxHeightPos;
		m_sMinHeightPos = sMinHeightPos;
	}

	if(m_pProgress)
		m_pProgress->setProgress(101.f);
	// enable auto loading
	gdm_GetDataMgr()->StopDataLoad(false);

}
void CGIS_Path::OnCalculateGISInfoWithoutTerrian(int m, int n, int level, double &dLengthOnSurface,
												GDM_POSITION &sMaxRatePos, GDM_POSITION &sMaxHeightPos, GDM_POSITION &sMinHeightPos)
{
	CGDM_Rect2D cellBox;
	CGDM_Vector3DF o_Pt;
	double dHeight = 0.0;

	double tileWidth = gdm_TwoPI / (1 << level);

	cellBox.m_cMinPt.m_tX = tileWidth * m - gdm_PI;
	cellBox.m_cMinPt.m_tY = tileWidth * n - gdm_PI;
	cellBox.m_cMaxPt.m_tX = cellBox.m_cMinPt.m_tX + tileWidth;
	cellBox.m_cMaxPt.m_tY = cellBox.m_cMinPt.m_tY + tileWidth;

	if(!IsIntersect(cellBox))
		return ;

	if(IsContain(cellBox))
		return ;

	CGDM_LocationArr cGridPoly;
	CGDM_Location3D pt;
	pt.m_tX = cellBox.m_cMinPt.m_tX;
	pt.m_tY = cellBox.m_cMinPt.m_tY;
	pt.m_tZ = 0;
	cGridPoly.push_back(pt);
	pt.m_tX = cellBox.m_cMaxPt.m_tX;
	pt.m_tY = cellBox.m_cMinPt.m_tY;
	pt.m_tZ = 0;
	cGridPoly.push_back(pt);
	pt.m_tX = cellBox.m_cMaxPt.m_tX;
	pt.m_tY = cellBox.m_cMaxPt.m_tY;
	pt.m_tZ = 0;
	cGridPoly.push_back(pt);
	pt.m_tX = cellBox.m_cMinPt.m_tX;
	pt.m_tY = cellBox.m_cMaxPt.m_tY;
	pt.m_tZ = 0;
	cGridPoly.push_back(pt);

	CGDM_LocationPolygonArr polyPolyArr;
	gdem_GetCrossLineArrWithGrid(m_PtArr, cellBox.m_cMinPt.m_tX, cellBox.m_cMaxPt.m_tX, 
		cellBox.m_cMinPt.m_tY, cellBox.m_cMaxPt.m_tY, polyPolyArr, false);

	for(unsigned int nPoly = 0; nPoly < polyPolyArr.size(); nPoly++)
	{
		CGDM_LocationArr &cPoly = polyPolyArr[nPoly];

		// set height value
		SetHeightFromDemTile(cPoly, NULL);

		// 길이계산
		dLengthOnSurface +=  gdem_GetLengthWithPolygon(cPoly, false);

		pt = cPoly[0];

		// 최대경사점 계산
		if(sMaxRatePos.rate <= 0)
		{
			sMaxRatePos.rate = 0;
			sMaxRatePos.longitude = pt.m_tX;
			sMaxRatePos.latitude  = pt.m_tY;
		}

		// 최대/최소높이 계산
		if(sMaxHeightPos.height <= 0)
		{
			sMaxHeightPos.height = 0;
			sMaxHeightPos.longitude = pt.m_tX;
			sMaxHeightPos.latitude  = pt.m_tY;
		}
		if(sMinHeightPos.height >= 0)
		{
			sMinHeightPos.height = 0;
			sMinHeightPos.longitude = pt.m_tX;
			sMinHeightPos.latitude  = pt.m_tY;
		}
	}
}

CGIS_Path &	CGIS_Path::operator=(const CGIS_Path & cNode)
{
	if(this != &cNode)
	{
		CopyFrom(cNode);
	}
	return *this;
}

void CGIS_Path::CopyFrom(const CGIS_Node & cNode)
{
	if(cNode.m_nGisType == E_GISNODE_TYPE_PATH)
	{
		CGIS_Path *pNode = (CGIS_Path *)&cNode;

		CGIS_Polygon::CopyFrom(cNode);
		
		m_sMaxRatePos			= pNode->m_sMaxRatePos;			// 최대경사점
		m_sMaxHeightPos			= pNode->m_sMaxHeightPos;		// 최고높은점
		m_sMinHeightPos			= pNode->m_sMinHeightPos;		// 최하낮은점
		m_dLengthOnSurface		= pNode->m_dLengthOnSurface;		// 지형고려길이
	}
}

void CGIS_Path::Write(QDataStream &out)
{
	CGIS_Polygon::Write(out);

	out.writeRawData((char*)&m_sMaxRatePos, sizeof(m_sMaxRatePos));
	out.writeRawData((char*)&m_sMaxHeightPos, sizeof(m_sMaxHeightPos));
	out.writeRawData((char*)&m_sMinHeightPos, sizeof(m_sMinHeightPos));
	out << m_dLengthOnSurface;
}

void CGIS_Path::Read(QDataStream &in)
{
	CGIS_Polygon::Read(in);

	in.readRawData((char*)&m_sMaxRatePos, sizeof(m_sMaxRatePos));
	in.readRawData((char*)&m_sMaxHeightPos, sizeof(m_sMaxHeightPos));
	in.readRawData((char*)&m_sMinHeightPos, sizeof(m_sMinHeightPos));
	in >> m_dLengthOnSurface;
}

CGIS_Node* CGIS_Path::Clone()
{
	CGIS_Node *pNode = new CGIS_Path();
	pNode->CopyFrom(*this);
	return pNode;
}
