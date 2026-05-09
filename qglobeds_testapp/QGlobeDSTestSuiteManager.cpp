#include <QDir>
#include <QString>

#include "QGlobeDSTestSuiteManager.h"
#include "../qglobeds_core/QGlobeDSCoreInterface.h"
#include "../qglobeds_database/QGlobeDSDatabaseInterface.h"
#include "../qglobeds_database/QGlobeDSBlockFileManager.h"
//#include "../qglobe_common/qglobe_environment.h"
#include "../qglobeds_common/QGlobeDSLog.h"

//#include "../qglobe_common/qglobe_protocol.h"
//#include "../qglobeds_common/QGlobeDSCommonInterface.h"
//#include "../qglobeds_core/QGlobeDSRequestProcess.h"
//#include "../qglobeds_common/QGlobeDSDefine.h"
//#include "../qglobeds_filemap/QGlobeDSFileInterface.h"
//#include "../qglobeds_network/QGlobeDSNetworkInterface.h"
//#include "../qglobeds_common/QGlobeDSLog.h"
//#include "../qglobeds_core/QGlobeDSRequestProcess.h"
//#include "../qglobeds_filemap/QGlobeDSFile.h"
//#include "../qglobeds_database/QGlobeDSTextureFile.h"

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

QGlobeDSTestSuiteManager::QGlobeDSTestSuiteManager()
{
	m_pAsyncFuncList = new QList<QString>;
	m_pAsyncFuncList->append("StartTimer");
}

QGlobeDSTestSuiteManager::~QGlobeDSTestSuiteManager()
{
	if(m_pAsyncFuncList)
	{
		delete m_pAsyncFuncList;
		m_pAsyncFuncList = NULL;
	}
}

void QGlobeDSTestSuiteManager::AppendTestStr(QString &a_strTest, const char* a_szFuncName, bool a_bResult, QString a_strComment)
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
	QGlobeDSLogOutput(E_LOG_ERROR, "GeoData type is unknown at GetGeoData. [type : %d]", 0);
	QGlobeDSLogOutput(E_LOG_ERROR, "GeoData response failed at QGlobeDSReceivedRequestCallback.[error : %s]", g_strErrorList[E_RESPONSE_ERROR_UNKNOWNGEODATATYPE]);

	return true;
}

bool Test_GetGeoData_Name(E_QGlobe_SUBDATA_TYPE a_subType, unsigned int a_nLevel, unsigned int a_nColumn, unsigned int a_nRow)
{
	ST_REQUEST_GEODATA* pRequest = (ST_REQUEST_GEODATA*)malloc(1024*150);
	pRequest->level = a_nLevel;
	pRequest->column = a_nColumn;
	pRequest->row = a_nRow;
	pRequest->main_type = E_QGlobe_MAINDATA_TYPE_NAME;
	pRequest->sub_type = a_subType;

	unsigned int nBytes;
	E_RESPONSE_ERROR_TYPE nErrorCode;
	bool bResult = GetGeoData(pRequest, (ST_RESPONSE_GEODATA*)pRequest, &nBytes, &nErrorCode);

	free(pRequest);

	return bResult;
}

bool Test_GetGeoData_Shape(E_QGlobe_SUBDATA_TYPE a_eDataType, unsigned int a_nLevel, unsigned int a_nColumn, unsigned int a_nRow)
{
	ST_REQUEST_GEODATA* pRequest = (ST_REQUEST_GEODATA*)malloc(1024*200);
	pRequest->level = a_nLevel;
	pRequest->column = a_nColumn;
	pRequest->row = a_nRow;
	pRequest->main_type = E_QGlobe_MAINDATA_TYPE_SHAPE;
	pRequest->sub_type = a_eDataType;

	unsigned int nBytes;
	E_RESPONSE_ERROR_TYPE nErrorCode;
	bool bResult = GetGeoData(pRequest, (ST_RESPONSE_GEODATA*)pRequest, &nBytes, &nErrorCode);

	free(pRequest);

	return bResult;
}

bool Test_GetGeoData_ShapeAll()
{
	return Test_GetGeoData_Shape(E_QGlobe_SUBDATA_TYPE_SHAPE_COUNTRY, 0, (1<<0)*307/360, (1<<0)*220/360)
		&& Test_GetGeoData_Shape(E_QGlobe_SUBDATA_TYPE_SHAPE_COUNTRY, 7, (1<<7)*307/360, (1<<7)*220/360)
		&& Test_GetGeoData_Shape(E_QGlobe_SUBDATA_TYPE_SHAPE_FIRSTADMIN, 0, (1<<0)*307/360, (1<<0)*220/360)
		&& Test_GetGeoData_Shape(E_QGlobe_SUBDATA_TYPE_SHAPE_FIRSTADMIN, 10, (1<<10)*307/360, (1<<10)*220/360);
}

bool Test_GetGeoData_NameAll()
{
	return /*Test_GetGeoData_Name(E_QGlobe_SUBDATA_TYPE_NAME_AIRFIELD, 14, 13956, 9954) 
		&& */ Test_GetGeoData_Name(E_QGlobe_SUBDATA_TYPE_NAME_ISLAND, 10, 252, 613)
		&& Test_GetGeoData_Name(E_QGlobe_SUBDATA_TYPE_NAME_COUNTRY, 3, (1<<3)*307/360, (1<<3)*220/360)
		&& Test_GetGeoData_Name(E_QGlobe_SUBDATA_TYPE_NAME_COUNTRY, 5, (1<<5)*307/360, (1<<5)*220/360)
		&& Test_GetGeoData_Name(E_QGlobe_SUBDATA_TYPE_NAME_COUNTRY, 8, (1<<8)*307/360, (1<<8)*220/360)
		&& Test_GetGeoData_Name(E_QGlobe_SUBDATA_TYPE_NAME_COUNTRY, 10, (1<<10)*307/360, (1<<10)*220/360);
}

bool Test_SearchName()
{
	//QGlobeDSGeoNameDBManager* t_DBManager = new QGlobeDSGeoNameDBManager();
	//if (t_DBManager == NULL)
	//{
	//	QGlobeDSLogOutput(E_LOG_ERROR, "new QGlobeDSGeoNameDBManager failed at InitializeGDSDatabase.");
	//	return false;
	//}

	//if (!t_DBManager->Initialize())
	//{
	//	QGlobeDSLogOutput(E_LOG_ERROR, "Connect mysql failed at InitializeGDSDatabase.");

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
	tKey = QString::fromLocal8Bit("�簡'��");

	QList<ST_RECORD_ID> tIDList;

	unsigned int		level;					// ���� ���ӱ��� ����
	float				left;					// ���� ���ӱ��� left
	float				top;					// ���� ���ӱ��� top
	float				right;					// ���� ���ӱ��� right
	float				bottom;					// ���� ���ӱ��� bottom
	unsigned int		start_index;			// ����� ǥ�� ���� �ε���
	unsigned int		end_index;				// ����� ǥ�� �� �ε���
	unsigned int		length;					// �˻����ڷ� ���� <= 254
	unsigned short		key_string[2];			// �˻����ڷ�
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
	bool bResult = GetGeoDataFromBlock(E_QGlobe_MAINDATA_TYPE_SHAPE, E_QGlobe_SUBDATA_TYPE_SHAPE_COUNTRY, 1, 1, 0,
								pResponse, &nBytes, &nErrorCode);

	bResult = GetGeoDataFromBlock(E_QGlobe_MAINDATA_TYPE_SHAPE, E_QGlobe_SUBDATA_TYPE_SHAPE_COUNTRY, 0, 0, 0,
		pResponse, &nBytes, &nErrorCode);

	bResult = GetGeoDataFromBlock(E_QGlobe_MAINDATA_TYPE_SHAPE, E_QGlobe_SUBDATA_TYPE_SHAPE_FIRSTADMIN, 7, 0, 0,
		pResponse, &nBytes, &nErrorCode);

	bResult = GetGeoDataFromBlock(E_QGlobe_MAINDATA_TYPE_DEM, E_QGlobe_SUBDATA_TYPE_DEM, 13, 128*14+0, 128*35+0,
		pResponse, &nBytes, &nErrorCode);

	bResult = GetGeoDataFromBlock(E_QGlobe_MAINDATA_TYPE_IMG, E_QGlobe_SUBDATA_TYPE_IMG, 0, 0, 0,
		pResponse, &nBytes, &nErrorCode);

	bResult = GetGeoDataFromBlock(E_QGlobe_MAINDATA_TYPE_IMG, E_QGlobe_SUBDATA_TYPE_IMG, 1, 0, 0,
		pResponse, &nBytes, &nErrorCode);

	bResult = GetGeoDataFromBlock(E_QGlobe_MAINDATA_TYPE_IMG, E_QGlobe_SUBDATA_TYPE_IMG, 2, 2, 2,
		pResponse, &nBytes, &nErrorCode);

	bResult = GetGeoDataFromBlock(E_QGlobe_MAINDATA_TYPE_IMG, E_QGlobe_SUBDATA_TYPE_IMG, 2, 0, 0,
		pResponse, &nBytes, &nErrorCode);

	bResult = GetGeoDataFromBlock(E_QGlobe_MAINDATA_TYPE_IMG, E_QGlobe_SUBDATA_TYPE_IMG, 3, 4, 4,
		pResponse, &nBytes, &nErrorCode);

	bResult = GetGeoDataFromBlock(E_QGlobe_MAINDATA_TYPE_IMG, E_QGlobe_SUBDATA_TYPE_IMG, 4, 8, 8,
		pResponse, &nBytes, &nErrorCode);

	bResult = GetGeoDataFromBlock(E_QGlobe_MAINDATA_TYPE_IMG, E_QGlobe_SUBDATA_TYPE_IMG, 13, 6975, 4984,
		pResponse, &nBytes, &nErrorCode);

	free(pResponse);

	return bResult;
}

QString QGlobeDSTestSuiteManager::GetFlowTestResult()
{
    QString curPath;
	// GetServerSeviceDir(curPath);
    if (!InitializeGDSCore(curPath))
		return false;

	m_strRet.clear();
	
	//AppendTestStr(m_strRet, "QGlobeDSCommon", QGlobeDSCommonTest(), "See Following Log File Info...");
	//AppendTestStr(m_strRet, "QGlobeDSCore()", QGlobeDSCoreTest());
	//AppendTestStr(m_strRet, "QGlobeDSDatabase()", QGlobeDSDatabaseTest());
	//AppendTestStr(m_strRet, "QGlobeDSFileManager()", QGlobeDSFileManagerTest());
	//AppendTestStr(m_strRet, "QGlobeDSFile Operation()", QGlobeDSFileTest());
	//AppendTestStr(m_strRet, "QGlobeDSNetwork()", QGlobeDSNetworkTest());

	ReleaseGDSCore();
	return m_strRet;
}

QString QGlobeDSTestSuiteManager::GetSyncTestResult()
{
	if (!InitializeGDSCore(QDir::currentPath()))
		return false;

	m_strRet.clear();

	AppendTestStr(m_strRet, "QGlobeDSLogOutput()", Test_LogOutput()); 
	AppendTestStr(m_strRet, "GetGeoData_NameAll()", Test_GetGeoData_NameAll()); 
	AppendTestStr(m_strRet, "GetGeoNameList()", Test_GetGeoNameList()); 
	AppendTestStr(m_strRet, "GetGeoDataFromBlock()", Test_GetGeoDataFromBlock()); 
	AppendTestStr(m_strRet, "GetGeoData_ShapeAll()", Test_GetGeoData_ShapeAll()); 
 
	ReleaseGDSCore();

	return m_strRet;
}

QString QGlobeDSTestSuiteManager::GetAsyncTestResult()
{
	m_strRet.clear();

	//nTimeID = StartTimer(100, TimeCallBackFunc, m_pAsyncFuncList);
	AppendTestStr(m_strRet, "StartTimer()", bTimeResult);
	return m_strRet;
}
