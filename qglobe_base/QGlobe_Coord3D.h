#pragma once
#include "QGlobe_Define.h"
#include "QGlobe_Vector3D.h"
#include "QGlobe_Location.h"
#include "QGlobe_Matrix4.h"
#include "QGlobe_Quaternion.h"

#include "qglobe_base_global.h"


//                | Z�� 
//                |		 /Y��
//                |		/
//				*****  /
//		   ****	  |  */***
//	   ****		  |	 /    ****
//	  *			  | /         *
//	  *			  |/----------*----------
//	  *			 			 *       X�� (�浵0 , ���� 0)
//	   ****			     ****
//		  **** 		****
//     		  *****
//			 
//			 
//		    

struct QGlobe_Coord3D
{
	CQGlobe_Point3DF	m_org;
	CQGlobe_Vector3DF	m_x;
	CQGlobe_Vector3DF	m_y;
	CQGlobe_Vector3DF	m_z;
};
// �浵 , ���� ��ǥ��ȯ ����
QGLOBE_BASE_EXPORT void qglobe_GetPosition(QGlobe_LOCATION *pLoc , CQGlobe_Point3DF* pPosF);
QGLOBE_BASE_EXPORT void qglobe_GetLocation(IN CQGlobe_Point3DF* pPosF, OUT QGlobe_LOCATION *pLoc);
QGLOBE_BASE_EXPORT void qglobe_GetLocation(IN CQGlobe_Point3DF* pPosF, OUT CQGlobe_Location3D *pLoc);
// ����˸�������
/***************************************************************************
̻�Ż�������溷 ��̩�� ����˾ pCoord�������� ������ �����Ե�˾ �в���.
****************************************************************************/
QGLOBE_BASE_EXPORT void qglobe_MatrixSCStoTCS(QGlobe_Coord3D *pCoord, CQGlobe_Matrix4 *pMat);

/***************************************************************************
pCoord�溷 ��̩�� ����˾ ̻�Ż��������� ������ �����Ե�˾ �в���.
****************************************************************************/
QGLOBE_BASE_EXPORT void qglobe_MatrixTCStoSCS(QGlobe_Coord3D *pCoord, CQGlobe_Matrix4 *pMat);

QGLOBE_BASE_EXPORT void qglobe_GetAngleBetweenTwoVector(CQGlobe_Vector3DF vt1, CQGlobe_Vector3DF vt2 , CQGlobe_Vector3DF upVt, double &angle);

//̻�Űᰣ�溷 ��̩�� ���� ���̼� ������˶� �� �������̩ ���� ��˾ �伭��˺�� ���� �伭���ֳ�. 
QGLOBE_BASE_EXPORT void qglobe_RotateAroundXAxis(QGlobe_Coord3D *pCoord, double angle);
QGLOBE_BASE_EXPORT void qglobe_RotateAroundYAxis(QGlobe_Coord3D *pCoord, double angle);
QGLOBE_BASE_EXPORT void qglobe_RotateAroundZAxis(QGlobe_Coord3D *pCoord, double angle);
QGLOBE_BASE_EXPORT void qglobe_TCStoSCS(QGlobe_Coord3D *pCoord, CQGlobe_Point3DF *pnt);
QGLOBE_BASE_EXPORT void qglobe_VectorTCStoSCS(QGlobe_Coord3D *pCoord, CQGlobe_Vector3DF *pVtr);
QGLOBE_BASE_EXPORT void qglobe_SCStoTCS(QGlobe_Coord3D *pCoord, CQGlobe_Point3DF* pnt);
QGLOBE_BASE_EXPORT void qglobe_VectorSCStoTCS(QGlobe_Coord3D *pCoord, CQGlobe_Vector3DF *pVtr);

QGLOBE_BASE_EXPORT void qglobe_GetQuaternion(IN QGlobe_Coord3D *pCoord, OUT CQGlobe_Quaternion* pQuaternion);
QGLOBE_BASE_EXPORT void qglobe_SetQuaternion(CQGlobe_Quaternion* pQuaternion , QGlobe_Coord3D *pCoord);

QGLOBE_BASE_EXPORT void qglobe_RotateAroundVector(CQGlobe_Point3DF *pPt, double angle, CQGlobe_Vector3DF vt);
QGLOBE_BASE_EXPORT void qglobe_RotateAroundVector(QGlobe_Coord3D *pCoord, double angle, CQGlobe_Vector3DF vt);
QGLOBE_BASE_EXPORT void qglobe_RotateAroundLine(QGlobe_Coord3D *pCoord, double angle, CQGlobe_Point3DF org , CQGlobe_Vector3DF vt);

QGLOBE_BASE_EXPORT void qglobe_GetPerpendPt(CQGlobe_Point3DF* pResult, const CQGlobe_Point3DF* pPt , const CQGlobe_Point3DF* pOrg , const CQGlobe_Vector3DF* pVt);

QGLOBE_BASE_EXPORT void qglobe_RotateAroundLine(CQGlobe_Point3DF *pPt, double angle, CQGlobe_Point3DF org , CQGlobe_Vector3DF vt);

