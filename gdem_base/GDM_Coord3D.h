#pragma once
#include "GDM_Define.h"
#include "GDM_Vector3D.h"
#include "GDM_Location.h"
#include "GDM_Matrix4.h"
#include "GDM_Quaternion.h"

#include "gdem_base_global.h"


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

struct GDM_Coord3D
{
	CGDM_Point3DF	m_org;
	CGDM_Vector3DF	m_x;
	CGDM_Vector3DF	m_y;
	CGDM_Vector3DF	m_z;
};
// �浵 , ���� ��ǥ��ȯ ����
GDEM_BASE_EXPORT void gdm_GetPosition(GDM_LOCATION *pLoc , CGDM_Point3DF* pPosF);
GDEM_BASE_EXPORT void gdm_GetLocation(IN CGDM_Point3DF* pPosF, OUT GDM_LOCATION *pLoc);
GDEM_BASE_EXPORT void gdm_GetLocation(IN CGDM_Point3DF* pPosF, OUT CGDM_Location3D *pLoc);
// ����˸�������
/***************************************************************************
̻�Ż�������溷 ��̩�� ����˾ pCoord�������� ������ �����Ե�˾ �в���.
****************************************************************************/
GDEM_BASE_EXPORT void gdm_MatrixSCStoTCS(GDM_Coord3D *pCoord, CGDM_Matrix4 *pMat);

/***************************************************************************
pCoord�溷 ��̩�� ����˾ ̻�Ż��������� ������ �����Ե�˾ �в���.
****************************************************************************/
GDEM_BASE_EXPORT void gdm_MatrixTCStoSCS(GDM_Coord3D *pCoord, CGDM_Matrix4 *pMat);

GDEM_BASE_EXPORT void gdm_GetAngleBetweenTwoVector(CGDM_Vector3DF vt1, CGDM_Vector3DF vt2 , CGDM_Vector3DF upVt, double &angle);

//̻�Űᰣ�溷 ��̩�� ���� ���̼� ������˶� �� �������̩ ���� ��˾ �伭��˺�� ���� �伭���ֳ�. 
GDEM_BASE_EXPORT void gdm_RotateAroundXAxis(GDM_Coord3D *pCoord, double angle);
GDEM_BASE_EXPORT void gdm_RotateAroundYAxis(GDM_Coord3D *pCoord, double angle);
GDEM_BASE_EXPORT void gdm_RotateAroundZAxis(GDM_Coord3D *pCoord, double angle);
GDEM_BASE_EXPORT void gdm_TCStoSCS(GDM_Coord3D *pCoord, CGDM_Point3DF *pnt);
GDEM_BASE_EXPORT void gdm_VectorTCStoSCS(GDM_Coord3D *pCoord, CGDM_Vector3DF *pVtr);
GDEM_BASE_EXPORT void gdm_SCStoTCS(GDM_Coord3D *pCoord, CGDM_Point3DF* pnt);
GDEM_BASE_EXPORT void gdm_VectorSCStoTCS(GDM_Coord3D *pCoord, CGDM_Vector3DF *pVtr);

GDEM_BASE_EXPORT void gdm_GetQuaternion(IN GDM_Coord3D *pCoord, OUT CGDM_Quaternion* pQuaternion);
GDEM_BASE_EXPORT void gdm_SetQuaternion(CGDM_Quaternion* pQuaternion , GDM_Coord3D *pCoord);

GDEM_BASE_EXPORT void gdm_RotateAroundVector(CGDM_Point3DF *pPt, double angle, CGDM_Vector3DF vt);
GDEM_BASE_EXPORT void gdm_RotateAroundVector(GDM_Coord3D *pCoord, double angle, CGDM_Vector3DF vt);
GDEM_BASE_EXPORT void gdm_RotateAroundLine(GDM_Coord3D *pCoord, double angle, CGDM_Point3DF org , CGDM_Vector3DF vt);

GDEM_BASE_EXPORT void gdm_GetPerpendPt(CGDM_Point3DF* pResult, const CGDM_Point3DF* pPt , const CGDM_Point3DF* pOrg , const CGDM_Vector3DF* pVt);

GDEM_BASE_EXPORT void gdm_RotateAroundLine(CGDM_Point3DF *pPt, double angle, CGDM_Point3DF org , CGDM_Vector3DF vt);

