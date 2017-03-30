#pragma once
#include "GDM_Define.h"
#include "GDM_Vector3D.h"
#include "GDM_Location.h"
#include "GDM_Matrix4.h"
#include "GDM_Quaternion.h"

#include "gdem_base_global.h"


//                | ZÃà 
//                |		 /YÃà
//                |		/
//				*****  /
//		   ****	  |  */***
//	   ****		  |	 /    ****
//	  *			  | /         *
//	  *			  |/----------*----------
//	  *			 			 *       XÃà (°æµµ0 , À§µµ 0)
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
// °æµµ , À§µµ ÁÂÇ¥º¯È¯ ¸ðÁì
GDEM_BASE_EXPORT void gdm_GetPosition(GDM_LOCATION *pLoc , CGDM_Point3DF* pPosF);
GDEM_BASE_EXPORT void gdm_GetLocation(IN CGDM_Point3DF* pPosF, OUT GDM_LOCATION *pLoc);
GDEM_BASE_EXPORT void gdm_GetLocation(IN CGDM_Point3DF* pPosF, OUT CGDM_Location3D *pLoc);
// ½ÁÁì±Ë¸êÃû·º¼Ý
/***************************************************************************
Ì»´Å»ô¶®Áì±ËËæº· ¼³Ì©´ô ¼°´ÉË¾ pCoord½ÁÁì±ËËæµá ²¿±¨²÷ ¸êÃûÃÔµÛË¾ ÊÐ²÷³Þ.
****************************************************************************/
GDEM_BASE_EXPORT void gdm_MatrixSCStoTCS(GDM_Coord3D *pCoord, CGDM_Matrix4 *pMat);

/***************************************************************************
pCoordËæº· ¼³Ì©´ô ¼°´ÉË¾ Ì»´Å»ô¶®Áì±ËËæµá ²¿±¨²÷ ¸êÃûÃÔµÛË¾ ÊÐ²÷³Þ.
****************************************************************************/
GDEM_BASE_EXPORT void gdm_MatrixTCStoSCS(GDM_Coord3D *pCoord, CGDM_Matrix4 *pMat);

GDEM_BASE_EXPORT void gdm_GetAngleBetweenTwoVector(CGDM_Vector3DF vt1, CGDM_Vector3DF vt2 , CGDM_Vector3DF upVt, double &angle);

//Ì»´Å°á°£Ëæº· ¼³Ì©´ô ÊÌÅð ¼ÑÊÌ¼ê »ô¶®Áì±Ë¶¦ °û »ô¶®Áì±ËÌ© ÊÌÅð ¾¢Ë¾ Ãä¼­¾¢Ëºµá Â×ÊÞ Ãä¼­»¤¿Ö³Þ. 
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

