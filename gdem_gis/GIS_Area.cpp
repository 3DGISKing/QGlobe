#include "GIS_Area.h"
#include "GDM_DataMgr.h"
#include "GIS_funcs.h"
#include "IGIS_Progress.h"
#include <QMetaType>

CGIS_Area::CGIS_Area(void)
{
	m_nGisType		= E_GISNODE_TYPE_AREA;
	m_sPen.color	= 0xff00ffff; // green
	m_sPen.width	= 2;
	m_sBrush.color	= 0x770000ff; // blue
	m_blClose		= true;
	m_nDrawType		= E_GISDRAWPOLY_ALL;

	m_dLengthOnSurface			= 0;
	m_dAreaOnSurface			= 0;
	m_dVolumeOnSurface			= 0;
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

CGIS_Area::~CGIS_Area(void)
{
}


static void SetHeightFromDemTile(CGDM_LocationArr &i_pts, GDM_DEM_TILE* pTile)
{
	unsigned i, count = i_pts.size();
	for(i = 0; i < count; i++)
	{
		if(pTile == NULL)
			i_pts[i].m_tZ = 0;
		else
			i_pts[i].m_tZ = gdem_GetHeightFromDemTile(i_pts[i], pTile);
	}
}

void CGIS_Area::Refresh(bool clearAll)
{
	if(clearAll)
	{
		m_dLengthOnSurface		= 0;
		m_dAreaOnSurface		= 0;
		m_dVolumeOnSurface		= 0;
		memset(&m_sMaxRatePos, 0, sizeof(m_sMaxRatePos));
		memset(&m_sMaxHeightPos, 0, sizeof(m_sMaxHeightPos));
		memset(&m_sMinHeightPos, 0, sizeof(m_sMinHeightPos));

	}

	CGIS_Polygon::Refresh(clearAll);
}

CGIS_Area &	CGIS_Area::operator=(const CGIS_Area & cNode)
{
	if(this != &cNode)
	{
		CopyFrom(cNode);
	}

	return *this;
}

void CGIS_Area::CopyFrom(const CGIS_Node & cNode)
{
	if(cNode.m_nGisType == E_GISNODE_TYPE_AREA)
	{
		CGIS_Area *pNode = (CGIS_Area *)&cNode;

		CGIS_Polygon::CopyFrom(cNode);
		
		m_sMaxRatePos			= pNode->m_sMaxRatePos;			// 최대경사점
		m_sMaxHeightPos			= pNode->m_sMaxHeightPos;		// 최고높은점
		m_sMinHeightPos			= pNode->m_sMinHeightPos;		// 최하낮은점
		m_dLengthOnSurface		= pNode->m_dLengthOnSurface;	// 지형고려길이
		m_dAreaOnSurface		= pNode->m_dAreaOnSurface;		// 지형고려면적
		m_dVolumeOnSurface		= pNode->m_dVolumeOnSurface;	// 지형까지 연장하는경우 체적

	}
}

void CGIS_Area::Write(QDataStream &out)
{
	CGIS_Polygon::Write(out);

	out.writeRawData((char*)&m_sMaxRatePos, sizeof(m_sMaxRatePos));
	out.writeRawData((char*)&m_sMaxHeightPos, sizeof(m_sMaxHeightPos));
	out.writeRawData((char*)&m_sMinHeightPos, sizeof(m_sMinHeightPos));
	out << m_dLengthOnSurface;
	out << m_dAreaOnSurface;
	out << m_dVolumeOnSurface;
}

void CGIS_Area::Read(QDataStream &in)
{
	CGIS_Polygon::Read(in);

	in.readRawData((char*)&m_sMaxRatePos, sizeof(m_sMaxRatePos));
	in.readRawData((char*)&m_sMaxHeightPos, sizeof(m_sMaxHeightPos));
	in.readRawData((char*)&m_sMinHeightPos, sizeof(m_sMinHeightPos));
	in >> m_dLengthOnSurface;
	in >> m_dAreaOnSurface;
	in >> m_dVolumeOnSurface;
}

CGIS_Node* CGIS_Area::Clone()
{
	CGIS_Node *pNode = new CGIS_Area();
	pNode->CopyFrom(*this);
	return pNode;
}

void CGIS_Area::OnCalculateGISInfo(void)
{
	int cntOnArea = 0;
	int nTile, nX, nY, nPoly;
	bool blComplet = true;
	CGDM_Rect2D cellBox;
	GDM_POSITION pos, minPos, maxPos;

	GDM_POSITION sMaxRatePos;
	GDM_POSITION sMaxHeightPos;
	GDM_POSITION sMinHeightPos;

	double dLengthOnSurface = 0;
	double dAreaOnSurface = 0;
	double dVolumeOnSurface = 0;

	sMaxRatePos.longitude 	= 0;
	sMaxRatePos.latitude  	= 0;
	sMaxRatePos.height		= 0;
	sMaxRatePos.rate			= 0;
	sMaxHeightPos.longitude	= 0;
	sMaxHeightPos.latitude	= 0;
	sMaxHeightPos.height		= -100000;
	sMaxHeightPos.rate		= 0;
	sMinHeightPos.longitude	= 0;
	sMinHeightPos.latitude	= 0;
	sMinHeightPos.height		= 100000;
	sMinHeightPos.rate		= 0;


	GDM_TILE_INFO	tileInfo;
	GDM_DEM_TILE	*pDemTile	  = NULL;
	GDM_TILE_INFO	startInfo, endInfo;
	CGDM_LocationPolygonArr PolygonArray;

	// get polyon list without twist
	gdem_Polygon2Polygons(m_cArrDivPt, PolygonArray);

	if(GetCount() == 0)
		return;
	
	// stop auto loading
	gdm_GetDataMgr()->StopDataLoad(true);
	if(m_pProgress)
		m_pProgress->setProgress(0);


	// get tile range
	double tileWidth = gdm_TwoPI / (1 << GDM_MAX_DEM_LEVEL);
	startInfo.nX = (int)((m_cBound.m_cMinPt.m_tX + gdm_PI) / tileWidth);
	startInfo.nY = (int)((m_cBound.m_cMinPt.m_tY + gdm_PI) / tileWidth);
	endInfo.nX = (int)((m_cBound.m_cMaxPt.m_tX + gdm_PI) / tileWidth);
	endInfo.nY = (int)((m_cBound.m_cMaxPt.m_tY + gdm_PI) / tileWidth);

	cntOnArea = (endInfo.nX - startInfo.nX + 1) * (endInfo.nY - startInfo.nY + 1);

	for(nTile = 0; nTile < cntOnArea; nTile++)
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

		pDemTile = gdm_GetDataMgr()->GetDemTileForCalcu(tileInfo.nX, tileInfo.nY, GDM_MAX_DEM_LEVEL);

		for(nPoly = 0; nPoly < (int)PolygonArray.size(); nPoly++)
		{
			CGDM_LocationArr &arrUserPolygonPts = PolygonArray[nPoly];

			if(pDemTile == NULL)
			{
				blComplet = false;
				OnCalculateGISInfoWithoutTerrian(tileInfo.nX, tileInfo.nY, GDM_MAX_DEM_LEVEL,
												dLengthOnSurface, dAreaOnSurface, dVolumeOnSurface,
												sMaxRatePos, sMaxHeightPos, sMinHeightPos,
												arrUserPolygonPts);
				if(m_pProgress)
					m_pProgress->setProgress((nTile + 1.0) * 100 / cntOnArea);
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

					bool isContain;
					isContain = IsContain(cellBox);

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

					if(isContain)
					{
						GDEM_MESH mesh1, mesh2;


						mesh1.pt1 = cGridPoly[0];
						mesh1.pt2 = cGridPoly[1];
						mesh1.pt3 = cGridPoly[2];
						mesh2.pt1 = cGridPoly[0];
						mesh2.pt2 = cGridPoly[2];
						mesh2.pt3 = cGridPoly[3];

						// 면적계산
						dAreaOnSurface += GetAreaInMeshWithDem(mesh1);
						dAreaOnSurface += GetAreaInMeshWithDem(mesh2);

						// 체적계산
						dVolumeOnSurface	+= GetVolumeInMeshWithDem(mesh1);
						dVolumeOnSurface	+= GetVolumeInMeshWithDem(mesh2);

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
					}
					else
					{
						CGDM_LocationPolygonArr polyPolyArr;
						CGDM_LocationPolygonArr crossLineArr;
						int nCrossLineCnt = 0;
						gdem_GetPolygonArrWithGrid(arrUserPolygonPts, cellBox.m_cMinPt.m_tX, cellBox.m_cMaxPt.m_tX,
							cellBox.m_cMinPt.m_tY, cellBox.m_cMaxPt.m_tY, polyPolyArr);
						nCrossLineCnt = gdem_GetCrossLineArrWithGrid(arrUserPolygonPts, cellBox.m_cMinPt.m_tX, cellBox.m_cMaxPt.m_tX,
							cellBox.m_cMinPt.m_tY, cellBox.m_cMaxPt.m_tY, crossLineArr);

						for(unsigned int nPoly = 0; nPoly < polyPolyArr.size(); nPoly++)
						{
							CGDM_LocationArr &cPoly = polyPolyArr[nPoly];
							CGDM_MeshArr	 meshArr;

							// set height value
							SetHeightFromDemTile(cPoly, pDemTile);

							// get division triangle
							gdem_DivisionToTriangle(cPoly, meshArr);

							for(unsigned int nMesh = 0; nMesh < meshArr.size(); nMesh++)
							{
								GDEM_MESH &mesh = meshArr[nMesh];

								// 면적계산
								dAreaOnSurface += GetAreaInMeshWithDem(mesh);

								// 체적계산
								dVolumeOnSurface += GetVolumeInMeshWithDem(mesh);
							}

							// 최대/최소높이 계산
							gdem_GetMinMaxHeightPos(cPoly, minPos, maxPos);
							if(sMaxHeightPos.height < maxPos.height)
								sMaxHeightPos = maxPos;
							if(sMinHeightPos.height > minPos.height)
								sMinHeightPos = minPos;
						}

						// 최대경사점 계산
						if (polyPolyArr.size() > 0)
						{						
							gdem_GetMaxRatePos(cGridPoly, pos);
							if(sMaxRatePos.rate < pos.rate)
								sMaxRatePos = pos;
						}

						// 길이계산
						for(unsigned int nPoly = 0; nPoly < crossLineArr.size(); nPoly++)
						{
							CGDM_LocationArr &cPoly = crossLineArr[nPoly];

							// set height value
							SetHeightFromDemTile(cPoly, pDemTile);

							// 길이계산
							if (nCrossLineCnt == -1)
								dLengthOnSurface +=  gdem_GetLengthWithPolygon(cPoly, true);
							else
								dLengthOnSurface +=  gdem_GetLengthWithPolygon(cPoly, false);
						}
					}
				}
			}
		}

		if(m_pProgress)
			m_pProgress->setProgress((nTile + 1.0) * 100 / cntOnArea);
		
		if(m_pProgress && m_pProgress->isStop())
			break;
	}

	if(m_pProgress && !(m_pProgress->isStop()))
	{
		m_dLengthOnSurface = dLengthOnSurface;
		m_dAreaOnSurface = dAreaOnSurface;
		m_dVolumeOnSurface = dVolumeOnSurface;
		m_sMaxRatePos = sMaxRatePos;
		m_sMaxHeightPos = sMaxHeightPos;
		m_sMinHeightPos = sMinHeightPos;
	}

	m_blCalculated = blComplet;

	if(m_pProgress)
		m_pProgress->setProgress(100);
	// enable auto loading
	gdm_GetDataMgr()->StopDataLoad(false);
}


void CGIS_Area::OnCalculateGISInfoWithoutTerrian(int m, int n, int level,
											double &dLengthOnSurface, double &dAreaOnSurface, double &dVolumeOnSurface,
											GDM_POSITION &sMaxRatePos, GDM_POSITION &sMaxHeightPos, GDM_POSITION &sMinHeightPos,
											CGDM_LocationArr	&arrUserPolygonPts)
{
	CGDM_Rect2D cellBox;
	CGDM_Vector3DF o_Pt;
	double dHeight = 0.0;
	bool isContain;

	double tileWidth = gdm_TwoPI / (1 << level);

	cellBox.m_cMinPt.m_tX = tileWidth * m - gdm_PI;
	cellBox.m_cMinPt.m_tY = tileWidth * n - gdm_PI;
	cellBox.m_cMaxPt.m_tX = cellBox.m_cMinPt.m_tX + tileWidth;
	cellBox.m_cMaxPt.m_tY = cellBox.m_cMinPt.m_tY + tileWidth;
	
	if(!IsIntersect(cellBox))
		return;

	isContain = IsContain(cellBox);
	

	CGDM_LocationArr cGridPoly;
	CGDM_Location3D pt;
	pt.m_tX = cellBox.m_cMinPt.m_tX;
	pt.m_tY = cellBox.m_cMinPt.m_tY;
	pt.m_tZ = 0;
	cGridPoly.push_back(pt);
	pt.m_tX = cellBox.m_cMaxPt.m_tX;
	pt.m_tY = cellBox.m_cMinPt.m_tY;
	cGridPoly.push_back(pt);
	pt.m_tX = cellBox.m_cMaxPt.m_tX;
	pt.m_tY = cellBox.m_cMaxPt.m_tY;
	cGridPoly.push_back(pt);
	pt.m_tX = cellBox.m_cMinPt.m_tX;
	pt.m_tY = cellBox.m_cMaxPt.m_tY;
	cGridPoly.push_back(pt);

	if(isContain)
	{
		GDEM_MESH mesh1, mesh2;


		mesh1.pt1 = cGridPoly[0];
		mesh1.pt2 = cGridPoly[1];
		mesh1.pt3 = cGridPoly[2];
		mesh2.pt1 = cGridPoly[0];
		mesh2.pt2 = cGridPoly[2];
		mesh2.pt3 = cGridPoly[3];
		pt = cGridPoly[0];

		// 면적계산
		dAreaOnSurface += GetAreaInMeshWithDem(mesh1);
		dAreaOnSurface += GetAreaInMeshWithDem(mesh2);

		// 체적계산
		dVolumeOnSurface	+= GetVolumeInMeshWithDem(mesh1);
		dVolumeOnSurface	+= GetVolumeInMeshWithDem(mesh2);

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
	else
	{
		CGDM_LocationPolygonArr polyPolyArr;
		CGDM_LocationPolygonArr crossLineArr;
		int nCrossLineCnt = 0;
		gdem_GetPolygonArrWithGrid(arrUserPolygonPts, cellBox.m_cMinPt.m_tX, cellBox.m_cMaxPt.m_tX,
			cellBox.m_cMinPt.m_tY, cellBox.m_cMaxPt.m_tY, polyPolyArr);
		nCrossLineCnt = gdem_GetCrossLineArrWithGrid(arrUserPolygonPts, cellBox.m_cMinPt.m_tX, cellBox.m_cMaxPt.m_tX,
			cellBox.m_cMinPt.m_tY, cellBox.m_cMaxPt.m_tY, crossLineArr);

		for(unsigned int nPoly = 0; nPoly < polyPolyArr.size(); nPoly++)
		{
			CGDM_LocationArr &cPoly = polyPolyArr[nPoly];
			CGDM_MeshArr	 meshArr;

			// set height value
			SetHeightFromDemTile(cPoly, NULL);

			// get division triangle
			gdem_DivisionToTriangle(cPoly, meshArr);

			for(unsigned int nMesh = 0; nMesh < meshArr.size(); nMesh++)
			{
				GDEM_MESH &mesh = meshArr[nMesh];

				// 면적계산
				dAreaOnSurface += GetAreaInMeshWithDem(mesh);

				// 체적계산
				dVolumeOnSurface += GetVolumeInMeshWithDem(mesh);
			}


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

		// 길이계산
		for(unsigned int nPoly = 0; nPoly < crossLineArr.size(); nPoly++)
		{
			CGDM_LocationArr &cPoly = crossLineArr[nPoly];

			// set height value
			SetHeightFromDemTile(cPoly, NULL);

			// 길이계산
			if (nCrossLineCnt == -1)
				dLengthOnSurface +=  gdem_GetLengthWithPolygon(cPoly, true);
			else
				dLengthOnSurface +=  gdem_GetLengthWithPolygon(cPoly, false);
		}
	}
}

double CGIS_Area::GetAreaInMeshWithDem(GDEM_MESH &mesh)
{
	double dArea = 0;
	GDEM_MESH convMesh;
	double dCalHeight = 0;

	switch (m_nHeightType)
	{
	case E_GISPOLYHEIGHT_CLAMPED_TO_GROUND:
	case E_GISPOLYHEIGHT_RELATIVE_TO_GROUND:
		dArea = gdem_GetAreaInMesh(mesh);
		break;
	case E_GISPOLYHEIGHT_ABSOLUTE:
		dCalHeight = (mesh.pt1.m_tZ + mesh.pt2.m_tZ + mesh.pt3.m_tZ) / 3;
		if (dCalHeight < m_dHeight)	// 지면이 물에 잠겼을때만 수평면적계산.
		{
			convMesh = mesh;
			convMesh.pt1.m_tZ = 0;
			convMesh.pt2.m_tZ = 0;
			convMesh.pt3.m_tZ = 0;

			dArea = gdem_GetAreaInMesh(convMesh);
		}
		break;
	}

	return dArea;
}
double CGIS_Area::GetVolumeInMeshWithDem(GDEM_MESH &mesh)
{
	double dVolume = 0;
	GDEM_MESH convMesh;

	switch (m_nHeightType)
	{
	case E_GISPOLYHEIGHT_CLAMPED_TO_GROUND:
		dVolume = 0;
		break;
	case E_GISPOLYHEIGHT_RELATIVE_TO_GROUND:
		convMesh = mesh;
		convMesh.pt1.m_tZ = 0;
		convMesh.pt2.m_tZ = 0;
		convMesh.pt3.m_tZ = 0;

		dVolume = gdem_GetVolumeInMesh(convMesh, m_dHeight);
		break;
	case E_GISPOLYHEIGHT_ABSOLUTE:
		dVolume = gdem_GetVolumeInMesh(mesh, m_dHeight);
		break;
	}

	return dVolume;
}