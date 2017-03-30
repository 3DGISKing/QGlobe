#include <QDir>
#include <QString>

#include "GDSTestSuiteManager.h"
#include "../gds_core/GDSCoreInterface.h"
#include "../gds_database/GDSDatabaseInterface.h"
#include "../gds_database/GDSBlockFileManager.h"
#include "../gdem_common/gdem_environment.h"
#include "../gds_common/GDSLog.h"

//#include "../gdem_common/gdem_protocol.h"
//#include "../gds_common/GDSCommonInterface.h"
//#include "../gds_core/GDSRequestProcess.h"
//#include "../gds_common/GDSDefine.h"
//#include "../gds_filemap/GDSFileInterface.h"
//#include "../gds_network/GDSNetworkInterface.h"
//#include "../gds_common/GDSLog.h"
//#include "../gds_core/GDSRequestProcess.h"
//#include "../gds_filemap/GDSFile.h"
//#include "../gds_database/GDSTextureFile.h"

#include <QFile>
#include <QtCore>

bool bTimeResult = false;
int nTimeID = 0;
void TimeCallBackFunc(void *a_pUserData)
{
	QList<QString> *pAsyncList = (QList<QString> *)a_pUserData;

	if( pAsyncList->indexOf("StartTimer") != -1 )
	{
		//StopTimer(nTimeID);
		bTimeResult = true;
	}
	else
		bTimeResult = true;
}

GDSTestSuiteManager::GDSTestSuiteManager()
{
	m_pAsyncFuncList = new QList<QString>;
	m_pAsyncFuncList->append("StartTimer");
}

GDSTestSuiteManager::~GDSTestSuiteManager()
{
	if(m_pAsyncFuncList)
	{
		delete m_pAsyncFuncList;
		m_pAsyncFuncList = NULL;
	}
}

void GDSTestSuiteManager::AppendTestStr(QString &a_strTest, const char* a_szFuncName, bool a_bResult, QString a_strComment)
{
	QString strOK(" -------- OK\n");
	QString strFail(" -------- Fail\n");

	a_strTest.append(a_szFuncName);
	
	if (a_bResult)
		a_strTest.append(strOK);
	else
		a_strTest.append(strFail);

	if (!a_strComment.isEmpty())
	{
		a_strTest.append("--------  ");
		a_strTest.append(a_strComment);
		a_strTest.append("  --------\n");
	}
	
	a_strTest.append("\n");
}

bool Test_LogOutput()
{
	GDSLogOutput(E_LOG_ERROR, "GeoData type is unknown at GetGeoData. [type : %d]", 0);
	GDSLogOutput(E_LOG_ERROR, "GeoData response failed at GDSReceivedRequestCallback.[error : %s]", g_strErrorList[E_RESPONSE_ERROR_UNKNOWNGEODATATYPE]);

	return true;
}

bool Test_GetGeoData_Name(E_GDM_SUBDATA_TYPE a_subType, unsigned int a_nLevel, unsigned int a_nColumn, unsigned int a_nRow)
{
	ST_REQUEST_GEODATA* pRequest = (ST_REQUEST_GEODATA*)malloc(1024*150);
	pRequest->level = a_nLevel;
	pRequest->column = a_nColumn;
	pRequest->row = a_nRow;
	pRequest->main_type = E_GDM_MAINDATA_TYPE_NAME;
	pRequest->sub_type = a_subType;

	unsigned int nBytes;
	E_RESPONSE_ERROR_TYPE nErrorCode;
	bool bResult = GetGeoData(pRequest, (ST_RESPONSE_GEODATA*)pRequest, &nBytes, &nErrorCode);

	free(pRequest);

	return bResult;
}

bool Test_GetGeoData_Shape(E_GDM_SUBDATA_TYPE a_eDataType, unsigned int a_nLevel, unsigned int a_nColumn, unsigned int a_nRow)
{
	ST_REQUEST_GEODATA* pRequest = (ST_REQUEST_GEODATA*)malloc(1024*200);
	pRequest->level = a_nLevel;
	pRequest->column = a_nColumn;
	pRequest->row = a_nRow;
	pRequest->main_type = E_GDM_MAINDATA_TYPE_SHAPE;
	pRequest->sub_type = a_eDataType;

	unsigned int nBytes;
	E_RESPONSE_ERROR_TYPE nErrorCode;
	bool bResult = GetGeoData(pRequest, (ST_RESPONSE_GEODATA*)pRequest, &nBytes, &nErrorCode);

	free(pRequest);

	return bResult;
}

bool Test_GetGeoData_ShapeAll()
{
	return Test_GetGeoData_Shape(E_GDM_SUBDATA_TYPE_SHAPE_COUNTRY, 0, (1<<0)*307/360, (1<<0)*220/360)
		&& Test_GetGeoData_Shape(E_GDM_SUBDATA_TYPE_SHAPE_COUNTRY, 7, (1<<7)*307/360, (1<<7)*220/360)
		&& Test_GetGeoData_Shape(E_GDM_SUBDATA_TYPE_SHAPE_FIRSTADMIN, 0, (1<<0)*307/360, (1<<0)*220/360)
		&& Test_GetGeoData_Shape(E_GDM_SUBDATA_TYPE_SHAPE_FIRSTADMIN, 10, (1<<10)*307/360, (1<<10)*220/360);
}

bool Test_GetGeoData_NameAll()
{
	return Test_GetGeoData_Name(E_GDM_SUBDATA_TYPE_NAME_AIRFIELD, 14, 13956, 9954)
		&& Test_GetGeoData_Name(E_GDM_SUBDATA_TYPE_NAME_ISLAND, 10, 252, 613)
		&& Test_GetGeoData_Name(E_GDM_SUBDATA_TYPE_NAME_COUNTRY, 3, (1<<3)*307/360, (1<<3)*220/360)
		&& Test_GetGeoData_Name(E_GDM_SUBDATA_TYPE_NAME_COUNTRY, 5, (1<<5)*307/360, (1<<5)*220/360)
		&& Test_GetGeoData_Name(E_GDM_SUBDATA_TYPE_NAME_COUNTRY, 8, (1<<8)*307/360, (1<<8)*220/360)
		&& Test_GetGeoData_Name(E_GDM_SUBDATA_TYPE_NAME_COUNTRY, 10, (1<<10)*307/360, (1<<10)*220/360);
}

bool Test_SearchName()
{
	//GDSGeoNameDBManager* t_DBManager = new GDSGeoNameDBManager();
	//if (t_DBManager == NULL)
	//{
	//	GDSLogOutput(E_LOG_ERROR, "new GDSGeoNameDBManager failed at InitializeGDSDatabase.");
	//	return false;
	//}

	//if (!t_DBManager->Initialize())
	//{
	//	GDSLogOutput(E_LOG_ERROR, "Connect mysql failed at InitializeGDSDatabase.");

	//	delete t_DBManager;
	//	t_DBManager = NULL;
	//	return false;
	//}
	return true;
}

bool Test_GetGeoNameList()
{

	QString tKey;
	unsigned short uGa[2] = {0xAC00, 0x00};
	//tKey = QString::fromUtf16(uGa);
	tKey = QString::fromLocal8Bit("사가'시");

	QList<ST_RECORD_ID> tIDList;

	unsigned int		level;					// 지도 보임구역 레벨
	float				left;					// 지도 보임구역 left
	float				top;					// 지도 보임구역 top
	float				right;					// 지도 보임구역 right
	float				bottom;					// 지도 보임구역 bottom
	unsigned int		start_index;			// 결과중 표시 시작 인덱스
	unsigned int		end_index;				// 결과중 표시 끝 인덱스
	unsigned int		length;					// 검색문자렬 길이 <= 254
	unsigned short		key_string[2];			// 검색문자렬
	ST_REQUEST_GEONAMESEARCH* pRequest = (ST_REQUEST_GEONAMESEARCH*)malloc(1024*150);
	memset(pRequest, 0x00, 1024*150);
	pRequest->type = E_SEARCH_TYPE_ID;
	ST_REQUEST_SEARCH_IDLIST* pReqData1 = (ST_REQUEST_SEARCH_IDLIST*)pRequest->reguest;
	pReqData1->level = 20;
	pReqData1->left = -10; //pReqData1->left = -180;
	pReqData1->top = 10;
	pReqData1->right = 10; //pReqData1->right = 180;
	pReqData1->bottom = -10; //pReqData1->bottom = -180;
	pReqData1->req_index = 0;
	pReqData1->length = (tKey.length() + 1) * 2;
	memcpy(pReqData1->key_string, tKey.utf16(), pReqData1->length);
	pReqData1->key_string[pReqData1->length] = 0;
	
	bool bResult = false;
	int req_index = 0;
	ST_RESPONSE_GEONAMESEARCH* pResponse = (ST_RESPONSE_GEONAMESEARCH*)pRequest;
	while (true)
	{
		unsigned int nBytes = 0;
		E_RESPONSE_ERROR_TYPE nErrorCode = E_RESPONSE_ERROR_NONE;
		bResult = GetGeoNameList(pRequest, pResponse, &nBytes, &nErrorCode);
		if (!bResult)
			break;

		if (pResponse->type == 0)
		{
 			ST_RESPONSE_SEARCH_IDLIST* pData = (ST_RESPONSE_SEARCH_IDLIST*)pResponse->response;

			for (int i=0; i<pData->count; i++)
			{
				tIDList.append(pData->id_list[i]);
			}

			if (pData->finished)
			{
				pRequest->type = E_SEARCH_TYPE_DATA;
				ST_REQUEST_SEARCH_DATA* pReqData2 = (ST_REQUEST_SEARCH_DATA*)pRequest->reguest;
				pReqData2->count = tIDList.count();
				for (int i=0; i<pReqData2->count; i++)
				{
					pReqData2->id_list[i] = tIDList[i];
				}
				continue;
			}
			else
			{
				pRequest->type = E_SEARCH_TYPE_ID;
				pReqData1->level = 20;
				pReqData1->left = -10; //pReqData1->left = -180;
				pReqData1->top = 10;
				pReqData1->right = 10; //pReqData1->right = 180;
				pReqData1->bottom = -10; //pReqData1->bottom = -180;
				req_index ++;
				pReqData1->req_index = req_index;
				pReqData1->length = (tKey.length() + 1) * 2;
				memcpy(pReqData1->key_string, tKey.utf16(), pReqData1->length);
				pReqData1->key_string[pReqData1->length] = 0;
				continue;
			}
		}
		else
		{
			break;
		}
	}


	free(pRequest);

	return bResult;
}

bool Test_GetGeoDataFromBlock()
{
	ST_GEODATA_DEM* pResponse = (ST_GEODATA_DEM*)malloc(1024*150);
	memset(pResponse, 0x00, 1024*150);

	unsigned int nBytes;
	E_RESPONSE_ERROR_TYPE nErrorCode;
	bool bResult = GetGeoDataFromBlock(E_GDM_MAINDATA_TYPE_SHAPE, E_GDM_SUBDATA_TYPE_SHAPE_COUNTRY, 1, 1, 0,
								pResponse, &nBytes, &nErrorCode);

	bResult = GetGeoDataFromBlock(E_GDM_MAINDATA_TYPE_SHAPE, E_GDM_SUBDATA_TYPE_SHAPE_COUNTRY, 0, 0, 0,
		pResponse, &nBytes, &nErrorCode);

	bResult = GetGeoDataFromBlock(E_GDM_MAINDATA_TYPE_SHAPE, E_GDM_SUBDATA_TYPE_SHAPE_FIRSTADMIN, 7, 0, 0,
		pResponse, &nBytes, &nErrorCode);

	bResult = GetGeoDataFromBlock(E_GDM_MAINDATA_TYPE_DEM, E_GDM_SUBDATA_TYPE_DEM, 13, 128*14+0, 128*35+0,
		pResponse, &nBytes, &nErrorCode);

	bResult = GetGeoDataFromBlock(E_GDM_MAINDATA_TYPE_IMG, E_GDM_SUBDATA_TYPE_IMG, 0, 0, 0,
		pResponse, &nBytes, &nErrorCode);

	bResult = GetGeoDataFromBlock(E_GDM_MAINDATA_TYPE_IMG, E_GDM_SUBDATA_TYPE_IMG, 1, 0, 0,
		pResponse, &nBytes, &nErrorCode);

	bResult = GetGeoDataFromBlock(E_GDM_MAINDATA_TYPE_IMG, E_GDM_SUBDATA_TYPE_IMG, 2, 2, 2,
		pResponse, &nBytes, &nErrorCode);

	bResult = GetGeoDataFromBlock(E_GDM_MAINDATA_TYPE_IMG, E_GDM_SUBDATA_TYPE_IMG, 2, 0, 0,
		pResponse, &nBytes, &nErrorCode);

	bResult = GetGeoDataFromBlock(E_GDM_MAINDATA_TYPE_IMG, E_GDM_SUBDATA_TYPE_IMG, 3, 4, 4,
		pResponse, &nBytes, &nErrorCode);

	bResult = GetGeoDataFromBlock(E_GDM_MAINDATA_TYPE_IMG, E_GDM_SUBDATA_TYPE_IMG, 4, 8, 8,
		pResponse, &nBytes, &nErrorCode);

	bResult = GetGeoDataFromBlock(E_GDM_MAINDATA_TYPE_IMG, E_GDM_SUBDATA_TYPE_IMG, 13, 6975, 4984,
		pResponse, &nBytes, &nErrorCode);

	free(pResponse);

	return bResult;
}

QString GDSTestSuiteManager::GetFlowTestResult()
{
    QString curPath;
	GetServerSeviceDir(curPath);
    if (!InitializeGDSCore(curPath))
		return false;

	m_strRet.clear();
	
	//AppendTestStr(m_strRet, "GDSCommon", GDSCommonTest(), "See Following Log File Info...");
	//AppendTestStr(m_strRet, "GDSCore()", GDSCoreTest());
	//AppendTestStr(m_strRet, "GDSDatabase()", GDSDatabaseTest());
	//AppendTestStr(m_strRet, "GDSFileManager()", GDSFileManagerTest());
	//AppendTestStr(m_strRet, "GDSFile Operation()", GDSFileTest());
	//AppendTestStr(m_strRet, "GDSNetwork()", GDSNetworkTest());

	ReleaseGDSCore();
	return m_strRet;
}

QString GDSTestSuiteManager::GetSyncTestResult()
{
	if (!InitializeGDSCore(QDir::currentPath()))
		return false;

	m_strRet.clear();

	AppendTestStr(m_strRet, "GDSLogOutput()", Test_LogOutput()); 
	AppendTestStr(m_strRet, "GetGeoData_NameAll()", Test_GetGeoData_NameAll()); 
	AppendTestStr(m_strRet, "GetGeoNameList()", Test_GetGeoNameList()); 
	AppendTestStr(m_strRet, "GetGeoDataFromBlock()", Test_GetGeoDataFromBlock()); 
	AppendTestStr(m_strRet, "GetGeoData_ShapeAll()", Test_GetGeoData_ShapeAll()); 
 
	ReleaseGDSCore();

	return m_strRet;
}

QString GDSTestSuiteManager::GetAsyncTestResult()
{
	m_strRet.clear();

	//nTimeID = StartTimer(100, TimeCallBackFunc, m_pAsyncFuncList);
	AppendTestStr(m_strRet, "StartTimer()", bTimeResult);
	return m_strRet;
}
