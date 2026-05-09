#include "GIS_funcs.h"

typedef qglobe_array<CQGlobe_LocationArr>	CQGlobe_LocationPolygonArr;	// Ÿ�ϰ� ����ڰ�輱�� ��ж� ����� ��������� �������

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
} QGlobe_CROSS_INFO;

typedef struct
{
	CQGlobe_Location3D cPt1;
	CQGlobe_Location3D cPt2;
} QGlobe_SEGMENT;

typedef qglobe_array<QGlobe_SEGMENT>	CQGlobe_SegmentArr;	// ������ �������




QGlobe_CROSS_INFO  qglobe_GetPtsSegmentWithGrid (CQGlobe_Location3D *pPt1, CQGlobe_Location3D *pPt2, double minX, double maxX, double minY, double maxY, CQGlobe_LocationArr &io_ptArr);


void getCrossPtsInCellBoundary(CQGlobe_LocationArr &io_ptArrInBoundary, double minX, double maxX, double minY, double maxY, CQGlobe_LocationPolygonArr &o_crossLineArr);
void insertPtToBoundary (CQGlobe_Location3D &crossPt, CQGlobe_LocationPolygonArr &io_polyArrInBorder, double minX, double maxX, double minY, double maxY);
int findPtInBoundary (CQGlobe_Location3D &crossPt, double minX, double maxX, double minY, double maxY);


void qglobe_GetClosedPolygonArr (CQGlobe_LocationArr &ptArrInBoundary, CQGlobe_LocationPolygonArr &crossLineArr, CQGlobe_LocationPolygonArr &io_polygonArr);
bool isEqualPtWithCrossLine (CQGlobe_Location3D &compPt, CQGlobe_LocationPolygonArr &crossLineArr, int &nCrossLineNumber, bool &bNormal);
bool isEqualPtWithBorderLine (CQGlobe_Location3D &compPt, CQGlobe_SegmentArr &segmentArr, int &nBorderLineNumber);



/*******************************************************************************
	Function name:	qglobe_GetPolygonArrWithGrid.
	Description:	�ϳ��� ��� ����ڰ�輱�� ������� �Ͽ� �����Ǵ� �ݱ� ��������� ���.
	Return: 		int : ������ �ݱ��������� ����
	input:			i_arrPt, minX, maxX, minY, maxY
	in-output:		io_polygonArr
	** : ���� �� �Լ��� ���� ����ڰ�輱�� ����� ������ ȣ���
********************************************************************************/
int  qglobe_GetPolygonArrWithGrid(CQGlobe_LocationArr &i_arrPt, double minX, double maxX, double minY, double maxY, CQGlobe_LocationPolygonArr &io_polygonArr)
{
	CQGlobe_LocationPolygonArr crossLineArr;		// ���� ����ڰ�輱�� ������� �Ͽ� �����Ǵ� ��Ѽ����� ���
	CQGlobe_LocationArr		ptArrInBoundary;	// ���� 4���� ����ڰ�輱���� �������� �̷������ �����(�ּ������κ��� �����������)
	CQGlobe_Location3D			averagePt;		// �ݱ���������� ��.
	CQGlobe_Location3D			pt1, pt2, pt3;	// �ݱ������������� ����Ǵ� ����.





	// ���� ���� ����ڰ�輱���� ��Ѽ����� ���Ѵ�.
	int nCrossLineCount = qglobe_GetCrossLineArrWithGrid(i_arrPt, minX, maxX, minY, maxY, crossLineArr);
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
	qglobe_GetClosedPolygonArr (ptArrInBoundary, crossLineArr, io_polygonArr);

	// ����ڰ�輱�ۿ� ���̴� �ݱ���������� �����Ѵ�.(�� ���꿡 ������ �����︸ ����)
	int nPolygonCount = (int) io_polygonArr.size();
	for (int i = 0; i < nPolygonCount; i++)
	{
		CQGlobe_LocationArr &ptArrInPolygon = io_polygonArr[i];	// �ϳ��� �ݱ�������.
		int nPtCnt = (int)ptArrInPolygon.size();

		for (int k = 0; k < nPtCnt; k++)
		{
			pt1 = ptArrInPolygon[k];
			pt2 = ptArrInPolygon[(k+1) % nPtCnt];
			pt3 = ptArrInPolygon[(k+2) % nPtCnt];
			averagePt.m_tX = (pt1.m_tX + pt2.m_tX + pt3.m_tX) / 3;
			averagePt.m_tY = (pt1.m_tY + pt2.m_tY + pt3.m_tY) / 3;

			if (qglobe_PtIsInPolygon(averagePt, ptArrInPolygon))
			{
				// �ݱ��������� ����ڰ�輱�ȿ� ������ ������ �����Ѵ�.
				if (!qglobe_PtIsInPolygon(averagePt, i_arrPt))
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
	Function name:	qglobe_GetCrossLineArrWithGrid.
	Description:	�ϳ��� ��� ����ڰ�輱�� ������� �Ͽ� �����Ǵ� ��Ѽ����� ���.
	Return: 		������ ��Ѽ��� ����, (-1 : ����ڰ�輱��ü�� ���ȿ� ����)
	input:			i_arrPt, minX, maxX, minY, maxY
	output:			io_crossLineArr
********************************************************************************/
int  qglobe_GetCrossLineArrWithGrid(CQGlobe_LocationArr &i_arrPt, double minX, double maxX, double minY, double maxY, CQGlobe_LocationPolygonArr &io_crossLineArr, bool blClose)
{
	int nCrossLineCount = 0;		// ���� ����ڰ�輱�� ������� �����Ǵ� ��Ѽ����� ����
	CQGlobe_Location3D *pPt1 = NULL;	// ������ ������
	CQGlobe_Location3D *pPt2 = NULL;	// ������ ����
	CQGlobe_LocationArr ptArr;			// ������ �Ա���������κ��� �ⱸ��������� �����(�ϳ��� ��Ѽ�)
	QGlobe_CROSS_INFO eCrossInfo;		// �Ѱ� ���а� ���� �������
	bool blStartPtInGrid = false;	// ����ڰ�輱�� ù���� ���ȿ� ���̴°��� �����ϴ� ����


	int nPtCount = i_arrPt.size();
	int num = nPtCount;
	if(!blClose)
		num--;

	for (int i = 0; i < num; i++)
	{
		pPt1 = &i_arrPt[i];
		pPt2 = &i_arrPt[(i+1) % nPtCount];
		
		eCrossInfo = qglobe_GetPtsSegmentWithGrid (pPt1, pPt2, minX, maxX, minY, maxY, ptArr);
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
				CQGlobe_LocationArr & tmpPtArr = io_crossLineArr[0];
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
				CQGlobe_LocationArr & tmpPtArr = io_crossLineArr[0];
				tmpPtArr.insert(i_arrPt[0], 0);
			}
		}
	}

	nCrossLineCount = (int)io_crossLineArr.size();
	return nCrossLineCount;
}



/*******************************************************************************
	Function name:	qglobe_GetPtsSegmentWithGrid.
	Description:	�ϳ��� ��� ������ ������� ���Ѵ�(������ ������ ���ȿ� ���϶� ������ io_ptArr �� �߰��Ѵ�)
	Return: 		������� ���� ����
	input:			pPt1, pPt2, minX, maxX, minY, maxY
	output:			io_ptArr
********************************************************************************/
QGlobe_CROSS_INFO  qglobe_GetPtsSegmentWithGrid(CQGlobe_Location3D *pPt1, CQGlobe_Location3D *pPt2, double minX, double maxX, double minY, double maxY, CQGlobe_LocationArr &io_ptArr)
{

	QGlobe_CROSS_INFO eCrossInfo = CROSS_NO;	// ���� ������ �������.
	CQGlobe_LocationArr ptArr;					// ���� ������ ��������.

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
		CQGlobe_Location3D crossPt;	// �����


		// ��������� ��´�.
		if (qglobe_GetCrossPtSegmentWithAxisY (minX, minY, maxY, pPt1, pPt2, crossPt))
		{
			ptArr.push_back(crossPt);
		}
		if (qglobe_GetCrossPtSegmentWithAxisY (maxX, minY, maxY, pPt1, pPt2, crossPt))
		{
			ptArr.push_back(crossPt);
		}
		if (qglobe_GetCrossPtSegmentWithAxisX (minY, minX, maxX, pPt1, pPt2, crossPt))
		{
			ptArr.push_back(crossPt);
		}
		if (qglobe_GetCrossPtSegmentWithAxisX (maxY, minX, maxX, pPt1, pPt2, crossPt))
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
				CQGlobe_Location3D tmpPt = ptArr[0];
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
		CQGlobe_Location3D *pPt = &ptArr[i];
		io_ptArr.push_back(*pPt);
	}

	return eCrossInfo;
}



/*******************************************************************************
	Function name:	qglobe_GetCrossPtSegmentWithAxisY.
	Description:	���а� x = a (y�࿡ ������ ����) ���� �����
	Return: 		true (����� ����), false (���� Ȥ�� �ٱ��̹Ƿ� ����� ����)
	input:			xConst, minY, maxY, pPt1, pPt2
	output:			crossPt
********************************************************************************/
bool qglobe_GetCrossPtSegmentWithAxisY (double xConst, double minY, double maxY, CQGlobe_Location3D *pPt1, CQGlobe_Location3D *pPt2, CQGlobe_Location3D &crossPt)
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
	Function name:	qglobe_GetCrossPtSegmentWithAxisX.
	Description:	���а� y = a (x�࿡ ������ ����) ���� �����
	Return: 		true (����� ����), false (���� Ȥ�� �ٱ��̹Ƿ� ����� ����)
	input:			yConst, minX, maxX, pPt1, pPt2
	output:			crossPt
********************************************************************************/
bool qglobe_GetCrossPtSegmentWithAxisX (double yConst, double minX, double maxX, CQGlobe_Location3D *pPt1, CQGlobe_Location3D *pPt2, CQGlobe_Location3D &crossPt)
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
void getCrossPtsInCellBoundary(CQGlobe_LocationArr &io_ptArrInBoundary, double minX, double maxX, double minY, double maxY, CQGlobe_LocationPolygonArr &o_crossLineArr)
{
	CQGlobe_Location3D		pt0;	// ��Ѽ��� ù��
	CQGlobe_Location3D		pt1;	// ��Ѽ��� ����
	CQGlobe_Location3D		cellPt1 (minX, minY, 0);
	CQGlobe_Location3D		cellPt2 (minX, maxY, 0);
	CQGlobe_Location3D		cellPt3 (maxX, maxY, 0);
	CQGlobe_Location3D		cellPt4 (maxX, minY, 0);
	CQGlobe_LocationArr	ptArrInBorder1;
	CQGlobe_LocationArr	ptArrInBorder2;
	CQGlobe_LocationArr	ptArrInBorder3;
	CQGlobe_LocationArr	ptArrInBorder4;
	CQGlobe_LocationPolygonArr polyArrInBorder;
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
		CQGlobe_LocationArr &ptArrInBorder = polyArrInBorder[i];
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
void insertPtToBoundary (CQGlobe_Location3D &crossPt, CQGlobe_LocationPolygonArr &io_polyArrInBorder, double minX, double maxX, double minY, double maxY)
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
		CQGlobe_LocationArr & ptArr = io_polyArrInBorder[nSegmentNumber-1];
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
int findPtInBoundary (CQGlobe_Location3D &crossPt, double minX, double maxX, double minY, double maxY)
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


	CQGlobe_Location3D		cellPt1 (minX, minY, 0);
	CQGlobe_Location3D		cellPt2 (minX, maxY, 0);
	CQGlobe_Location3D		cellPt3 (maxX, maxY, 0);
	CQGlobe_Location3D		cellPt4 (maxX, minY, 0);

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
	Function name:	qglobe_GetClosedPolygonArr.
	Description:	������� �������� ��Ѽ���κ��� �ݱ� ��������� ����.
	Return: 		void
	input:			ptArrInBoundary, crossLineArr
	output:			io_polygonArr
********************************************************************************/
void qglobe_GetClosedPolygonArr (CQGlobe_LocationArr &ptArrInBoundary, CQGlobe_LocationPolygonArr &crossLineArr, CQGlobe_LocationPolygonArr &io_polygonArr)
{
	CQGlobe_LocationArr	ptArrInPolygon;		// �������� �ϳ��ϳ��� �ݱ�������.
	CQGlobe_SegmentArr		segmentArr;			// ��輱�� ���

	int		nPtCount;		// ��������� ����
	int		nSegmentCount;	// ����輱�� �������� �Ͽ� ����� �а���(����������� ����)

	QGlobe_SEGMENT		segment;		// ��輱���丷
	int		nCrossLineNumber  = 0;	// ������� ���ԵǴ� ��Ѽ���ȣ.
	int		nBorderLineNumber = 0;	// ������� ���ԵǴ� ��輱��ȣ.
	CQGlobe_Location3D		compPt;		// ���� ����� ����� ����(��輱���� ��)�̸� ������θ� �����µ��� ���ϴ���
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
					CQGlobe_LocationArr &crossLine = crossLineArr[nCrossLineNumber];	// ���õ� ��Ѽ�
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
									CQGlobe_LocationArr &crossLine = crossLineArr[nCrossLineNumber];	// ���õ� ��Ѽ�
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
							CQGlobe_LocationArr &crossLine = crossLineArr[nCrossLineNumber];	// ���õ� ��Ѽ�
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
				CQGlobe_LocationArr &crossLine = crossLineArr[nCrossLineNumber];	// ���õ� ��Ѽ�
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
bool isEqualPtWithCrossLine (CQGlobe_Location3D &compPt, CQGlobe_LocationPolygonArr &crossLineArr, int &nCrossLineNumber, bool &bNormal)
{
	CQGlobe_Location3D	startPt;	// ��Ѽ��� ù��
	CQGlobe_Location3D endPt;		// ��Ѽ��� ����

	int nCrossLineCount = (int)crossLineArr.size();
	for (int i = 0; i < nCrossLineCount; i++)
	{
		CQGlobe_LocationArr &crossLine = crossLineArr[i];
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
bool isEqualPtWithBorderLine (CQGlobe_Location3D &compPt, CQGlobe_SegmentArr &segmentArr, int &nBorderLineNumber)
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
void  qglobe_GetLineArrInRectangle(CQGlobe_LocationArr &i_arrPt, double minX, double maxX, double minY, double maxY, 
								 CQGlobe_LocationPolygonArr &o_LineArr)
{
	int nCrossLineCount = 0;		// ���� ����ڰ�輱�� ������� �����Ǵ� ��Ѽ����� ����
	CQGlobe_Location3D pPt1 ;	       // ������ ������
	CQGlobe_Location3D pPt2 ;	       // ������ ����
	CQGlobe_LocationArr crossPtArr; // ������ �Ա���������κ��� �ⱸ��������� �����(�ϳ��� ��Ѽ�)
	CQGlobe_LocationArr line;		  // 
	QGlobe_CROSS_INFO eCrossInfo;		// �Ѱ� ���а� ���� �������

	int nPtCount = i_arrPt.size();

	for (int i = 0; i < nPtCount-1; i++)
	{
		pPt1 = i_arrPt[i];
		pPt2 = i_arrPt[i+1];

		crossPtArr.clear();
		eCrossInfo = qglobe_GetPtsSegmentWithGrid (&pPt1, &pPt2, minX, maxX, minY, maxY, crossPtArr);
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
