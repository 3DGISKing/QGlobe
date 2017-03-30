#include "GIS_funcs.h"

typedef gdm_array<CGDM_LocationArr>	CGDM_LocationPolygonArr;	// 타일과 사용자경계선이 사귈때 생기는 폴리곤들의 배렬정의

#undef MAX
#define MAX(a,b)	((a) > (b) ? (a) : (b))
#undef MIN
#define MIN(a,b)	((a) < (b) ? (a) : (b))

#define ERROR_LIMIT 0.000000000001

typedef enum
{
	CROSS_NO,			// 사귐점이 하나도 없음
	CROSS_INCELL,		// 선분이 셀안에 놓임
	CROSS_ONE_START,	// 사귐점이 하나이고 시작점이 셀안에 놓임
	CROSS_ONE_END,		// 사귐점이 하나이고 끝점이 셀안에 놓임
	CROSS_TWO,			// 사귐점이 두개
	CROSS_UNKNOWN
} GDM_CROSS_INFO;

typedef struct
{
	CGDM_Location3D cPt1;
	CGDM_Location3D cPt2;
} GDM_SEGMENT;

typedef gdm_array<GDM_SEGMENT>	CGDM_SegmentArr;	// 선분의 배렬정의




GDM_CROSS_INFO  gdem_GetPtsSegmentWithGrid (CGDM_Location3D *pPt1, CGDM_Location3D *pPt2, double minX, double maxX, double minY, double maxY, CGDM_LocationArr &io_ptArr);


void getCrossPtsInCellBoundary(CGDM_LocationArr &io_ptArrInBoundary, double minX, double maxX, double minY, double maxY, CGDM_LocationPolygonArr &o_crossLineArr);
void insertPtToBoundary (CGDM_Location3D &crossPt, CGDM_LocationPolygonArr &io_polyArrInBorder, double minX, double maxX, double minY, double maxY);
int findPtInBoundary (CGDM_Location3D &crossPt, double minX, double maxX, double minY, double maxY);


void gdem_GetClosedPolygonArr (CGDM_LocationArr &ptArrInBoundary, CGDM_LocationPolygonArr &crossLineArr, CGDM_LocationPolygonArr &io_polygonArr);
bool isEqualPtWithCrossLine (CGDM_Location3D &compPt, CGDM_LocationPolygonArr &crossLineArr, int &nCrossLineNumber, bool &bNormal);
bool isEqualPtWithBorderLine (CGDM_Location3D &compPt, CGDM_SegmentArr &segmentArr, int &nBorderLineNumber);



/*******************************************************************************
	Function name:	gdem_GetPolygonArrWithGrid.
	Description:	하나의 쎌과 사용자경계선의 사귐으로 하여 생성되는 닫긴 폴리곤들을 장악.
	Return: 		int : 생성된 닫긴폴리곤의 개수
	input:			i_arrPt, minX, maxX, minY, maxY
	in-output:		io_polygonArr
	** : 현재 이 함수는 셀과 사용자경계선의 사귐이 있을때 호출됨
********************************************************************************/
int  gdem_GetPolygonArrWithGrid(CGDM_LocationArr &i_arrPt, double minX, double maxX, double minY, double maxY, CGDM_LocationPolygonArr &io_polygonArr)
{
	CGDM_LocationPolygonArr crossLineArr;		// 셀과 사용자경계선의 사귐으로 하여 생성되는 사귐선들의 배렬
	CGDM_LocationArr		ptArrInBoundary;	// 셀의 4점과 사용자경계선과의 사귐점들로 이루어지는 점배렬(최소점으로부터 오름나사방향)
	CGDM_Location3D			averagePt;		// 닫긴폴리곤안의 점.
	CGDM_Location3D			pt1, pt2, pt3;	// 닫긴폴리곤판정에 리용되는 세점.





	// 먼저 셀과 사용자경계선과의 사귐선들을 구한다.
	int nCrossLineCount = gdem_GetCrossLineArrWithGrid(i_arrPt, minX, maxX, minY, maxY, crossLineArr);
	if (nCrossLineCount == 0)
	{
		// 셀이 사용자경계선안에 포함관계를 따질것.
		return 0;
	}
	else if (nCrossLineCount == -1)	// 사용자경계선전체가 셀안에 놓이면.
	{
		io_polygonArr.push_back(i_arrPt);
		return 1;
	}

	// 오름나사방향으로 사귐점목록을 장악.(여기에는 셀의 네정점도 포함됨)
	getCrossPtsInCellBoundary(ptArrInBoundary, minX, maxX, minY, maxY, crossLineArr);

	// 사귐선들과 사귐점들로부터 생성되는 모든 닫긴폴리곤들을 얻어낸다.
	gdem_GetClosedPolygonArr (ptArrInBoundary, crossLineArr, io_polygonArr);

	// 사용자경계선밖에 놓이는 닫긴폴리곤들을 삭제한다.(즉 연산에 참가할 폴리곤만 추출)
	int nPolygonCount = (int) io_polygonArr.size();
	for (int i = 0; i < nPolygonCount; i++)
	{
		CGDM_LocationArr &ptArrInPolygon = io_polygonArr[i];	// 하나의 닫긴폴리곤.
		int nPtCnt = (int)ptArrInPolygon.size();

		for (int k = 0; k < nPtCnt; k++)
		{
			pt1 = ptArrInPolygon[k];
			pt2 = ptArrInPolygon[(k+1) % nPtCnt];
			pt3 = ptArrInPolygon[(k+2) % nPtCnt];
			averagePt.m_tX = (pt1.m_tX + pt2.m_tX + pt3.m_tX) / 3;
			averagePt.m_tY = (pt1.m_tY + pt2.m_tY + pt3.m_tY) / 3;

			if (gdem_PtIsInPolygon(averagePt, ptArrInPolygon))
			{
				// 닫긴폴리곤이 사용자경계선안에 놓이지 않으면 삭제한다.
				if (!gdem_PtIsInPolygon(averagePt, i_arrPt))
				{
					io_polygonArr.erase(i);
					nPolygonCount--;
					i--;
				}

				break;
			}
		}
	}

	return nPolygonCount;
}



/*******************************************************************************
	Function name:	gdem_GetCrossLineArrWithGrid.
	Description:	하나의 쎌과 사용자경계선의 사귐으로 하여 생성되는 사귐선들을 장악.
	Return: 		생성된 사귐선의 개수, (-1 : 사용자경계선전체가 셀안에 놓임)
	input:			i_arrPt, minX, maxX, minY, maxY
	output:			io_crossLineArr
********************************************************************************/
int  gdem_GetCrossLineArrWithGrid(CGDM_LocationArr &i_arrPt, double minX, double maxX, double minY, double maxY, CGDM_LocationPolygonArr &io_crossLineArr, bool blClose)
{
	int nCrossLineCount = 0;		// 셀과 사용자경계선의 사귐으로 생성되는 사귐선들의 개수
	CGDM_Location3D *pPt1 = NULL;	// 선분의 시작점
	CGDM_Location3D *pPt2 = NULL;	// 선분의 끝점
	CGDM_LocationArr ptArr;			// 셀과의 입구사귐점으로부터 출구사귐점까지 점목록(하나의 사귐선)
	GDM_CROSS_INFO eCrossInfo;		// 한개 선분과 셀의 사귐정보
	bool blStartPtInGrid = false;	// 사용자경계선의 첫점이 셀안에 놓이는가를 판정하는 변수


	int nPtCount = i_arrPt.size();
	int num = nPtCount;
	if(!blClose)
		num--;

	for (int i = 0; i < num; i++)
	{
		pPt1 = &i_arrPt[i];
		pPt2 = &i_arrPt[(i+1) % nPtCount];
		
		eCrossInfo = gdem_GetPtsSegmentWithGrid (pPt1, pPt2, minX, maxX, minY, maxY, ptArr);
		switch (eCrossInfo)
		{
		case CROSS_NO:
			break;
		case CROSS_INCELL:
			{
				if (i == 0)
				{
					blStartPtInGrid = true;
				}
			}
			break;
		case CROSS_ONE_START:
			{
				if (i == 0)
				{
					blStartPtInGrid = true;
				}
				if (!blClose)	// 경로오브젝트인때만 적용
				{
					ptArr.insert(*pPt1, 0);
				}
				io_crossLineArr.push_back(ptArr);
				ptArr.clear();
			}
			break;
		case CROSS_ONE_END:
			if (!blClose)	// 경로오브젝트인때만 적용
			{
				if (i == num -1)
					io_crossLineArr.push_back(ptArr);
			}
			break;
		case CROSS_TWO:
			io_crossLineArr.push_back(ptArr);
			ptArr.clear();
			break;
		case CROSS_UNKNOWN:
			break;
		}
	}

	// 사용자경계선의 첫점이 셀안에 놓이면 마지막 사귐선토막과 련결(다각형오브젝트인때만 적용)
	if (blStartPtInGrid)
	{
		if (blClose)
		{
			if (io_crossLineArr.size() == 0)	// 사용자경계선이 모두 셀안에 놓이면
			{
				io_crossLineArr.push_back(ptArr);
				return -1;
			}
			else
			{
				CGDM_LocationArr & tmpPtArr = io_crossLineArr[0];
				for (int i = 0; i < (int)tmpPtArr.size(); i++)
				{
					ptArr.push_back(tmpPtArr[i]);
				}
				io_crossLineArr.erase(0);
				io_crossLineArr.push_back(ptArr);
			}
		}
		else
		{
			if (io_crossLineArr.size() == 0)	// 사용자경계선이 모두 셀안에 놓이면
			{
				ptArr.insert(i_arrPt[0], 0);
				io_crossLineArr.push_back(ptArr);
				return -1;
			}
			else
			{
				CGDM_LocationArr & tmpPtArr = io_crossLineArr[0];
				tmpPtArr.insert(i_arrPt[0], 0);
			}
		}
	}

	nCrossLineCount = (int)io_crossLineArr.size();
	return nCrossLineCount;
}



/*******************************************************************************
	Function name:	gdem_GetPtsSegmentWithGrid.
	Description:	하나의 쎌과 선분의 사귐점을 구한다(선분의 끝점이 셀안에 놓일때 그점도 io_ptArr 에 추가한다)
	Return: 		사귐점에 대한 정보
	input:			pPt1, pPt2, minX, maxX, minY, maxY
	output:			io_ptArr
********************************************************************************/
GDM_CROSS_INFO  gdem_GetPtsSegmentWithGrid(CGDM_Location3D *pPt1, CGDM_Location3D *pPt2, double minX, double maxX, double minY, double maxY, CGDM_LocationArr &io_ptArr)
{

	GDM_CROSS_INFO eCrossInfo = CROSS_NO;	// 셀과 선분의 사귐정보.
	CGDM_LocationArr ptArr;					// 셀과 선분의 사귐점목록.

	// 선분의 직사각형경계와 셀경계가 사귀는가를 판단.
	double x1 = pPt1->m_tX;
	double y1 = pPt1->m_tY;
	double x2 = pPt2->m_tX;
	double y2 = pPt2->m_tY;
	double minLineX = MIN (pPt1->m_tX, pPt2->m_tX);
	double maxLineX = MAX (pPt1->m_tX, pPt2->m_tX);
	double minLineY = MIN (pPt1->m_tY, pPt2->m_tY);
	double maxLineY = MAX (pPt1->m_tY, pPt2->m_tY);

	if (minLineX >= minX && maxLineX <= maxX && minLineY >= minY && maxLineY <= maxY)
	{
		// 선분이 셀안에 놓임.
		ptArr.push_back(*pPt2);
		eCrossInfo = CROSS_INCELL;
	}
	else if (minLineX >= maxX || maxLineX <= minX || minLineY >= maxY || maxLineY <= minY)
	{
		// 선분이 셀밖에 놓이므로 사귐점이 하나도 없음
		eCrossInfo = CROSS_NO;
	}
	else
	{
		int nCrossCount = 0;		// 셀과 선분의 사귐점개수.
		CGDM_Location3D crossPt;	// 사귐점


		// 사귐점들을 얻는다.
		if (gdem_GetCrossPtSegmentWithAxisY (minX, minY, maxY, pPt1, pPt2, crossPt))
		{
			ptArr.push_back(crossPt);
		}
		if (gdem_GetCrossPtSegmentWithAxisY (maxX, minY, maxY, pPt1, pPt2, crossPt))
		{
			ptArr.push_back(crossPt);
		}
		if (gdem_GetCrossPtSegmentWithAxisX (minY, minX, maxX, pPt1, pPt2, crossPt))
		{
			ptArr.push_back(crossPt);
		}
		if (gdem_GetCrossPtSegmentWithAxisX (maxY, minX, maxX, pPt1, pPt2, crossPt))
		{
			ptArr.push_back(crossPt);
		}

		nCrossCount = ptArr.size();

		if (nCrossCount == 0)
		{
			eCrossInfo = CROSS_NO;
		}
		else if (nCrossCount == 1)
		{
			if (x1 > minX && x1 < maxX && y1 > minY && y1 < maxY)	// 첫점이 셀안에 놓이면
			{
				eCrossInfo = CROSS_ONE_START;
			}
			else if (x2 > minX && x2 < maxX && y2 > minY && y2 < maxY)	// 둘째점이 셀안에 놓이면
			{
				ptArr.push_back(*pPt2);
				eCrossInfo = CROSS_ONE_END;
			}
		}
		else if (nCrossCount == 2)
		{
			if (fabs(ptArr[0].m_tX - x1) > fabs(ptArr[1].m_tX -x1))
			{
				CGDM_Location3D tmpPt = ptArr[0];
				ptArr[0] = ptArr[1];
				ptArr[1] = tmpPt;
			}
			eCrossInfo = CROSS_TWO;
		}
		else
		{
			eCrossInfo = CROSS_UNKNOWN;
		}
	}

	// io_ptArr 에 ptArr 를 추가할것.
	for (int i = 0; i < (int)ptArr.size(); i++)
	{
		CGDM_Location3D *pPt = &ptArr[i];
		io_ptArr.push_back(*pPt);
	}

	return eCrossInfo;
}



/*******************************************************************************
	Function name:	gdem_GetCrossPtSegmentWithAxisY.
	Description:	선분과 x = a (y축에 평행인 선분) 와의 사귐점
	Return: 		true (사귐점 있음), false (평행 혹은 바깥이므로 사귐점 없음)
	input:			xConst, minY, maxY, pPt1, pPt2
	output:			crossPt
********************************************************************************/
bool gdem_GetCrossPtSegmentWithAxisY (double xConst, double minY, double maxY, CGDM_Location3D *pPt1, CGDM_Location3D *pPt2, CGDM_Location3D &crossPt)
{
	double x1 = pPt1->m_tX;
	double y1 = pPt1->m_tY;
	double x2 = pPt2->m_tX;
	double y2 = pPt2->m_tY;
	double minLineX = MIN (pPt1->m_tX, pPt2->m_tX);
	double maxLineX = MAX (pPt1->m_tX, pPt2->m_tX);

	// 선분이 Y축에 평행인가를 판정 
        if (fabs (x1 - x2) <= ERROR_LIMIT)
	{
		return false;
	}

	// 사귐점이 선분에 놓이는가를 판정
	if (!(xConst > minLineX && xConst < maxLineX))
	{
		return false;
	}

	// 사귐점계산
	crossPt.m_tX = xConst;
	crossPt.m_tY = ((y1 - y2) * xConst + (x1 * y2 - x2 * y1)) / (x1 - x2);

	// 사귐점이 Y축평행선분에 놓이는가를 판정
	if (!(crossPt.m_tY > minY && crossPt.m_tY < maxY))
	{
		return false;
	}

	return true;
}



/*******************************************************************************
	Function name:	gdem_GetCrossPtSegmentWithAxisX.
	Description:	선분과 y = a (x축에 평행인 선분) 와의 사귐점
	Return: 		true (사귐점 있음), false (평행 혹은 바깥이므로 사귐점 없음)
	input:			yConst, minX, maxX, pPt1, pPt2
	output:			crossPt
********************************************************************************/
bool gdem_GetCrossPtSegmentWithAxisX (double yConst, double minX, double maxX, CGDM_Location3D *pPt1, CGDM_Location3D *pPt2, CGDM_Location3D &crossPt)
{
	double x1 = pPt1->m_tX;
	double y1 = pPt1->m_tY;
	double x2 = pPt2->m_tX;
	double y2 = pPt2->m_tY;
	double minLineY = MIN (pPt1->m_tY, pPt2->m_tY);
	double maxLineY = MAX (pPt1->m_tY, pPt2->m_tY);

	// 선분이 X축에 평행인가를 판정 
        if (fabs (y1 - y2) <= ERROR_LIMIT)
	{
		return false;
	}

	// 사귐점이 선분에 놓이는가를 판정
	if (!(yConst > minLineY && yConst < maxLineY))
	{
		return false;
	}

	// 사귐점계산
	crossPt.m_tX = ((x1 - x2) * yConst + (y1 * x2 - x1 * y2)) / (y1 - y2);
	crossPt.m_tY = yConst;

	// 사귐점이 X축평행선분에 놓이는가를 판정
	if (!(crossPt.m_tX > minX && crossPt.m_tX < maxX))
	{
		return false;
	}

	return true;
}






/*******************************************************************************
	Function name:	getCrossPtsInCellBoundary.
	Description:	셀의 경계선에 놓이는 사귐점들을 오름나사방향으로 장악.
	Return: 		void
	input:			minX, maxX, minY, maxY, o_crossLineArr
	output:			io_ptArrInBoundary
********************************************************************************/
void getCrossPtsInCellBoundary(CGDM_LocationArr &io_ptArrInBoundary, double minX, double maxX, double minY, double maxY, CGDM_LocationPolygonArr &o_crossLineArr)
{
	CGDM_Location3D		pt0;	// 사귐선의 첫점
	CGDM_Location3D		pt1;	// 사귐선의 끝점
	CGDM_Location3D		cellPt1 (minX, minY, 0);
	CGDM_Location3D		cellPt2 (minX, maxY, 0);
	CGDM_Location3D		cellPt3 (maxX, maxY, 0);
	CGDM_Location3D		cellPt4 (maxX, minY, 0);
	CGDM_LocationArr	ptArrInBorder1;
	CGDM_LocationArr	ptArrInBorder2;
	CGDM_LocationArr	ptArrInBorder3;
	CGDM_LocationArr	ptArrInBorder4;
	CGDM_LocationPolygonArr polyArrInBorder;
	int nCrossLineCount = (int)o_crossLineArr.size();

	polyArrInBorder.push_back(ptArrInBorder1);
	polyArrInBorder.push_back(ptArrInBorder2);
	polyArrInBorder.push_back(ptArrInBorder3);
	polyArrInBorder.push_back(ptArrInBorder4);
//			_________________
//			|		2		|
//			|				|
//			|				|
//			|1			   3|
//			|				|
//			|				|
//			|				|
//			|		4		|
//			-----------------

	// 사귐선들의 첫점과 끝점을 판정하여 해당한 목록에 추가한다.
	for (int i = 0; i < nCrossLineCount; i++)
	{
		// 사귐선의 첫번째점판정
		pt0 = o_crossLineArr[i][0];
		insertPtToBoundary (pt0, polyArrInBorder, minX, maxX, minY, maxY);

		// 사귐선의 두번째점판정
		pt1 = o_crossLineArr[i].getLast();
		insertPtToBoundary (pt1, polyArrInBorder, minX, maxX, minY, maxY);
	}

	// 셀의 마디점까지 포함한 전체 목록을 얻어낸다.
	polyArrInBorder[0].insert(cellPt1);
	polyArrInBorder[1].insert(cellPt2);
	polyArrInBorder[2].insert(cellPt3);
	polyArrInBorder[3].insert(cellPt4);
	for (int i = 0; i < 4; i++)
	{
		CGDM_LocationArr &ptArrInBorder = polyArrInBorder[i];
		int nPtCount = (int)ptArrInBorder.size();

		for (int k = 0; k < nPtCount; k++)
		{
			io_ptArrInBoundary.push_back (ptArrInBorder[k]);
		}
	}
}


/*******************************************************************************
	Function name:	insertPtToBoundary.
	Description:	사귐점을 사귐점목록에 추가하되 쎌의 오름나사방향으로 순서대로 추가.
	Return: 		void
	input:			crossPt, minX, maxX, minY, maxY
	output:			io_polyArrInBorder
********************************************************************************/
void insertPtToBoundary (CGDM_Location3D &crossPt, CGDM_LocationPolygonArr &io_polyArrInBorder, double minX, double maxX, double minY, double maxY)
{
	int nSegmentNumber;	// 셀의 경계선의 번호
	int nPtCount;		// 경계선에 놓이는 사귐점들의 개수

//			_________________
//			|		2		|
//			|				|
//			|				|
//			|1			   3|
//			|				|
//			|				|
//			|				|
//			|		4		|
//			-----------------

	// 사귐점이 놓이는 경계선의 번호를 찾는다.
	nSegmentNumber = findPtInBoundary (crossPt, minX, maxX, minY, maxY);

	// 사귐점이 경계선에 놓이면 오름나사방향으로 배치한다.
	if (nSegmentNumber != 0)
	{
		int k = 0;	// 경계선에 놓이는 사귐점의 번호(크기순서)
		CGDM_LocationArr & ptArr = io_polyArrInBorder[nSegmentNumber-1];
		nPtCount = (int)ptArr.size();
		for (k = 0; k < nPtCount; k++)
		{
			if (nSegmentNumber == 1)
			{
				if (ptArr[k].m_tY > crossPt.m_tY)
					break;
				else
					continue;
			}
			else if (nSegmentNumber == 2)
			{
				if (ptArr[k].m_tX > crossPt.m_tX)
					break;
				else
					continue;
			}
			else if (nSegmentNumber == 3)
			{
				if (ptArr[k].m_tY < crossPt.m_tY)
					break;
				else
					continue;
			}
			else if (nSegmentNumber == 4)
			{
				if (ptArr[k].m_tX < crossPt.m_tX)
					break;
				else
					continue;
			}
		}

		ptArr.insert (crossPt, k);
	}
}



/*******************************************************************************
	Function name:	findPtInBoundary.
	Description:	사귐점이 쎌의 어느 경계선에 놓이는가를 판정한다.
	Return: 		int :	0 : 셀의 마디점과 일치하므로 무시한다), 
							1, 2, 3, 4 : 각각 셀의 1, 2, 3, 4 선분에 놓임)
	input:			crossPt, minX, maxX, minY, maxY
********************************************************************************/
int findPtInBoundary (CGDM_Location3D &crossPt, double minX, double maxX, double minY, double maxY)
{
//			_________________
//			|		2		|
//			|				|
//			|				|
//			|1			   3|
//			|				|
//			|				|
//			|				|
//			|		4		|
//			-----------------


	CGDM_Location3D		cellPt1 (minX, minY, 0);
	CGDM_Location3D		cellPt2 (minX, maxY, 0);
	CGDM_Location3D		cellPt3 (maxX, maxY, 0);
	CGDM_Location3D		cellPt4 (maxX, minY, 0);

	if (crossPt == cellPt1 || crossPt == cellPt2 || crossPt == cellPt3 || crossPt == cellPt4)
	{
		return 0;
	}

	if (crossPt.m_tX == minX)
	{
		return 1;
	}
	else if (crossPt.m_tY == maxY)
	{
		return 2;
	}
	else if (crossPt.m_tX == maxX)
	{
		return 3;
	}
	else if (crossPt.m_tY == minY)
	{
		return 4;
	}

	return 0;
}





/*******************************************************************************
	Function name:	gdem_GetClosedPolygonArr.
	Description:	셀경계의 사귐점들과 사귐선들로부터 닫긴 폴리곤들을 얻어낸다.
	Return: 		void
	input:			ptArrInBoundary, crossLineArr
	output:			io_polygonArr
********************************************************************************/
void gdem_GetClosedPolygonArr (CGDM_LocationArr &ptArrInBoundary, CGDM_LocationPolygonArr &crossLineArr, CGDM_LocationPolygonArr &io_polygonArr)
{
	CGDM_LocationArr	ptArrInPolygon;		// 얻으려는 하나하나의 닫긴폴리곤.
	CGDM_SegmentArr		segmentArr;			// 경계선분 목록

	int		nPtCount;		// 사귐점들의 개수
	int		nSegmentCount;	// 셀경계선에 사귐점들로 하여 생기는 분개수(사귐점개수와 같음)

	GDM_SEGMENT		segment;		// 경계선분토막
	int		nCrossLineNumber  = 0;	// 사귐점이 포함되는 사귐선번호.
	int		nBorderLineNumber = 0;	// 사귐점이 포함되는 경계선번호.
	CGDM_Location3D		compPt;		// 현재 얻어진 경로의 끝점(경계선분의 점)이며 다음경로를 나가는데서 비교하는점
	bool	bNormal		= true;		// 선택된 사귐선의 방향 ( true : 순서대로, false : 거꾸로 )



	// 셀경계의 사귐점들로부터 선분토막배렬을 생성한다.
	nPtCount = (int) ptArrInBoundary.size();
	for (int i = 0; i < nPtCount; i++)
	{
		segment.cPt1 = ptArrInBoundary[i];
		segment.cPt2 = ptArrInBoundary[(i+1) % nPtCount];
		segmentArr.push_back(segment);
	}	


	nSegmentCount = (int) segmentArr.size();
	while (nSegmentCount > 0)
	{
		ptArrInPolygon.clear();		// 생성되는 닫긴폴리곤 초기화

		// 경계선의 시작선분은 생성되는 닫긴폴리곤에 무조건 추가된다.
		ptArrInPolygon.push_back(segmentArr[0].cPt1);
		ptArrInPolygon.push_back(segmentArr[0].cPt2);

		// 다음 닫긴경로가 생성될때까지 나머지 선분토막들을 순환한다.
		compPt  = segmentArr[0].cPt2;
		if (nSegmentCount > 1)
		{
			for (int i = 1; i < nSegmentCount; i++)
			{
				if (isEqualPtWithCrossLine (compPt, crossLineArr, nCrossLineNumber, bNormal))	// 다음경로가 사귐선이면
				{
					CGDM_LocationArr &crossLine = crossLineArr[nCrossLineNumber];	// 선택된 사귐선
					int nCrossPtCount = crossLine.size();				
					for (int k = 1; k < nCrossPtCount; k++)
					{
						if (bNormal)	// 사귐선이 첫점부터 시작하면
							ptArrInPolygon.push_back(crossLine[k]);
						else			// 사귐선이 끝점부터 시작하면
							ptArrInPolygon.push_back(crossLine[(nCrossPtCount - 1) - k]);
					}

					// 비교점을 찾는다.
					if (bNormal)
						compPt = crossLine[nCrossPtCount - 1];
					else
						compPt = crossLine[0];

					if (isEqualPtWithBorderLine (compPt, segmentArr, nBorderLineNumber))	// 다음경로가 경계선분이면
					{
						if (nBorderLineNumber == 0)
						{
							break;	// 닫긴곡선 완성되는 시점
						}
						else
						{
							ptArrInPolygon.push_back(segmentArr[nBorderLineNumber].cPt2);
							compPt = segmentArr[nBorderLineNumber].cPt2;

							// 선분삭제 및 배렬크기 감소.
							segmentArr.erase(nBorderLineNumber);
							nSegmentCount--;
							i = nBorderLineNumber - 1;

							if (i == (nSegmentCount - 1))	// 마지막경계선분이면
							{
								if (isEqualPtWithCrossLine (compPt, crossLineArr, nCrossLineNumber, bNormal))	// 다음경로는 무조건 사귐선이여야 한다.
								{
									CGDM_LocationArr &crossLine = crossLineArr[nCrossLineNumber];	// 선택된 사귐선
									int nCrossPtCount = crossLine.size();				
									for (int k = 1; k < nCrossPtCount; k++)
									{
										if (bNormal)	// 사귐선이 첫점부터 시작하면
											ptArrInPolygon.push_back(crossLine[k]);
										else			// 사귐선이 끝점부터 시작하면
											ptArrInPolygon.push_back(crossLine[(nCrossPtCount - 1) - k]);
									}
								}
								else
								{
									// Error
								}
							}
						}
					}
					else	// 경계선분이 아니면 오유
					{
						// Error
					}
				}
				else	// 다음경로가 경계선분이면
				{
					ptArrInPolygon.push_back(segmentArr[i].cPt2);
					compPt = segmentArr[i].cPt2;

					segmentArr.erase(i);
					nSegmentCount--;
					i = i - 1;

					if (i == (nSegmentCount - 1))	// 마지막경계선분이면
					{
						if (isEqualPtWithCrossLine (compPt, crossLineArr, nCrossLineNumber, bNormal))	// 다음경로는 무조건 사귐선이여야 한다.
						{
							CGDM_LocationArr &crossLine = crossLineArr[nCrossLineNumber];	// 선택된 사귐선
							int nCrossPtCount = crossLine.size();				
							for (int k = 1; k < nCrossPtCount; k++)
							{
								if (bNormal)	// 사귐선이 첫점부터 시작하면
									ptArrInPolygon.push_back(crossLine[k]);
								else			// 사귐선이 끝점부터 시작하면
									ptArrInPolygon.push_back(crossLine[(nCrossPtCount - 1) - k]);
							}
						}
						else
						{
							// Error
						}
					}
				}
			}// for //
		}
		else
		{
			if (isEqualPtWithCrossLine (compPt, crossLineArr, nCrossLineNumber, bNormal))	// 다음경로가 사귐선이면
			{
				CGDM_LocationArr &crossLine = crossLineArr[nCrossLineNumber];	// 선택된 사귐선
				int nCrossPtCount = crossLine.size();				
				for (int k = 1; k < nCrossPtCount; k++)
				{
					if (bNormal)	// 사귐선이 첫점부터 시작하면
						ptArrInPolygon.push_back(crossLine[k]);
					else			// 사귐선이 끝점부터 시작하면
						ptArrInPolygon.push_back(crossLine[(nCrossPtCount - 1) - k]);
				}
			}
		}

		// 마지막점은 첫점과 일치하므로 삭제한다.
		ptArrInPolygon.erase(ptArrInPolygon.size() - 1);

		segmentArr.erase(0);	// 첫번째 경계선분을 삭제.
		nSegmentCount = (int) segmentArr.size();
		io_polygonArr.push_back(ptArrInPolygon);	// 닫긴폴리곤을 추가함.
	}// while //
}


/*******************************************************************************
	Function name:	isEqualPtWithCrossLine.
	Description:	비교점이 사귐선들의 첫점, 혹은 마지막점과 일치하는 사귐선의 번호와 방향을 구한다.
	Return: 		bool (일치하는 점이 있으면 true, 없으면 false)
	input:			compPt, crossLineArr
	output:			nCrossLineNumber, bNormal
********************************************************************************/
bool isEqualPtWithCrossLine (CGDM_Location3D &compPt, CGDM_LocationPolygonArr &crossLineArr, int &nCrossLineNumber, bool &bNormal)
{
	CGDM_Location3D	startPt;	// 사귐선의 첫점
	CGDM_Location3D endPt;		// 사귐선의 끝점

	int nCrossLineCount = (int)crossLineArr.size();
	for (int i = 0; i < nCrossLineCount; i++)
	{
		CGDM_LocationArr &crossLine = crossLineArr[i];
		startPt = crossLine[0];
		endPt   = crossLine[crossLine.size()-1];
		if (startPt == compPt)
		{
			nCrossLineNumber = i;
			bNormal = true;
			return true;
		}
		else if (endPt == compPt)
		{
			nCrossLineNumber = i;
			bNormal = false;
			return true;
		}
	}

	return false;
}


/*******************************************************************************
	Function name:	isEqualPtWithBorderLine.
	Description:	비교점이 경계선분의 첫점과 일치하는 경계선분의 번호를 구한다.
	Return: 		bool (일치하는 경계선분이 있으면 true, 없으면 false)
	input:			compPt, segmentArr
	output:			nBorderLineNumber
********************************************************************************/
bool isEqualPtWithBorderLine (CGDM_Location3D &compPt, CGDM_SegmentArr &segmentArr, int &nBorderLineNumber)
{
	int nSegmentCount = (int)segmentArr.size();
	for (int i = 0; i < nSegmentCount; i++)
	{
		if (segmentArr[i].cPt1 == compPt)
		{
			nBorderLineNumber = i;
			return true;
		}
	}

	return false;
}

//2011 3 14 added by ugi
void  gdem_GetLineArrInRectangle(CGDM_LocationArr &i_arrPt, double minX, double maxX, double minY, double maxY, 
								 CGDM_LocationPolygonArr &o_LineArr)
{
	int nCrossLineCount = 0;		// 셀과 사용자경계선의 사귐으로 생성되는 사귐선들의 개수
	CGDM_Location3D pPt1 ;	       // 선분의 시작점
	CGDM_Location3D pPt2 ;	       // 선분의 끝점
	CGDM_LocationArr crossPtArr; // 셀과의 입구사귐점으로부터 출구사귐점까지 점목록(하나의 사귐선)
	CGDM_LocationArr line;		  // 
	GDM_CROSS_INFO eCrossInfo;		// 한개 선분과 셀의 사귐정보

	int nPtCount = i_arrPt.size();

	for (int i = 0; i < nPtCount-1; i++)
	{
		pPt1 = i_arrPt[i];
		pPt2 = i_arrPt[i+1];

		crossPtArr.clear();
		eCrossInfo = gdem_GetPtsSegmentWithGrid (&pPt1, &pPt2, minX, maxX, minY, maxY, crossPtArr);
		switch (eCrossInfo)
		{
		case CROSS_NO:
			break;
		case CROSS_INCELL:
			if(i==nPtCount-2)
			{
				line.push_back(pPt1);
				line.push_back(pPt2);
				o_LineArr.push_back(line);
			}
			else
				line.push_back(pPt1);
			break;
		case CROSS_ONE_START:
			line.push_back(pPt1);
			line.push_back(crossPtArr[0]);
			o_LineArr.push_back(line);
			line.clear();
			break;
		case CROSS_ONE_END:
			if(i==nPtCount-2)
			{
				line.push_back(crossPtArr[0]);
				line.push_back(pPt2);
				o_LineArr.push_back(line);
			}
			else
				line.push_back(crossPtArr[0]);
			break;
		case CROSS_TWO:
			line.push_back(pPt1);
			line.push_back(crossPtArr[0]);
			o_LineArr.push_back(line);
			line.clear();

			line.push_back(crossPtArr[0]);
			line.push_back(crossPtArr[1]);
			o_LineArr.push_back(line);
			line.clear();
			break;
		case CROSS_UNKNOWN:
			break;
		}
	}
}
//end
