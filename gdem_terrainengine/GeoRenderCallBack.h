
#pragma once

typedef int (*GEO_CALLBACK_GETDATA)(void* pUserData, void* param1, void* param2, void* param3);

typedef struct _geoCallBackList{
	GEO_CALLBACK_GETDATA	callBackGetRenderBuf; // for render terrain node
	GEO_CALLBACK_GETDATA	callBackGetName;  // for render name node	
	GEO_CALLBACK_GETDATA	callBackGetShape; // for render shape
	GEO_CALLBACK_GETDATA	callBackGetGrid;  // for render grid
	GEO_CALLBACK_GETDATA	callBackGetContour;  // for render contour
	GEO_CALLBACK_GETDATA	callBackGetRulerSeg;  // for render ruler segment
}GEO_CALL_BACK_LIST;
	

