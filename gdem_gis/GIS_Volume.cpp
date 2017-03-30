#include "GIS_funcs.h"

typedef gdm_array<CGDM_LocationArr>	CGDM_LocationPolygonArr;	// Ÿ�ϰ� ����ڰ�輱�� ��ж� ����� ��������� �������

#undef MAX
#define MAX(a,b)	((a) > (b) ? (a) : (b))
#undef MIN
#define MIN(a,b)	((a) < (b) ? (a) : (b))

#define ERROR_LIMIT 0.000000000001

typedef enum
{
	CROSS_NO,			// ������� �ϳ��� ����
	CROSS_INCELL,		// ������ ���ȿ� ����
	CROSS_ONE_START,	// ������� �ϳ��̰� �������� ���ȿ� ����
	CROSS_ONE_END,		// ������� �ϳ��̰� ������ ���ȿ� ����
	CROSS_TWO,			// ������� �ΰ�
	CROSS_UNKNOWN
} GDM_CROSS_INFO;

typedef struct
{
	CGDM_Location3D cPt1;
	CGDM_Location3D cPt2;
} GDM_SEGMENT;

typedef gdm_array<GDM_SEGMENT>	CGDM_SegmentArr;	// ������ �������




GDM_CROSS_INFO  gdem_GetPtsSegmentWithGrid (CGDM_Location3D *pPt1, CGDM_Location3D *pPt2, double minX, double maxX, double minY, double maxY, CGDM_LocationArr &io_ptArr);


void getCrossPtsInCellBoundary(CGDM_LocationArr &io_ptArrInBoundary, double minX, double maxX, double minY, double maxY, CGDM_LocationPolygonArr &o_crossLineArr);
void insertPtToBoundary (CGDM_Location3D &crossPt, CGDM_LocationPolygonArr &io_polyArrInBorder, double minX, double maxX, double minY, double maxY);
int findPtInBoundary (CGDM_Location3D &crossPt, double minX, double maxX, double minY, double maxY);


void gdem_GetClosedPolygonArr (CGDM_LocationArr &ptArrInBoundary, CGDM_LocationPolygonArr &crossLineArr, CGDM_LocationPolygonArr &io_polygonArr);
bool isEqualPtWithCrossLine (CGDM_Location3D &compPt, CGDM_LocationPolygonArr &crossLineArr, int &nCrossLineNumber, bool &bNormal);
bool isEqualPtWithBorderLine (CGDM_Location3D &compPt, CGDM_SegmentArr &segmentArr, int &nBorderLineNumber);



/*******************************************************************************
	Function name:	gdem_GetPolygonArrWithGrid.
	Description:	�ϳ��� ��� ����ڰ�輱�� ������� �Ͽ� �����Ǵ� �ݱ� ��������� ���.
	Return: 		int : ������ �ݱ��������� ����
	input:			i_arrPt, minX, maxX, minY, maxY
	in-output:		io_polygonArr
	** : ���� �� �Լ��� ���� ����ڰ�輱�� ����� ������ ȣ���
********************************************************************************/
int  gdem_GetPolygonArrWithGrid(CGDM_LocationArr &i_arrPt, double minX, double maxX, double minY, double maxY, CGDM_LocationPolygonArr &io_polygonArr)
{
	CGDM_LocationPolygonArr crossLineArr;		// ���� ����ڰ�輱�� ������� �Ͽ� �����Ǵ� ��Ѽ����� ���
	CGDM_LocationArr		ptArrInBoundary;	// ���� 4���� ����ڰ�輱���� �������� �̷������ �����(�ּ������κ��� �����������)
	CGDM_Location3D			averagePt;		// �ݱ���������� ��.
	CGDM_Location3D			pt1, pt2, pt3;	// �ݱ������������� ����Ǵ� ����.





	// ���� ���� ����ڰ�輱���� ��Ѽ����� ���Ѵ�.
	int nCrossLineCount = gdem_GetCrossLineArrWithGrid(i_arrPt, minX, maxX, minY, maxY, crossLineArr);
	if (nCrossLineCount == 0)
	{
		// ���� ����ڰ�輱�ȿ� ���԰��踦 ������.
		return 0;
	}
	else if (nCrossLineCount == -1)	// ����ڰ�輱��ü�� ���ȿ� ���̸�.
	{
		io_polygonArr.push_back(i_arrPt);
		return 1;
	}

	// ��������������� ���������� ���.(���⿡�� ���� �������� ���Ե�)
	getCrossPtsInCellBoundary(ptArrInBoundary, minX, maxX, minY, maxY, crossLineArr);

	// ��Ѽ���� �������κ��� �����Ǵ� ��� �ݱ���������� ����.
	gdem_GetClosedPolygonArr (ptArrInBoundary, crossLineArr, io_polygonArr);

	// ����ڰ�輱�ۿ� ���̴� �ݱ���������� �����Ѵ�.(�� ���꿡 ������ �����︸ ����)
	int nPolygonCount = (int) io_polygonArr.size();
	for (int i = 0; i < nPolygonCount; i++)
	{
		CGDM_LocationArr &ptArrInPolygon = io_polygonArr[i];	// �ϳ��� �ݱ�������.
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
				// �ݱ��������� ����ڰ�輱�ȿ� ������ ������ �����Ѵ�.
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
	Description:	�ϳ��� ��� ����ڰ�輱�� ������� �Ͽ� �����Ǵ� ��Ѽ����� ���.
	Return: 		������ ��Ѽ��� ����, (-1 : ����ڰ�輱��ü�� ���ȿ� ����)
	input:			i_arrPt, minX, maxX, minY, maxY
	output:			io_crossLineArr
********************************************************************************/
int  gdem_GetCrossLineArrWithGrid(CGDM_LocationArr &i_arrPt, double minX, double maxX, double minY, double maxY, CGDM_LocationPolygonArr &io_crossLineArr, bool blClose)
{
	int nCrossLineCount = 0;		// ���� ����ڰ�輱�� ������� �����Ǵ� ��Ѽ����� ����
	CGDM_Location3D *pPt1 = NULL;	// ������ ������
	CGDM_Location3D *pPt2 = NULL;	// ������ ����
	CGDM_LocationArr ptArr;			// ������ �Ա���������κ��� �ⱸ��������� �����(�ϳ��� ��Ѽ�)
	GDM_CROSS_INFO eCrossInfo;		// �Ѱ� ���а� ���� �������
	bool blStartPtInGrid = false;	// ����ڰ�輱�� ù���� ���ȿ� ���̴°��� �����ϴ� ����


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
				if (!blClose)	// ��ο�����Ʈ�ζ��� ����
				{
					ptArr.insert(*pPt1, 0);
				}
				io_crossLineArr.push_back(ptArr);
				ptArr.clear();
			}
			break;
		case CROSS_ONE_END:
			if (!blClose)	// ��ο�����Ʈ�ζ��� ����
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

	// ����ڰ�輱�� ù���� ���ȿ� ���̸� ������ ��Ѽ��丷�� �ð�(�ٰ���������Ʈ�ζ��� ����)
	if (blStartPtInGrid)
	{
		if (blClose)
		{
			if (io_crossLineArr.size() == 0)	// ����ڰ�輱�� ��� ���ȿ� ���̸�
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
			if (io_crossLineArr.size() == 0)	// ����ڰ�輱�� ��� ���ȿ� ���̸�
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
	Description:	�ϳ��� ��� ������ ������� ���Ѵ�(������ ������ ���ȿ� ���϶� ������ io_ptArr �� �߰��Ѵ�)
	Return: 		������� ���� ����
	input:			pPt1, pPt2, minX, maxX, minY, maxY
	output:			io_ptArr
********************************************************************************/
GDM_CROSS_INFO  gdem_GetPtsSegmentWithGrid(CGDM_Location3D *pPt1, CGDM_Location3D *pPt2, double minX, double maxX, double minY, double maxY, CGDM_LocationArr &io_ptArr)
{

	GDM_CROSS_INFO eCrossInfo = CROSS_NO;	// ���� ������ �������.
	CGDM_LocationArr ptArr;					// ���� ������ ��������.

	// ������ ���簢������ ����谡 ��ʹ°��� �Ǵ�.
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
		// ������ ���ȿ� ����.
		ptArr.push_back(*pPt2);
		eCrossInfo = CROSS_INCELL;
	}
	else if (minLineX >= maxX || maxLineX <= minX || minLineY >= maxY || maxLineY <= minY)
	{
		// ������ ���ۿ� ���̹Ƿ� ������� �ϳ��� ����
		eCrossInfo = CROSS_NO;
	}
	else
	{
		int nCrossCount = 0;		// ���� ������ ���������.
		CGDM_Location3D crossPt;	// �����


		// ��������� ��´�.
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
			if (x1 > minX && x1 < maxX && y1 > minY && y1 < maxY)	// ù���� ���ȿ� ���̸�
			{
				eCrossInfo = CROSS_ONE_START;
			}
			else if (x2 > minX && x2 < maxX && y2 > minY && y2 < maxY)	// ��°���� ���ȿ� ���̸�
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

	// io_ptArr �� ptArr �� �߰��Ұ�.
	for (int i = 0; i < (int)ptArr.size(); i++)
	{
		CGDM_Location3D *pPt = &ptArr[i];
		io_ptArr.push_back(*pPt);
	}

	return eCrossInfo;
}



/*******************************************************************************
	Function name:	gdem_GetCrossPtSegmentWithAxisY.
	Description:	���а� x = a (y�࿡ ������ ����) ���� �����
	Return: 		true (����� ����), false (���� Ȥ�� �ٱ��̹Ƿ� ����� ����)
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

	// ������ Y�࿡ �����ΰ��� ���� 
        if (fabs (x1 - x2) <= ERROR_LIMIT)
	{
		return false;
	}

	// ������� ���п� ���̴°��� ����
	if (!(xConst > minLineX && xConst < maxLineX))
	{
		return false;
	}

	// ��������
	crossPt.m_tX = xConst;
	crossPt.m_tY = ((y1 - y2) * xConst + (x1 * y2 - x2 * y1)) / (x1 - x2);

	// ������� Y�����༱�п� ���̴°��� ����
	if (!(crossPt.m_tY > minY && crossPt.m_tY < maxY))
	{
		return false;
	}

	return true;
}



/*******************************************************************************
	Function name:	gdem_GetCrossPtSegmentWithAxisX.
	Description:	���а� y = a (x�࿡ ������ ����) ���� �����
	Return: 		true (����� ����), false (���� Ȥ�� �ٱ��̹Ƿ� ����� ����)
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

	// ������ X�࿡ �����ΰ��� ���� 
        if (fabs (y1 - y2) <= ERROR_LIMIT)
	{
		return false;
	}

	// ������� ���п� ���̴°��� ����
	if (!(yConst > minLineY && yConst < maxLineY))
	{
		return false;
	}

	// ��������
	crossPt.m_tX = ((x1 - x2) * yConst + (y1 * x2 - x1 * y2)) / (y1 - y2);
	crossPt.m_tY = yConst;

	// ������� X�����༱�п� ���̴°��� ����
	if (!(crossPt.m_tX > minX && crossPt.m_tX < maxX))
	{
		return false;
	}

	return true;
}






/*******************************************************************************
	Function name:	getCrossPtsInCellBoundary.
	Description:	���� ��輱�� ���̴� ��������� ��������������� ���.
	Return: 		void
	input:			minX, maxX, minY, maxY, o_crossLineArr
	output:			io_ptArrInBoundary
********************************************************************************/
void getCrossPtsInCellBoundary(CGDM_LocationArr &io_ptArrInBoundary, double minX, double maxX, double minY, double maxY, CGDM_LocationPolygonArr &o_crossLineArr)
{
	CGDM_Location3D		pt0;	// ��Ѽ��� ù��
	CGDM_Location3D		pt1;	// ��Ѽ��� ����
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

	// ��Ѽ����� ù���� ������ �����Ͽ� �ش��� ��Ͽ� �߰��Ѵ�.
	for (int i = 0; i < nCrossLineCount; i++)
	{
		// ��Ѽ��� ù��°������
		pt0 = o_crossLineArr[i][0];
		insertPtToBoundary (pt0, polyArrInBorder, minX, maxX, minY, maxY);

		// ��Ѽ��� �ι�°������
		pt1 = o_crossLineArr[i].getLast();
		insertPtToBoundary (pt1, polyArrInBorder, minX, maxX, minY, maxY);
	}

	// ���� ���������� ������ ��ü ����� ����.
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
	Description:	������� �������Ͽ� �߰��ϵ� ���� ��������������� ������� �߰�.
	Return: 		void
	input:			crossPt, minX, maxX, minY, maxY
	output:			io_polyArrInBorder
********************************************************************************/
void insertPtToBoundary (CGDM_Location3D &crossPt, CGDM_LocationPolygonArr &io_polyArrInBorder, double minX, double maxX, double minY, double maxY)
{
	int nSegmentNumber;	// ���� ��輱�� ��ȣ
	int nPtCount;		// ��輱�� ���̴� ��������� ����

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

	// ������� ���̴� ��輱�� ��ȣ�� ã�´�.
	nSegmentNumber = findPtInBoundary (crossPt, minX, maxX, minY, maxY);

	// ������� ��輱�� ���̸� ��������������� ��ġ�Ѵ�.
	if (nSegmentNumber != 0)
	{
		int k = 0;	// ��輱�� ���̴� ������� ��ȣ(ũ�����)
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
	Description:	������� ���� ��� ��輱�� ���̴°��� �����Ѵ�.
	Return: 		int :	0 : ���� �������� ��ġ�ϹǷ� �����Ѵ�), 
							1, 2, 3, 4 : ���� ���� 1, 2, 3, 4 ���п� ����)
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
	Description:	������� �������� ��Ѽ���κ��� �ݱ� ��������� ����.
	Return: 		void
	input:			ptArrInBoundary, crossLineArr
	output:			io_polygonArr
********************************************************************************/
void gdem_GetClosedPolygonArr (CGDM_LocationArr &ptArrInBoundary, CGDM_LocationPolygonArr &crossLineArr, CGDM_LocationPolygonArr &io_polygonArr)
{
	CGDM_LocationArr	ptArrInPolygon;		// �������� �ϳ��ϳ��� �ݱ�������.
	CGDM_SegmentArr		segmentArr;			// ��輱�� ���

	int		nPtCount;		// ��������� ����
	int		nSegmentCount;	// ����輱�� �������� �Ͽ� ����� �а���(����������� ����)

	GDM_SEGMENT		segment;		// ��輱���丷
	int		nCrossLineNumber  = 0;	// ������� ���ԵǴ� ��Ѽ���ȣ.
	int		nBorderLineNumber = 0;	// ������� ���ԵǴ� ��輱��ȣ.
	CGDM_Location3D		compPt;		// ���� ����� ����� ����(��輱���� ��)�̸� ������θ� �����µ��� ���ϴ���
	bool	bNormal		= true;		// ���õ� ��Ѽ��� ���� ( true : �������, false : �Ųٷ� )



	// ������� �������κ��� �����丷����� �����Ѵ�.
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
		ptArrInPolygon.clear();		// �����Ǵ� �ݱ������� �ʱ�ȭ

		// ��輱�� ���ۼ����� �����Ǵ� �ݱ������￡ ������ �߰��ȴ�.
		ptArrInPolygon.push_back(segmentArr[0].cPt1);
		ptArrInPolygon.push_back(segmentArr[0].cPt2);

		// ���� �ݱ��ΰ� �����ɶ����� ������ �����丷���� ��ȯ�Ѵ�.
		compPt  = segmentArr[0].cPt2;
		if (nSegmentCount > 1)
		{
			for (int i = 1; i < nSegmentCount; i++)
			{
				if (isEqualPtWithCrossLine (compPt, crossLineArr, nCrossLineNumber, bNormal))	// ������ΰ� ��Ѽ��̸�
				{
					CGDM_LocationArr &crossLine = crossLineArr[nCrossLineNumber];	// ���õ� ��Ѽ�
					int nCrossPtCount = crossLine.size();				
					for (int k = 1; k < nCrossPtCount; k++)
					{
						if (bNormal)	// ��Ѽ��� ù������ �����ϸ�
							ptArrInPolygon.push_back(crossLine[k]);
						else			// ��Ѽ��� �������� �����ϸ�
							ptArrInPolygon.push_back(crossLine[(nCrossPtCount - 1) - k]);
					}

					// ������ ã�´�.
					if (bNormal)
						compPt = crossLine[nCrossPtCount - 1];
					else
						compPt = crossLine[0];

					if (isEqualPtWithBorderLine (compPt, segmentArr, nBorderLineNumber))	// ������ΰ� ��輱���̸�
					{
						if (nBorderLineNumber == 0)
						{
							break;	// �ݱ� �ϼ��Ǵ� ����
						}
						else
						{
							ptArrInPolygon.push_back(segmentArr[nBorderLineNumber].cPt2);
							compPt = segmentArr[nBorderLineNumber].cPt2;

							// ���л��� �� ���ũ�� ����.
							segmentArr.erase(nBorderLineNumber);
							nSegmentCount--;
							i = nBorderLineNumber - 1;

							if (i == (nSegmentCount - 1))	// ��������輱���̸�
							{
								if (isEqualPtWithCrossLine (compPt, crossLineArr, nCrossLineNumber, bNormal))	// ������δ� ������ ��Ѽ��̿��� �Ѵ�.
								{
									CGDM_LocationArr &crossLine = crossLineArr[nCrossLineNumber];	// ���õ� ��Ѽ�
									int nCrossPtCount = crossLine.size();				
									for (int k = 1; k < nCrossPtCount; k++)
									{
										if (bNormal)	// ��Ѽ��� ù������ �����ϸ�
											ptArrInPolygon.push_back(crossLine[k]);
										else			// ��Ѽ��� �������� �����ϸ�
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
					else	// ��輱���� �ƴϸ� ����
					{
						// Error
					}
				}
				else	// ������ΰ� ��輱���̸�
				{
					ptArrInPolygon.push_back(segmentArr[i].cPt2);
					compPt = segmentArr[i].cPt2;

					segmentArr.erase(i);
					nSegmentCount--;
					i = i - 1;

					if (i == (nSegmentCount - 1))	// ��������輱���̸�
					{
						if (isEqualPtWithCrossLine (compPt, crossLineArr, nCrossLineNumber, bNormal))	// ������δ� ������ ��Ѽ��̿��� �Ѵ�.
						{
							CGDM_LocationArr &crossLine = crossLineArr[nCrossLineNumber];	// ���õ� ��Ѽ�
							int nCrossPtCount = crossLine.size();				
							for (int k = 1; k < nCrossPtCount; k++)
							{
								if (bNormal)	// ��Ѽ��� ù������ �����ϸ�
									ptArrInPolygon.push_back(crossLine[k]);
								else			// ��Ѽ��� �������� �����ϸ�
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
			if (isEqualPtWithCrossLine (compPt, crossLineArr, nCrossLineNumber, bNormal))	// ������ΰ� ��Ѽ��̸�
			{
				CGDM_LocationArr &crossLine = crossLineArr[nCrossLineNumber];	// ���õ� ��Ѽ�
				int nCrossPtCount = crossLine.size();				
				for (int k = 1; k < nCrossPtCount; k++)
				{
					if (bNormal)	// ��Ѽ��� ù������ �����ϸ�
						ptArrInPolygon.push_back(crossLine[k]);
					else			// ��Ѽ��� �������� �����ϸ�
						ptArrInPolygon.push_back(crossLine[(nCrossPtCount - 1) - k]);
				}
			}
		}

		// ���������� ù���� ��ġ�ϹǷ� �����Ѵ�.
		ptArrInPolygon.erase(ptArrInPolygon.size() - 1);

		segmentArr.erase(0);	// ù��° ��輱���� ����.
		nSegmentCount = (int) segmentArr.size();
		io_polygonArr.push_back(ptArrInPolygon);	// �ݱ��������� �߰���.
	}// while //
}


/*******************************************************************************
	Function name:	isEqualPtWithCrossLine.
	Description:	������ ��Ѽ����� ù��, Ȥ�� ���������� ��ġ�ϴ� ��Ѽ��� ��ȣ�� ������ ���Ѵ�.
	Return: 		bool (��ġ�ϴ� ���� ������ true, ������ false)
	input:			compPt, crossLineArr
	output:			nCrossLineNumber, bNormal
********************************************************************************/
bool isEqualPtWithCrossLine (CGDM_Location3D &compPt, CGDM_LocationPolygonArr &crossLineArr, int &nCrossLineNumber, bool &bNormal)
{
	CGDM_Location3D	startPt;	// ��Ѽ��� ù��
	CGDM_Location3D endPt;		// ��Ѽ��� ����

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
	Description:	������ ��輱���� ù���� ��ġ�ϴ� ��輱���� ��ȣ�� ���Ѵ�.
	Return: 		bool (��ġ�ϴ� ��輱���� ������ true, ������ false)
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
	int nCrossLineCount = 0;		// ���� ����ڰ�輱�� ������� �����Ǵ� ��Ѽ����� ����
	CGDM_Location3D pPt1 ;	       // ������ ������
	CGDM_Location3D pPt2 ;	       // ������ ����
	CGDM_LocationArr crossPtArr; // ������ �Ա���������κ��� �ⱸ��������� �����(�ϳ��� ��Ѽ�)
	CGDM_LocationArr line;		  // 
	GDM_CROSS_INFO eCrossInfo;		// �Ѱ� ���а� ���� �������

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
