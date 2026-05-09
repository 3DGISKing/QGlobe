#include "GIS_Area.h"
#include "QGlobe_DataMgr.h"
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


static void SetHeightFromDemTile(CQGlobe_LocationArr &i_pts, QGlobe_DEM_TILE* pTile)
{
	unsigned i, count = i_pts.size();
	for(i = 0; i < count; i++)
	{
		if(pTile == NULL)
			i_pts[i].m_tZ = 0;
		else
			i_pts[i].m_tZ = qglobe_GetHeightFromDemTile(i_pts[i], pTile);
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
		
		m_sMaxRatePos			= pNode->m_sMaxRatePos;			// �ִ�����
		m_sMaxHeightPos			= pNode->m_sMaxHeightPos;		// �ְ�������
		m_sMinHeightPos			= pNode->m_sMinHeightPos;		// ���ϳ�����
		m_dLengthOnSurface		= pNode->m_dLengthOnSurface;	// ������������
		m_dAreaOnSurface		= pNode->m_dAreaOnSurface;		// ������������
		m_dVolumeOnSurface		= pNode->m_dVolumeOnSurface;	// �������� �����ϴ°�� ü��

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
	CQGlobe_Rect2D cellBox;
	QGlobe_POSITION pos, minPos, maxPos;

	QGlobe_POSITION sMaxRatePos;
	QGlobe_POSITION sMaxHeightPos;
	QGlobe_POSITION sMinHeightPos;

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


	QGlobe_TILE_INFO	tileInfo;
	QGlobe_DEM_TILE	*pDemTile	  = NULL;
	QGlobe_TILE_INFO	startInfo, endInfo;
	CQGlobe_LocationPolygonArr PolygonArray;

	// get polyon list without twist
	qglobe_Polygon2Polygons(m_cArrDivPt, PolygonArray);

	if(GetCount() == 0)
		return;
	
	// stop auto loading
	qglobe_GetDataMgr()->StopDataLoad(true);
	if(m_pProgress)
		m_pProgress->setProgress(0);


	// get tile range
	double tileWidth = qglobe_TwoPI / (1 << QGlobe_MAX_DEM_LEVEL);
	startInfo.nX = (int)((m_cBound.m_cMinPt.m_tX + qglobe_PI) / tileWidth);
	startInfo.nY = (int)((m_cBound.m_cMinPt.m_tY + qglobe_PI) / tileWidth);
	endInfo.nX = (int)((m_cBound.m_cMaxPt.m_tX + qglobe_PI) / tileWidth);
	endInfo.nY = (int)((m_cBound.m_cMaxPt.m_tY + qglobe_PI) / tileWidth);

	cntOnArea = (endInfo.nX - startInfo.nX + 1) * (endInfo.nY - startInfo.nY + 1);

	for(nTile = 0; nTile < cntOnArea; nTile++)
	{
		tileInfo.nLevel = QGlobe_MAX_DEM_LEVEL;
		tileInfo.nX     = nTile % (endInfo.nX - startInfo.nX + 1) + startInfo.nX;
		tileInfo.nY		= nTile / (endInfo.nX - startInfo.nX + 1) + startInfo.nY;

		double tileWidth = qglobe_TwoPI / (1 << QGlobe_MAX_DEM_LEVEL);

		cellBox.m_cMinPt.m_tX = tileWidth * tileInfo.nX - qglobe_PI;
		cellBox.m_cMinPt.m_tY = tileWidth * tileInfo.nY - qglobe_PI;
		cellBox.m_cMaxPt.m_tX = cellBox.m_cMinPt.m_tX + tileWidth;
		cellBox.m_cMaxPt.m_tY = cellBox.m_cMinPt.m_tY + tileWidth;

		if(!IsIntersect(cellBox))
			continue;

		pDemTile = qglobe_GetDataMgr()->GetDemTileForCalcu(tileInfo.nX, tileInfo.nY, QGlobe_MAX_DEM_LEVEL);

		for(nPoly = 0; nPoly < (int)PolygonArray.size(); nPoly++)
		{
			CQGlobe_LocationArr &arrUserPolygonPts = PolygonArray[nPoly];

			if(pDemTile == NULL)
			{
				blComplet = false;
				OnCalculateGISInfoWithoutTerrian(tileInfo.nX, tileInfo.nY, QGlobe_MAX_DEM_LEVEL,
												dLengthOnSurface, dAreaOnSurface, dVolumeOnSurface,
												sMaxRatePos, sMaxHeightPos, sMinHeightPos,
												arrUserPolygonPts);
				if(m_pProgress)
					m_pProgress->setProgress((nTile + 1.0) * 100 / cntOnArea);
				continue;
			}

			for(nX = 0; nX < QGLOBE_MAX_DEM_DIVISION; nX++)
			{
				for(nY = 0; nY < QGLOBE_MAX_DEM_DIVISION; nY++)
				{
					cellBox.m_cMinPt.m_tX = pDemTile->minLongitude + pDemTile->gridWidth * nX;
					cellBox.m_cMinPt.m_tY = pDemTile->minLatitude  + pDemTile->gridWidth * nY;
					cellBox.m_cMaxPt.m_tX = cellBox.m_cMinPt.m_tX + pDemTile->gridWidth;
					cellBox.m_cMaxPt.m_tY = cellBox.m_cMinPt.m_tY + pDemTile->gridWidth;

					if(!IsIntersect(cellBox))
						continue;

					bool isContain;
					isContain = IsContain(cellBox);

					CQGlobe_LocationArr cGridPoly;
					CQGlobe_Location3D pt;
					pt.m_tX = cellBox.m_cMinPt.m_tX;
					pt.m_tY = cellBox.m_cMinPt.m_tY;
					pt.m_tZ = pDemTile->m_pData[nY * QGLOBE_MAX_DEM_PT_COUNT + nX];
					cGridPoly.push_back(pt);
					pt.m_tX = cellBox.m_cMaxPt.m_tX;
					pt.m_tY = cellBox.m_cMinPt.m_tY;
					pt.m_tZ = pDemTile->m_pData[nY * QGLOBE_MAX_DEM_PT_COUNT + nX + 1];
					cGridPoly.push_back(pt);
					pt.m_tX = cellBox.m_cMaxPt.m_tX;
					pt.m_tY = cellBox.m_cMaxPt.m_tY;
					pt.m_tZ = pDemTile->m_pData[(nY + 1) * QGLOBE_MAX_DEM_PT_COUNT + nX + 1];
					cGridPoly.push_back(pt);
					pt.m_tX = cellBox.m_cMinPt.m_tX;
					pt.m_tY = cellBox.m_cMaxPt.m_tY;
					pt.m_tZ = pDemTile->m_pData[(nY + 1) * QGLOBE_MAX_DEM_PT_COUNT + nX];
					cGridPoly.push_back(pt);

					if(isContain)
					{
						QGLOBE_MESH mesh1, mesh2;


						mesh1.pt1 = cGridPoly[0];
						mesh1.pt2 = cGridPoly[1];
						mesh1.pt3 = cGridPoly[2];
						mesh2.pt1 = cGridPoly[0];
						mesh2.pt2 = cGridPoly[2];
						mesh2.pt3 = cGridPoly[3];

						// �������
						dAreaOnSurface += GetAreaInMeshWithDem(mesh1);
						dAreaOnSurface += GetAreaInMeshWithDem(mesh2);

						// ü�����
						dVolumeOnSurface	+= GetVolumeInMeshWithDem(mesh1);
						dVolumeOnSurface	+= GetVolumeInMeshWithDem(mesh2);

						// �ִ����� ���
						qglobe_GetMaxRatePos(cGridPoly, pos);
						if(sMaxRatePos.rate < pos.rate)
							sMaxRatePos = pos;

						// �ִ�/�ּҳ��� ���
						qglobe_GetMinMaxHeightPos(cGridPoly, minPos, maxPos);
						if(sMaxHeightPos.height < maxPos.height)
							sMaxHeightPos = maxPos;
						if(sMinHeightPos.height > minPos.height)
							sMinHeightPos = minPos;
					}
					else
					{
						CQGlobe_LocationPolygonArr polyPolyArr;
						CQGlobe_LocationPolygonArr crossLineArr;
						int nCrossLineCnt = 0;
						qglobe_GetPolygonArrWithGrid(arrUserPolygonPts, cellBox.m_cMinPt.m_tX, cellBox.m_cMaxPt.m_tX,
							cellBox.m_cMinPt.m_tY, cellBox.m_cMaxPt.m_tY, polyPolyArr);
						nCrossLineCnt = qglobe_GetCrossLineArrWithGrid(arrUserPolygonPts, cellBox.m_cMinPt.m_tX, cellBox.m_cMaxPt.m_tX,
							cellBox.m_cMinPt.m_tY, cellBox.m_cMaxPt.m_tY, crossLineArr);

						for(unsigned int nPoly = 0; nPoly < polyPolyArr.size(); nPoly++)
						{
							CQGlobe_LocationArr &cPoly = polyPolyArr[nPoly];
							CQGlobe_MeshArr	 meshArr;

							// set height value
							SetHeightFromDemTile(cPoly, pDemTile);

							// get division triangle
							qglobe_DivisionToTriangle(cPoly, meshArr);

							for(unsigned int nMesh = 0; nMesh < meshArr.size(); nMesh++)
							{
								QGLOBE_MESH &mesh = meshArr[nMesh];

								// �������
								dAreaOnSurface += GetAreaInMeshWithDem(mesh);

								// ü�����
								dVolumeOnSurface += GetVolumeInMeshWithDem(mesh);
							}

							// �ִ�/�ּҳ��� ���
							qglobe_GetMinMaxHeightPos(cPoly, minPos, maxPos);
							if(sMaxHeightPos.height < maxPos.height)
								sMaxHeightPos = maxPos;
							if(sMinHeightPos.height > minPos.height)
								sMinHeightPos = minPos;
						}

						// �ִ����� ���
						if (polyPolyArr.size() > 0)
						{						
							qglobe_GetMaxRatePos(cGridPoly, pos);
							if(sMaxRatePos.rate < pos.rate)
								sMaxRatePos = pos;
						}

						// ���̰��
						for(unsigned int nPoly = 0; nPoly < crossLineArr.size(); nPoly++)
						{
							CQGlobe_LocationArr &cPoly = crossLineArr[nPoly];

							// set height value
							SetHeightFromDemTile(cPoly, pDemTile);

							// ���̰��
							if (nCrossLineCnt == -1)
								dLengthOnSurface +=  qglobe_GetLengthWithPolygon(cPoly, true);
							else
								dLengthOnSurface +=  qglobe_GetLengthWithPolygon(cPoly, false);
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
	qglobe_GetDataMgr()->StopDataLoad(false);
}


void CGIS_Area::OnCalculateGISInfoWithoutTerrian(int m, int n, int level,
											double &dLengthOnSurface, double &dAreaOnSurface, double &dVolumeOnSurface,
											QGlobe_POSITION &sMaxRatePos, QGlobe_POSITION &sMaxHeightPos, QGlobe_POSITION &sMinHeightPos,
											CQGlobe_LocationArr	&arrUserPolygonPts)
{
	CQGlobe_Rect2D cellBox;
	CQGlobe_Vector3DF o_Pt;
	double dHeight = 0.0;
	bool isContain;

	double tileWidth = qglobe_TwoPI / (1 << level);

	cellBox.m_cMinPt.m_tX = tileWidth * m - qglobe_PI;
	cellBox.m_cMinPt.m_tY = tileWidth * n - qglobe_PI;
	cellBox.m_cMaxPt.m_tX = cellBox.m_cMinPt.m_tX + tileWidth;
	cellBox.m_cMaxPt.m_tY = cellBox.m_cMinPt.m_tY + tileWidth;
	
	if(!IsIntersect(cellBox))
		return;

	isContain = IsContain(cellBox);
	

	CQGlobe_LocationArr cGridPoly;
	CQGlobe_Location3D pt;
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
		QGLOBE_MESH mesh1, mesh2;


		mesh1.pt1 = cGridPoly[0];
		mesh1.pt2 = cGridPoly[1];
		mesh1.pt3 = cGridPoly[2];
		mesh2.pt1 = cGridPoly[0];
		mesh2.pt2 = cGridPoly[2];
		mesh2.pt3 = cGridPoly[3];
		pt = cGridPoly[0];

		// �������
		dAreaOnSurface += GetAreaInMeshWithDem(mesh1);
		dAreaOnSurface += GetAreaInMeshWithDem(mesh2);

		// ü�����
		dVolumeOnSurface	+= GetVolumeInMeshWithDem(mesh1);
		dVolumeOnSurface	+= GetVolumeInMeshWithDem(mesh2);

		// �ִ����� ���
		if(sMaxRatePos.rate <= 0)
		{
			sMaxRatePos.rate = 0;
			sMaxRatePos.longitude = pt.m_tX;
			sMaxRatePos.latitude  = pt.m_tY;
		}

		// �ִ�/�ּҳ��� ���
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
		CQGlobe_LocationPolygonArr polyPolyArr;
		CQGlobe_LocationPolygonArr crossLineArr;
		int nCrossLineCnt = 0;
		qglobe_GetPolygonArrWithGrid(arrUserPolygonPts, cellBox.m_cMinPt.m_tX, cellBox.m_cMaxPt.m_tX,
			cellBox.m_cMinPt.m_tY, cellBox.m_cMaxPt.m_tY, polyPolyArr);
		nCrossLineCnt = qglobe_GetCrossLineArrWithGrid(arrUserPolygonPts, cellBox.m_cMinPt.m_tX, cellBox.m_cMaxPt.m_tX,
			cellBox.m_cMinPt.m_tY, cellBox.m_cMaxPt.m_tY, crossLineArr);

		for(unsigned int nPoly = 0; nPoly < polyPolyArr.size(); nPoly++)
		{
			CQGlobe_LocationArr &cPoly = polyPolyArr[nPoly];
			CQGlobe_MeshArr	 meshArr;

			// set height value
			SetHeightFromDemTile(cPoly, NULL);

			// get division triangle
			qglobe_DivisionToTriangle(cPoly, meshArr);

			for(unsigned int nMesh = 0; nMesh < meshArr.size(); nMesh++)
			{
				QGLOBE_MESH &mesh = meshArr[nMesh];

				// �������
				dAreaOnSurface += GetAreaInMeshWithDem(mesh);

				// ü�����
				dVolumeOnSurface += GetVolumeInMeshWithDem(mesh);
			}


			pt = cPoly[0];

			// �ִ����� ���
			if(sMaxRatePos.rate <= 0)
			{
				sMaxRatePos.rate = 0;
				sMaxRatePos.longitude = pt.m_tX;
				sMaxRatePos.latitude  = pt.m_tY;
			}

			// �ִ�/�ּҳ��� ���
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

		// ���̰��
		for(unsigned int nPoly = 0; nPoly < crossLineArr.size(); nPoly++)
		{
			CQGlobe_LocationArr &cPoly = crossLineArr[nPoly];

			// set height value
			SetHeightFromDemTile(cPoly, NULL);

			// ���̰��
			if (nCrossLineCnt == -1)
				dLengthOnSurface +=  qglobe_GetLengthWithPolygon(cPoly, true);
			else
				dLengthOnSurface +=  qglobe_GetLengthWithPolygon(cPoly, false);
		}
	}
}

double CGIS_Area::GetAreaInMeshWithDem(QGLOBE_MESH &mesh)
{
	double dArea = 0;
	QGLOBE_MESH convMesh;
	double dCalHeight = 0;

	switch (m_nHeightType)
	{
	case E_GISPOLYHEIGHT_CLAMPED_TO_GROUND:
	case E_GISPOLYHEIGHT_RELATIVE_TO_GROUND:
		dArea = qglobe_GetAreaInMesh(mesh);
		break;
	case E_GISPOLYHEIGHT_ABSOLUTE:
		dCalHeight = (mesh.pt1.m_tZ + mesh.pt2.m_tZ + mesh.pt3.m_tZ) / 3;
		if (dCalHeight < m_dHeight)	// ������ ���� ��������� ����������.
		{
			convMesh = mesh;
			convMesh.pt1.m_tZ = 0;
			convMesh.pt2.m_tZ = 0;
			convMesh.pt3.m_tZ = 0;

			dArea = qglobe_GetAreaInMesh(convMesh);
		}
		break;
	}

	return dArea;
}
double CGIS_Area::GetVolumeInMeshWithDem(QGLOBE_MESH &mesh)
{
	double dVolume = 0;
	QGLOBE_MESH convMesh;

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

		dVolume = qglobe_GetVolumeInMesh(convMesh, m_dHeight);
		break;
	case E_GISPOLYHEIGHT_ABSOLUTE:
		dVolume = qglobe_GetVolumeInMesh(mesh, m_dHeight);
		break;
	}

	return dVolume;
}