#include "stable.h"
#include "GIS_Polygon.h"
#include "QGlobe_Mem.h"
#include "QGlobe_Coord3D.h"
#include "QGlobe_Interface.h"
#include "QGlobe_Camera.h"
#include "QGlobe_DataMgr.h"
#include "GIS_funcs.h"
#include "GIS_Doc.h"

//by RSH 2013.3.8
#include "../qglobe_libmain/QGlobe_PyramidMgr.h"
//end addition by RSH

#define BOCK_SIZE	10

CGIS_Polygon::CGIS_Polygon(void)
{
	m_nGeoType  = E_GEONODE_TYPE_POLYGON;
	m_nDrawType = E_GISDRAWPOLY_LINE;
	m_nHeightType = E_GISPOLYHEIGHT_CLAMPED_TO_GROUND;
	m_dHeight	= 0;
	
	m_dLength	= 0;
	m_dArea		= 0;
	m_dVolume	= 0;
	m_blClose	= false;
	m_blHeight	= false;

	m_dMinDivLen = PAI * 2 / (1 << 20) * 5;

	m_MeasureInfo.length = 0;
	m_MeasureInfo.angle = 0;
	m_MeasureInfo.unit = 2;
	m_sTextureName[0]=0;
}

CGIS_Polygon::~CGIS_Polygon(void)
{
	Clear();
}

void CGIS_Polygon::AddPoint(CQGlobe_Location3D &cPt)
{
	m_PtArr.push_back(cPt);
}

void CGIS_Polygon::InsertPoint(unsigned int index , CQGlobe_Location3D &cPt)
{
	if(index < 0 || (unsigned int)index >= m_PtArr.size())
	{
		m_PtArr.push_back(cPt);
		return;
	}
	m_PtArr.insert(cPt, index);
}
void CGIS_Polygon::AddPoints(CQGlobe_Location3D *pPts, int count)
{
	int i = 0;
	for(i = 0; i < count; i++)
	{
		m_PtArr.push_back(pPts[i]);
	}
}

void CGIS_Polygon::SetPoint(CQGlobe_Location3D &cPt, int index)
{
	if(index < 0 || (unsigned int)index >= m_PtArr.size())
		return;
	m_PtArr[index] = cPt;
}
void CGIS_Polygon::RemovePoint(int index)
{
	if(index < 0 || (unsigned int)index >= m_PtArr.size())
		return;
	m_PtArr.erase(index);
}

void CGIS_Polygon::Clear()
{
	if(m_PtArr.size() == 0)
		return;
	m_PtArr.clear();
}

CGIS_Polygon &	CGIS_Polygon::operator=(const CGIS_Polygon & cNode)
{
	if(this != &cNode)
	{
		CopyFrom(cNode);
	}

	return *this;
}

void CGIS_Polygon::CopyFrom(const CGIS_Node & cNode)
{
	if(cNode.m_nGeoType == E_GEONODE_TYPE_POLYGON)
	{
		CGIS_Polygon *pNode = (CGIS_Polygon *)&cNode;

		CGIS_Node::CopyFrom(cNode);
		
		m_dHeight		=	pNode->m_dHeight;			// �ع߰� (������)
		m_nDrawType		=	pNode->m_nDrawType;			// �׸��� ���� 
		m_dLength		=	pNode->m_dLength;			// ���� 
		m_dArea			=	pNode->m_dArea;				// ���� 
		m_dVolume		=	pNode->m_dVolume;			// ����ü��

		m_PtArr			=	pNode->m_PtArr;				// the points
		m_blClose		=	pNode->m_blClose;			// the flag of close
		m_blHeight		=	pNode->m_blHeight;			// check display volume flag
	}
}

void CGIS_Polygon::Write(QDataStream &out)
{
	unsigned int i, nCount = m_PtArr.size();

	CGIS_Node::Write(out);

	out << m_dHeight;
	out << m_nDrawType;
	out << m_nHeightType;
	out << m_dLength;
	out << m_dArea;
	out << m_dVolume;
	out << m_blClose;
	out << m_blHeight;

	// write pt
	out << nCount;
	for(i = 0; i < nCount; i++)
	{
		out << m_PtArr[i].m_tX;
		out << m_PtArr[i].m_tY;
		out << m_PtArr[i].m_tZ;
	}
}

void CGIS_Polygon::Read(QDataStream &in)
{
	unsigned int i, nCount, type;
	CQGlobe_Location3D pt;

	CGIS_Node::Read(in);

	in >> m_dHeight;

	in >> type;
	m_nDrawType = (E_GISDRAWPOLY_TYPE)type;
	in >> type;
	m_nHeightType = (E_GISPOLYHEIGHT_TYPE)type;

	in >> m_dLength;
	in >> m_dArea;
	in >> m_dVolume;
	in >> m_blClose;
	in >> m_blHeight;

	// write pt
	m_PtArr.clear();
	in >> nCount;
	for(i = 0; i < nCount; i++)
	{
		in >> pt.m_tX;
		in >> pt.m_tY;
		in >> pt.m_tZ;
		m_PtArr.push_back(pt);
	}

	Refresh();
}


CGIS_Node* CGIS_Polygon::Clone()
{
	CGIS_Node *pNode = new CGIS_Polygon();
	pNode->CopyFrom(*this);
	return pNode;
}

unsigned int CGIS_Polygon::Get_TrackCount()
{
	if(m_nHeightType == E_GISPOLYHEIGHT_CLAMPED_TO_GROUND)
		return m_PtArr.size();
	else
		return m_PtArr.size() * 2;
}

// track functions for select or modify
void CGIS_Polygon::Get_TrackPt(unsigned int trackIndex, CQGlobe_Location3D *pPt)
{
	unsigned int count, id;

	if(!pPt)
		return;
	count = m_PtArr.size();
	id	  = trackIndex;

	if(id >= count)
		id -= count;

	if(id < 0 || id >= count)
		return;

	m_PtArr[id].m_tZ = qglobe_getMaxHeight(m_PtArr[id].m_tX, m_PtArr[id].m_tY);
	*pPt	= m_PtArr[id];

	if(trackIndex < count)
		return;

	SetHeightValue(m_PtArr[id]);

	switch(m_nHeightType)
	{
	case E_GISPOLYHEIGHT_CLAMPED_TO_GROUND:
		break;
	//case E_GISPOLYHEIGHT_CLAMPED_TO_SEA:
	//	pPt->m_tZ = 0;
	//	break;
	case E_GISPOLYHEIGHT_RELATIVE_TO_GROUND:
		pPt->m_tZ += m_dHeight;
		break;
	//case E_GISPOLYHEIGHT_RELATIVE_TO_SEA:
	//	pPt->m_tZ = m_dHeight;
	//	break;
	case E_GISPOLYHEIGHT_ABSOLUTE:
		pPt->m_tZ = m_dHeight;
		break;
	}
}

void CGIS_Polygon::SetHeightValue(CQGlobe_Location3D &pt)
{
	double dDeltaHeight = m_dHeight;
	if(qglobe_GetDataMgr())
		pt.m_tZ = qglobe_GetDataMgr()->m_sOption.dem_detail_rate * pt.m_tZ;

	switch(m_nHeightType)
	{
	case E_GISPOLYHEIGHT_CLAMPED_TO_GROUND:
	//case E_GISPOLYHEIGHT_CLAMPED_TO_SEA:
		pt.m_tZ = qglobe_getMaxHeight(pt.m_tX, pt.m_tY);
		break;
	case E_GISPOLYHEIGHT_RELATIVE_TO_GROUND:
	//case E_GISPOLYHEIGHT_RELATIVE_TO_SEA:
		pt.m_tZ = qglobe_getMaxHeight(pt.m_tX, pt.m_tY) + dDeltaHeight;
		break;
	case E_GISPOLYHEIGHT_ABSOLUTE:
		pt.m_tZ = dDeltaHeight;
		break;
	}

}

bool CGIS_Polygon::IsDrawVectorLine()
{
	if(!m_blClose)
		return true;
	if(m_nDrawType != E_GISDRAWPOLY_LINE && m_nDrawType != E_GISDRAWPOLY_ALL)
		return false;
	if(m_nDrawType == E_GISDRAWPOLY_ALL && m_nHeightType == E_GISPOLYHEIGHT_CLAMPED_TO_GROUND)
		return false;
	return true;
}

bool CGIS_Polygon::IsFill()
{
	if(m_nDrawType == E_GISDRAWPOLY_FILL || m_nDrawType == E_GISDRAWPOLY_ALL)
		return true;
	return false;
}

void CGIS_Polygon::Set_TrackPt(unsigned int trackIndex, CQGlobe_Location3D *pPt)
{
	unsigned int count, id;

	if(!pPt)
		return;

	m_nActiveTrackId = trackIndex;

	count = m_PtArr.size();
	id	  = trackIndex;

	if(id >= count)
		id -= count;

	if(id < 0 || id >= count)
		return;

	m_PtArr[id].m_tX = pPt->m_tX;
	m_PtArr[id].m_tY = pPt->m_tY;
}

// calulation area, line, bounding box
void CGIS_Polygon::Refresh(bool clearAll)
{
	ReCalcuRange();
	ReCalcuAreaVolume();
	ReCalcuLength();
	ResetDivisionPoints();
	ReCalcuCoverMesh();
	qglobe_GetGISDoc()->SetModifyFlag();
	CheckPathData();
	if(clearAll)
		m_blCalculated = false;
	if(GetCount() == 0)
		m_blVisible = false;
}
void CGIS_Polygon::CheckPathData()
{
	if(GetCount() == 0)
		return;

	if(!m_blClose)
		return;

	if(m_nHeightType != E_GISPOLYHEIGHT_CLAMPED_TO_GROUND)
	{
		m_PathData.Clear();
		return;
	}

	unsigned int i, count;

	count = m_PtArr.size();
	
	if(m_PathData.GetCount() != count)
		m_PathData.Clear();

	for (i=0; i<count; i++)
	{
		m_PathData.Change(i, m_PtArr[i].m_tX, m_PtArr[i].m_tY);
	}
}
void CGIS_Polygon::ReCalcuAreaVolume()
{
	m_dArea = 0;
	m_dVolume = 0;
	if(!m_blClose)
		return;
	double	dArea = 0;
	bool bTwisted = false;

	

	CQGlobe_LocationArr arrPt1, arrPt2, arrPt;
	CQGlobe_LocationPolygonArr arrPolygon;
	CQGlobe_MeshArr meshArr;

	arrPt = m_PtArr;

	qglobe_DivisionToTriangle(m_PtArr, meshArr);


	int count = meshArr.size();

	//qglobe_PolygonIsTwisted(arrPt, arrPolygon);
	qglobe_Polygon2Polygons(arrPt, arrPolygon);

	int nCount = arrPolygon.size();

	// TO DO calulation area of polygon
	for (int i = 0; i < nCount; i++)
	{
		m_dArea += qglobe_GetAreaWithoutTerrain(arrPolygon[i], 0);
	}
	
	m_dVolume = m_dArea * m_dHeight;
}

void CGIS_Polygon::ReCalcuRange()
{
	if(GetCount() == 0)
		return;

	unsigned int i, count;

	m_cBound.m_cMinPt.m_tX = m_PtArr[0].m_tX;
	m_cBound.m_cMinPt.m_tY = m_PtArr[0].m_tY;
	m_cBound.m_cMaxPt.m_tX = m_PtArr[0].m_tX;
	m_cBound.m_cMaxPt.m_tY = m_PtArr[0].m_tY;

	count = m_PtArr.size();

	for (i=1; i<count; i++)
	{
		m_cBound.addInternalPoint(m_PtArr[i].m_tX, m_PtArr[i].m_tY);
	}
}

void CGIS_Polygon::ReCalcuLength()
{
	double	dblLength = 0;
	QGlobe_LOCATION pt1, pt2;

	unsigned int i, count, num;

	count = m_PtArr.size();
	if(count == 0)
		return;

	if(!m_blClose)
		num = count - 1;
	else
		num = count;
	for (i=0; i< num; i++)
	{
		pt1.m_dLongitude	= m_PtArr[i].m_tX;
		pt1.m_dLatitude		= m_PtArr[i].m_tY;
		pt2.m_dLongitude	= m_PtArr[(i+1) % count].m_tX;
		pt2.m_dLatitude		= m_PtArr[(i+1) % count].m_tY;

		double	dblValue = 0;
		if (qglobe_GetLength (NULL, &pt1, &pt2, &dblValue) == QGlobe_SUCCESS && !(count == 2 && i == 1))
			dblLength += dblValue;
	}

	m_dLength = dblLength;
}

#define		QGLOBE_MIN_DRAW_DIVISION_ANGLE		0.01 // ~PAI / 30
#define		MIN_DIVISION	5

void CGIS_Polygon::ResetDivisionPoints()
{
	unsigned int i, count, allCount, nDiv;
	CQGlobe_Location3D pt1, pt2;
	QGlobe_LOCATION start, end;
	m_cArrDivPt.clear();

	count = m_PtArr.size();

	if(count <= 1)
		return;

	pt1 = m_PtArr[0];
	m_cArrDivPt.push_back(pt1);

	allCount = count;

	if(m_blClose)
		allCount++;

	for(i = 1; i < allCount; i++)
	{
		pt1 = m_PtArr[i - 1];
		if(i < count)
			pt2 = m_PtArr[i];
		else
			pt2 = m_PtArr[0];

		pt1.m_tZ = 0;
		pt2.m_tZ = 0;
		double dist = pt1.getDistanceFrom(pt2);
		if(dist < QGLOBE_MIN_DRAW_DIVISION_ANGLE)
		{
			m_cArrDivPt.push_back(pt2);
			continue;
		}

		// division
		nDiv = (int)(dist / QGLOBE_MIN_DRAW_DIVISION_ANGLE + 1);
		start.m_dLongitude	= pt1.m_tX;
		start.m_dLatitude	= pt1.m_tY;
		start.m_dDist		= 0;
		end.m_dLongitude	= pt2.m_tX;
		end.m_dLatitude		= pt2.m_tY;
		end.m_dDist			= 0;
		if(nDiv < MIN_DIVISION)
			nDiv = MIN_DIVISION;
		GetSegPoints(&m_cArrDivPt, &start, &end, nDiv);
	}
}

void CGIS_Polygon::ReCalcuCoverMesh()
{
	unsigned int i, count;
	m_cArrMesh.clear();

	if(!m_blClose)
		return;

	if(m_nDrawType != E_GISDRAWPOLY_FILL && m_nDrawType != E_GISDRAWPOLY_ALL)
		return;

	switch(m_nHeightType)
	{
	case E_GISPOLYHEIGHT_CLAMPED_TO_GROUND:
	//case E_GISPOLYHEIGHT_CLAMPED_TO_SEA:
		return;
		break;
	case E_GISPOLYHEIGHT_RELATIVE_TO_GROUND:
	//case E_GISPOLYHEIGHT_RELATIVE_TO_SEA:
	case E_GISPOLYHEIGHT_ABSOLUTE:
		qglobe_DivisionToTriangle(m_PtArr, m_cArrMesh);
		break;
	}

	count = m_cArrMesh.size();
	for(i = 0; i < count; i++)
	{
		SetHeightValue(m_cArrMesh[i].pt1);
		SetHeightValue(m_cArrMesh[i].pt2);
		SetHeightValue(m_cArrMesh[i].pt3);
	}
}

void	CGIS_Polygon::GetSegPoints(CQGlobe_LocationArr* pLocationArr, QGlobe_LOCATION *pStart, QGlobe_LOCATION *pEnd, int nDiv)
{
	double	dHeight = 0;

	CQGlobe_Point3DF	ptStart, ptEnd;

	qglobe_GetPosition (pStart, &ptStart);
	qglobe_GetPosition (pEnd,	 &ptEnd);

	double			dAngle	 = 0;
	CQGlobe_Vector3DF	rotAxis = ptStart.crossProduct (ptEnd);

	qglobe_GetAngleBetweenTwoVector (ptStart, ptEnd, rotAxis, dAngle);
	if (fabs (dAngle) > qglobe_PI)
	{
		if (dAngle < 0)
			dAngle += qglobe_TwoPI;
		else
			dAngle -= qglobe_TwoPI;
	}

	CQGlobe_Location3D	locNode;

	double			dAngStep = dAngle / nDiv;
	CQGlobe_Point3DF	point	 = ptStart;

	int		nPts = nDiv / 2;

	if (nPts > nDiv / 6) nPts = nDiv / 6;

	double	dAngNode = dAngStep / 2;
	for (int i=1; i<nPts; i++, dAngNode += dAngStep)
	{
		point = ptStart;

		qglobe_RotateAroundVector (&point, dAngNode, rotAxis);
		qglobe_GetLocation (&point, &locNode);
		pLocationArr->push_back(locNode);
	}

	for (int i=nPts+1; i<nDiv - nPts; i++)
	{
		point = ptStart;
		dAngNode += dAngStep;

		qglobe_RotateAroundVector (&point, dAngNode, rotAxis);
		qglobe_GetLocation (&point, &locNode);
		pLocationArr->push_back(locNode);
	}

	dAngNode = dAngStep/2 - nPts * dAngStep;
	for (int i=1; i<nPts; i++, dAngNode += dAngStep)
	{
		point = ptEnd;

		qglobe_RotateAroundVector (&point, dAngNode, rotAxis);
		qglobe_GetLocation (&point, &locNode);
		pLocationArr->push_back(locNode);
	}

	qglobe_GetLocation (&ptEnd, &locNode);
	if(pLocationArr->getLast() != locNode)
		pLocationArr->push_back(locNode);
}


void CGIS_Polygon::GetDrawSegPoints(CQGlobe_LocationArr &locArr, CQGlobe_Camera *pCamera)
{
	unsigned int i, count, allCount;
	CQGlobe_Location3D pt1, pt2;
	count = m_cArrDivPt.size();

	if(count <= 1)
		return;

	switch(m_nHeightType)
	{
	case E_GISPOLYHEIGHT_RELATIVE_TO_GROUND:
		//case E_GISPOLYHEIGHT_RELATIVE_TO_SEA:
	case E_GISPOLYHEIGHT_ABSOLUTE:
		{
			locArr = m_PtArr;
			if(m_blClose)
				locArr.push_back(m_PtArr[0]);
		}
		break;
	default:
		{
			//commented out by RSH 2013.3.11 for Ruler lines to be just over ground
			////if (Get_GisType() == E_GISNODE_TYPE_PATH || 
			//if ( Get_GisType() == E_GISNODE_TYPE_RULER )
			//{
			//	locArr = m_cArrDivPt;
			//}

			//added by RSH 2013.3.6 for gis path rendering above ground
			//and revised by RSH 2013.3.11
			if (Get_GisType() == E_GISNODE_TYPE_PATH || 		
				Get_GisType() == E_GISNODE_TYPE_RULER)
			{
				///***
				double height = 0;
				int nDemDivision = 32;
				int level = g_pQGlobeDataMgr->m_pPyramidMgr->GetMaxLevel();
				if (level > 15)  nDemDivision = 128;
				if (level > 13)	 level = 13;


				double cellWidth = qglobe_TwoPI/(1<<level)/nDemDivision;

				for (i = 0; i<m_PtArr.size(); i++)
				{
					if (i == 0) 
					{
						height = qglobe_getExactHeight(m_PtArr[0].m_tX, m_PtArr[0].m_tY);
						locArr.push_back(m_PtArr[0]);
						continue;
					}
					CQGlobe_Location3D prevPt, curPt;
					prevPt = m_PtArr[i - 1];
					curPt  = m_PtArr[i];
					double dist = (prevPt.m_tX - curPt.m_tX)*(prevPt.m_tX - curPt.m_tX) + (prevPt.m_tY - curPt.m_tY)*(prevPt.m_tY - curPt.m_tY);
					dist = sqrt(dist);
					if (dist > cellWidth)
					{
						int nDiv = (int)(dist/cellWidth);
						if (nDiv < 1)
						{
							if (curPt.m_tZ == 0) curPt.m_tZ = qglobe_getExactHeight(curPt.m_tX, curPt.m_tY);
							if (curPt.m_tZ == 0) curPt.m_tZ = height;
							height = curPt.m_tZ;
							locArr.push_back(curPt);
							continue;
						}
						for (int k=0; k<nDiv; k++)
						{
							CQGlobe_Location3D pt;
							pt.m_tX = prevPt.m_tX + ((double)(k+1))/((double)(nDiv+1))*(curPt.m_tX - prevPt.m_tX);
							pt.m_tY = prevPt.m_tY + ((double)(k+1))/((double)(nDiv+1))*(curPt.m_tY - prevPt.m_tY);
							if (pt.m_tZ == 0) pt.m_tZ = qglobe_getExactHeight(pt.m_tX, pt.m_tY);
							if (pt.m_tZ == 0) pt.m_tZ = height;
							height = pt.m_tZ;
							locArr.push_back(pt);
						}
					}
					if (curPt.m_tZ == 0) curPt.m_tZ = qglobe_getExactHeight(curPt.m_tX, curPt.m_tY);
					if (curPt.m_tZ == 0) curPt.m_tZ = height;
					height = curPt.m_tZ;
					locArr.push_back(curPt);
				}
				//*/
				//locArr = m_PtArr;				
			}
			//end addition by RSH
			else if (Get_GisType() == E_GISNODE_TYPE_AREA)
			{
				pt1 = m_PtArr[0];

				locArr.push_back(pt1);

				count = m_PtArr.size();
				allCount = count;

				if (m_blClose)
					allCount++;

				for (i = 1; i < allCount; i++)
				{
					pt1 = m_PtArr[i - 1];
					if(i < count)
						pt2 = m_PtArr[i];
					else
						pt2 = m_PtArr[0];

					pt1.m_tZ = 0;
					pt2.m_tZ = 0;

					GetDrawSegPoints(&locArr, pt1, pt2, pCamera->m_location.m_dDist);
				}
			}
		}
	}
	//added by RSH 2013.3.6 to block code to set height
	if (Get_GisType() == E_GISNODE_TYPE_AREA)
	{
		for(i = 0; i < locArr.size(); i++)
			SetHeightValue(locArr[i]);

		// check cover mesh height
		count = m_cArrMesh.size();
		for(i = 0; i < count; i++)
		{
			SetHeightValue(m_cArrMesh[i].pt1);
			SetHeightValue(m_cArrMesh[i].pt2);
			SetHeightValue(m_cArrMesh[i].pt3);
		}
	}
	//end addition by RSH

}

//by RSH 2013.3.8
/*
void CGIS_Polygon::GetDrawSegPoints(CQGlobe_LocationArr &locArr, CQGlobe_Camera *pCamera)
{
	unsigned int i, count, allCount;
	CQGlobe_Location3D pt1, pt2;
	count = m_cArrDivPt.size();

	if(count <= 1)
		return;

	switch(m_nHeightType)
	{
	case E_GISPOLYHEIGHT_RELATIVE_TO_GROUND:
	//case E_GISPOLYHEIGHT_RELATIVE_TO_SEA:
	case E_GISPOLYHEIGHT_ABSOLUTE:
		{
			locArr = m_PtArr;
			if(m_blClose)
				locArr.push_back(m_PtArr[0]);
		}
		break;
	default:
		{
 			if (Get_GisType() == E_GISNODE_TYPE_PATH || 
 				Get_GisType() == E_GISNODE_TYPE_RULER)
 			{
				locArr = m_cArrDivPt;
 			}
 			else if (Get_GisType() == E_GISNODE_TYPE_AREA)
 			{
				pt1 = m_PtArr[0];

				locArr.push_back(pt1);

				count = m_PtArr.size();
				allCount = count;

				if (m_blClose)
					allCount++;

				for (i = 1; i < allCount; i++)
				{
					pt1 = m_PtArr[i - 1];
					if(i < count)
						pt2 = m_PtArr[i];
					else
						pt2 = m_PtArr[0];

					pt1.m_tZ = 0;
					pt2.m_tZ = 0;

					GetDrawSegPoints(&locArr, pt1, pt2, pCamera->m_location.m_dDist);
				}
 			}
		}
	}

	for(i = 0; i < locArr.size(); i++)
		SetHeightValue(locArr[i]);

	// check cover mesh height
	count = m_cArrMesh.size();
	for(i = 0; i < count; i++)
	{
		SetHeightValue(m_cArrMesh[i].pt1);
		SetHeightValue(m_cArrMesh[i].pt2);
		SetHeightValue(m_cArrMesh[i].pt3);
	}
}
//end revision by RSH  */
#define QGLOBE_LINE_MAX_DIVISION	50

void CGIS_Polygon::GetDrawSegPoints(CQGlobe_LocationArr* pLocArr, CQGlobe_Location3D &pt1, CQGlobe_Location3D &pt2, double distEye)
{
	double	dHeight = 0;
	int		nDiv, i;
	CQGlobe_Location3D pt;

	// get div
	nDiv = (int)(pt1.getDistanceFrom(pt2) / m_dMinDivLen);
	if(nDiv > QGLOBE_LINE_MAX_DIVISION)
		nDiv = QGLOBE_LINE_MAX_DIVISION;
	if(nDiv < 2)
	{
		pLocArr->push_back(pt2);
		return;
	}

	double dx = (pt2.m_tX - pt1.m_tX) / nDiv;
	double dy = (pt2.m_tY - pt1.m_tY) / nDiv;

	for(i = 0; i < nDiv; i++)
	{
		pt.m_tX = pt1.m_tX + dx * (i + 1);
		pt.m_tY = pt1.m_tY + dy * (i + 1);
		pLocArr->push_back(pt);
	}

//	pLocArr->push_back(pt2);

	return;
}

int	CGIS_Polygon::GetValidDivid(QGlobe_LOCATION *pStart, QGlobe_LOCATION *pEnd, double	&dHeight, double distEye)
{
	#define		MIN_DIVIDE		2
	#define		MAX_DIVIDE		1024

	dHeight	= 0;

	double	dblDist = distEye;
	double	dL		= dblDist * EARTH_RADIUS / (MIN_DIVIDE * CAMERA_DIST_MAX * 10);
	double	dAngUnit = atan(dL / EARTH_RADIUS);

	double			dAngle = 0;
	CQGlobe_Point3DF	ptStart, ptEnd, rotAxis;

	qglobe_GetPosition (pStart, &ptStart);
	qglobe_GetPosition (pEnd,	 &ptEnd);
	rotAxis = ptStart.crossProduct (ptEnd);

	qglobe_GetAngleBetweenTwoVector (ptStart, ptEnd, rotAxis, dAngle);

	if (fabs(dAngle) > qglobe_PI)
	{
		if (dAngle < 0)
			dAngle += qglobe_PI;
		else
			dAngle -= qglobe_PI;
	}

	double	nDiv = dAngle / dAngUnit;

	if (nDiv < MIN_DIVIDE)
		nDiv = MIN_DIVIDE;
	if (nDiv > MAX_DIVIDE)
		nDiv = MAX_DIVIDE;

	dHeight	= EARTH_RADIUS / cos (dAngle / nDiv) - EARTH_RADIUS;

	return	nDiv;
}

bool CGIS_Polygon::IsContain(MOUSE_POSITION &pos)
{
	CQGlobe_Location3D pt;
	int i, count = m_PtArr.size();

	pt.m_tX = pos.longitude;
	pt.m_tY = pos.latitude;

	if(pt.m_tX < (m_cBound.m_cMinPt.m_tX - pos.rangeInWorld)||
		pt.m_tX > (m_cBound.m_cMaxPt.m_tX + pos.rangeInWorld) ||
		pt.m_tY < (m_cBound.m_cMinPt.m_tY - pos.rangeInWorld) ||
		pt.m_tY > (m_cBound.m_cMaxPt.m_tY + pos.rangeInWorld))
		return false;


	if(m_blClose)
	{
		// case polygon
		return IsContain(pt.m_tX, pt.m_tY);
	}

	// polyline
	for(i = 1; i < count; i++)
	{
		CQGlobe_Location3D &pt1 = m_PtArr[i-1];
		CQGlobe_Location3D &pt2 = m_PtArr[i];
		CQGlobe_Location3D midPt;
		double dx, dy;

		dx = pt2.m_tX - pt1.m_tX;
		dy = pt2.m_tY - pt1.m_tY;

		if(!qglobe_iszero(dx))
		{
			midPt.m_tX = pt.m_tX;
			midPt.m_tY = pt1.m_tY + (midPt.m_tX - pt1.m_tX) / dx * (pt2.m_tY - pt1.m_tY);
			if(fabs(midPt.m_tY - pt.m_tY) < pos.rangeInWorld)
				return true;
		}
		else if(!qglobe_iszero(dx))
		{
			dy = pt2.m_tY - pt1.m_tY;
			midPt.m_tY = pt.m_tY;
			midPt.m_tX = pt1.m_tX + (midPt.m_tY - pt1.m_tY) / dy * (pt2.m_tX - pt1.m_tX);
			if(fabs(midPt.m_tY - pt.m_tY) < pos.rangeInWorld)
				return true;
		}
	}

	return false;
}

bool CGIS_Polygon::IsIntersect(CQGlobe_Rect2D &bound)
{
	unsigned int index, count;
	CQGlobe_Location3D *pPt1, *pPt2;
	CQGlobe_Location3D crossPt;

	if(m_cBound.m_cMaxPt.m_tX < bound.m_cMinPt.m_tX ||
		m_cBound.m_cMaxPt.m_tY < bound.m_cMinPt.m_tY ||
		m_cBound.m_cMinPt.m_tX > bound.m_cMaxPt.m_tX ||
		m_cBound.m_cMinPt.m_tY > bound.m_cMaxPt.m_tY)
		return false;

	count = m_PtArr.size();
	for(index = 0; index < count; index++)
	{
		if(bound.isPointInside(m_PtArr[index].m_tX, m_PtArr[index].m_tY))
			return true;
	}

	if(IsContainNoCheck(bound.m_cMinPt.m_tX, bound.m_cMinPt.m_tY))
		return true;
	if(IsContainNoCheck(bound.m_cMinPt.m_tX, bound.m_cMaxPt.m_tY))
		return true;
	if(IsContainNoCheck(bound.m_cMaxPt.m_tX, bound.m_cMinPt.m_tY))
		return true;
	if(IsContainNoCheck(bound.m_cMaxPt.m_tX, bound.m_cMaxPt.m_tY))
		return true;

	for(unsigned int i = 0; i < m_PtArr.size(); i++)
	{
		pPt1 = &m_PtArr[i];
		if(i == m_PtArr.size() - 1)
			pPt2 = &m_PtArr[0];
		else
			pPt2 = &m_PtArr[i + 1];

		// check intersect point 
		if(qglobe_GetCrossPtSegmentWithAxisY (bound.m_cMinPt.m_tX, bound.m_cMinPt.m_tY, bound.m_cMaxPt.m_tY, pPt1, pPt2, crossPt))
			return true;
		if(qglobe_GetCrossPtSegmentWithAxisY (bound.m_cMaxPt.m_tX, bound.m_cMinPt.m_tY, bound.m_cMaxPt.m_tY, pPt1, pPt2, crossPt))
			return true;
		if(qglobe_GetCrossPtSegmentWithAxisX (bound.m_cMinPt.m_tY, bound.m_cMinPt.m_tX, bound.m_cMaxPt.m_tX, pPt1, pPt2, crossPt))
			return true;
		if(qglobe_GetCrossPtSegmentWithAxisX (bound.m_cMaxPt.m_tY, bound.m_cMinPt.m_tX, bound.m_cMaxPt.m_tX, pPt1, pPt2, crossPt))
			return true;
	}

	return false;
}

bool CGIS_Polygon::IsContain(double x, double y)
{
	CQGlobe_Location3D pos;

	pos.m_tX = x;
	pos.m_tY = y;

	return IsContain(pos);
}

bool CGIS_Polygon::IsContainNoCheck(double x, double y)
{
	unsigned int index, count, crossCount;
	CQGlobe_Location3D *pPt1, *pPt2;
	double crossX;

	count = m_PtArr.size();
	crossCount = 0;
	for(index = 0; index < count; index++)
	{
		pPt1 = &m_PtArr[index];
		if(index == m_PtArr.size() - 1)
			pPt2 = &m_PtArr[0];
		else
			pPt2 = &m_PtArr[index + 1];

		if((pPt1->m_tY > y && pPt2->m_tY > y) || (pPt1->m_tY < y && pPt2->m_tY < y))
			continue;
		if(pPt1->m_tX < x && pPt2->m_tX < x)
			continue;

		crossX = ((pPt1->m_tX - pPt2->m_tX) * y + (pPt1->m_tY * pPt2->m_tX - pPt1->m_tX * pPt2->m_tY)) / (pPt1->m_tY - pPt2->m_tY);
		if(crossX > x)
			crossCount++;
	}

	if(crossCount % 2 != 0)
		return true;
	return false;
}

bool CGIS_Polygon::IsContain(CQGlobe_Location3D &pos)
{
	if(!m_blClose)
		return false;
	if(m_cBound.m_cMaxPt.m_tX < pos.m_tX ||
		m_cBound.m_cMaxPt.m_tY < pos.m_tY ||
		m_cBound.m_cMinPt.m_tX > pos.m_tX ||
		m_cBound.m_cMinPt.m_tY > pos.m_tY)
		return false;

	return IsContainNoCheck(pos.m_tX, pos.m_tY);
}

bool CGIS_Polygon::IsContain(CQGlobe_Rect2D &bound)
{
	CQGlobe_Location3D *pPt1, *pPt2;
	CQGlobe_Location3D crossPt;

	if(!m_blClose)
		return false;
	if(!IsIntersect(bound))
		return false;
	if(!IsContainNoCheck(bound.m_cMinPt.m_tX, bound.m_cMinPt.m_tY))
		return false;
	for(unsigned int i = 0; i < m_PtArr.size(); i++)
	{
		pPt1 = &m_PtArr[i];
		if(i == m_PtArr.size() - 1)
			pPt2 = &m_PtArr[0];
		else
			pPt2 = &m_PtArr[i + 1];

		// check intersect point 
		if(qglobe_GetCrossPtSegmentWithAxisY (bound.m_cMinPt.m_tX, bound.m_cMinPt.m_tY, bound.m_cMaxPt.m_tY, pPt1, pPt2, crossPt))
			return false;
		if(qglobe_GetCrossPtSegmentWithAxisY (bound.m_cMaxPt.m_tX, bound.m_cMinPt.m_tY, bound.m_cMaxPt.m_tY, pPt1, pPt2, crossPt))
			return false;
		if(qglobe_GetCrossPtSegmentWithAxisX (bound.m_cMinPt.m_tY, bound.m_cMinPt.m_tX, bound.m_cMaxPt.m_tX, pPt1, pPt2, crossPt))
			return false;
		if(qglobe_GetCrossPtSegmentWithAxisX (bound.m_cMaxPt.m_tY, bound.m_cMinPt.m_tX, bound.m_cMaxPt.m_tX, pPt1, pPt2, crossPt))
			return false;
	}

	return true;
}

QString CGIS_Polygon::ConvertLengthToString()
{
	double result = 0.0;
	QString length;
	QString unit;

	switch(m_MeasureInfo.unit)
	{
	case 0:
		result = m_dLength * 100;
		unit = "cm";
		break;
	case 1:
		result = m_dLength;
		unit = "m";
		break;
	case 2:
		result = m_dLength / 1000;
		unit = "km";
		break;
	case 3:
		result = (m_dLength * 100) / 2.54;
		unit = "inch";
		break;
	case 4:
		result = (m_dLength * 100) / 30.48;
		unit = "feet";
		break;
	case 5:
		result = (m_dLength * 100) / 91.4;
		unit = "yard";
		break;
	case 6:
		result = m_dLength / 1609.3;
		unit = "mile";
		break;
	default:
		break;
	}

	length = QString("%1").arg(result, 0, 'f', 2, QLatin1Char('0'));

	int len = (length.length() - 1) / 3;
	for (int i = 1; i < len; i++)
		length.insert(length.length() - 2 - (4 * i), QChar(','));

	length += unit;

	return length;
}
