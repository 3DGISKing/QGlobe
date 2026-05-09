#include "GIS_Path.h"
#include "QGlobe_DataMgr.h"
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

static void SetHeightFromDemTile(CQGlobe_LocationArr &i_pts, QGlobe_DEM_TILE* pTile)
{
	unsigned i, count = i_pts.size();
	for(i = 0; i < count; i++)
	{
		i_pts[i].m_tZ = qglobe_GetHeightFromDemTile(i_pts[i], pTile);
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
	CQGlobe_Rect2D cellBox;
	QGlobe_POSITION pos, minPos, maxPos;
	QGlobe_TILE_INFO	startInfo, endInfo;
	QGlobe_TILE_INFO	tileInfo;

	QGlobe_DEM_TILE	*pDemTile	  = NULL;
	
	if(m_pProgress)
		m_pProgress->setProgress(0);

	if(GetCount() == 0)
		return;

	// stop auto loading
	qglobe_GetDataMgr()->StopDataLoad(true);

	// init member
	double	dLengthOnSurface		= 0;
	QGlobe_POSITION sMaxRatePos;
	QGlobe_POSITION sMaxHeightPos;
	QGlobe_POSITION sMinHeightPos;

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
	double tileWidth = qglobe_TwoPI / (1 << QGlobe_MAX_DEM_LEVEL);
	startInfo.nX = (int)((m_cBound.m_cMinPt.m_tX + qglobe_PI) / tileWidth);
	startInfo.nY = (int)((m_cBound.m_cMinPt.m_tY + qglobe_PI) / tileWidth);
	endInfo.nX = (int)((m_cBound.m_cMaxPt.m_tX + qglobe_PI) / tileWidth);
	endInfo.nY = (int)((m_cBound.m_cMaxPt.m_tY + qglobe_PI) / tileWidth);

	count = (endInfo.nX - startInfo.nX + 1) * (endInfo.nY - startInfo.nY + 1);

	for(nTile = 0; nTile < count; nTile++)
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
		if(IsContain(cellBox))
			continue;

		pDemTile = qglobe_GetDataMgr()->GetDemTileForCalcu(tileInfo.nX, tileInfo.nY, QGlobe_MAX_DEM_LEVEL);
		if(pDemTile == NULL)
		{
			OnCalculateGISInfoWithoutTerrian(tileInfo.nX, tileInfo.nY, QGlobe_MAX_DEM_LEVEL, dLengthOnSurface,
											sMaxRatePos, sMaxHeightPos, sMinHeightPos);
			if(m_pProgress)
				m_pProgress->setProgress((nTile + 1.0) * 100 / count);
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

				if(IsContain(cellBox))
					continue;

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

				CQGlobe_LocationPolygonArr polyPolyArr;
				qglobe_GetCrossLineArrWithGrid(m_PtArr, cellBox.m_cMinPt.m_tX, cellBox.m_cMaxPt.m_tX, 
					cellBox.m_cMinPt.m_tY, cellBox.m_cMaxPt.m_tY, polyPolyArr, false);

				for(unsigned int nPoly = 0; nPoly < polyPolyArr.size(); nPoly++)
				{
					CQGlobe_LocationArr &cPoly = polyPolyArr[nPoly];

					// set height value
					SetHeightFromDemTile(cPoly, pDemTile);

					// ���̰��
					dLengthOnSurface +=  qglobe_GetLengthWithPolygon(cPoly, false);
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
	qglobe_GetDataMgr()->StopDataLoad(false);

}
void CGIS_Path::OnCalculateGISInfoWithoutTerrian(int m, int n, int level, double &dLengthOnSurface,
												QGlobe_POSITION &sMaxRatePos, QGlobe_POSITION &sMaxHeightPos, QGlobe_POSITION &sMinHeightPos)
{
	CQGlobe_Rect2D cellBox;
	CQGlobe_Vector3DF o_Pt;
	double dHeight = 0.0;

	double tileWidth = qglobe_TwoPI / (1 << level);

	cellBox.m_cMinPt.m_tX = tileWidth * m - qglobe_PI;
	cellBox.m_cMinPt.m_tY = tileWidth * n - qglobe_PI;
	cellBox.m_cMaxPt.m_tX = cellBox.m_cMinPt.m_tX + tileWidth;
	cellBox.m_cMaxPt.m_tY = cellBox.m_cMinPt.m_tY + tileWidth;

	if(!IsIntersect(cellBox))
		return ;

	if(IsContain(cellBox))
		return ;

	CQGlobe_LocationArr cGridPoly;
	CQGlobe_Location3D pt;
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

	CQGlobe_LocationPolygonArr polyPolyArr;
	qglobe_GetCrossLineArrWithGrid(m_PtArr, cellBox.m_cMinPt.m_tX, cellBox.m_cMaxPt.m_tX, 
		cellBox.m_cMinPt.m_tY, cellBox.m_cMaxPt.m_tY, polyPolyArr, false);

	for(unsigned int nPoly = 0; nPoly < polyPolyArr.size(); nPoly++)
	{
		CQGlobe_LocationArr &cPoly = polyPolyArr[nPoly];

		// set height value
		SetHeightFromDemTile(cPoly, NULL);

		// ���̰��
		dLengthOnSurface +=  qglobe_GetLengthWithPolygon(cPoly, false);

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
		
		m_sMaxRatePos			= pNode->m_sMaxRatePos;			// �ִ�����
		m_sMaxHeightPos			= pNode->m_sMaxHeightPos;		// �ְ�������
		m_sMinHeightPos			= pNode->m_sMinHeightPos;		// ���ϳ�����
		m_dLengthOnSurface		= pNode->m_dLengthOnSurface;		// ������������
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
