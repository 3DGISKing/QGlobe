// GDSGeoNameDBManager.cpp : implementation file

#include "GDSGeoNameDBManager.h"

#include <QtCore/QStringList>
#include <QtSql/QSqlError>
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlQuery>
#include "QtSql/QSqlField"
#include <QtSql/QSqlQueryModel>

#include "../gds_common/GDSConfigMgr.h"
#include "../gds_common/GDSCommon.h"
#include "../gds_common/GDSLog.h"
#include "../gdem_common/gdem_config.h"
#include "../gdem_common/gdem_version.h"

#include "../gdem_gis/GIS_Polygon.h"
#include "../gdem_gis/GIS_Area.h"
#include "../gdem_gis/GIS_Path.h"
#include "../gdem_client/gdemclient.h"
#include "../gdem_libmain/Layer.h"


#ifndef USE_MYSQL
#define SQLITE_DB_PATH "geodb/geodb.db"
#endif

#define DB_TABLE_TYPE_NAME "tb_type"
#define RECORD_SPLIT_COUNT	1000000	//10^6
#define DB_IDCOUNT_LIMIT	10000	//10^4

GDSGeoNameDBManager::GDSGeoNameDBManager()
{
#ifdef USE_MYSQL
	m_Database = QSqlDatabase::addDatabase("QMYSQL");
	m_Database.setHostName(g_ServerConfig.GetDBServerIP());
	m_Database.setUserName(g_ServerConfig.GetDBUserID());
	m_Database.setPassword(g_ServerConfig.GetDBPassword());
	m_Database.setDatabaseName(g_ServerConfig.GetDBName());
#else
	m_Database = QSqlDatabase::addDatabase("QSQLITE");

	QString strDBPath;
#ifdef GEONAME_MODIFY_TEMP
	strDBPath.sprintf("%s/database/%s",
		g_ServerConfig.GetCurrentPath().toAscii().data(),
		SQLITE_DB_PATH);
#else
	QString path=g_app->GetWorkDir();

	strDBPath.sprintf("%s/%s",
		g_ServerConfig.GetFileDBPath().toAscii().data(),
		SQLITE_DB_PATH);

#endif
	m_Database.setDatabaseName(strDBPath);
#endif

	m_nTableCount = 0;
	m_pTableList = NULL;
}

GDSGeoNameDBManager::~GDSGeoNameDBManager()
{
	Release();
}

bool GDSGeoNameDBManager::Initialize()
{
	return connect_mysql() && InitTableData();
}

void GDSGeoNameDBManager::Release()
{
	ReleaseTableData();
	connect_mysql();
}

bool GDSGeoNameDBManager::connect_mysql()
{
	if (!m_Database.open())
	{
		GDSLogOutput(E_LOG_ERROR, "Database engine open failed at GDSGeoNameDBManager::connect_mysql.[error : %s]", m_Database.lastError().text().toAscii().data());
		return false;
	}

	return m_Database.isOpen();
}

void GDSGeoNameDBManager::disconnect_mysql()
{
	if (m_Database.isOpen())
		m_Database.close();
}

bool GDSGeoNameDBManager::InitTableData()
{
	if (!m_Database.isOpen())
	{
		if (!m_Database.open())
		{
			GDSLogOutput(E_LOG_ERROR, "Database engine open failed at GDSGeoNameDBManager::InitTableData.[error : %s]", m_Database.lastError().text().toAscii().data());
			return false;
		}
	}

	QString strQuery;
	strQuery.sprintf("SELECT * FROM %s ORDER BY id", DB_TABLE_TYPE_NAME);

	QSqlQueryModel oSqlQueryModel;
	oSqlQueryModel.setQuery(strQuery, m_Database);
	bool bSuccess = !oSqlQueryModel.lastError().isValid();
	if (!bSuccess)
	{
		m_Database.close();
		if (m_Database.open())
		{
			oSqlQueryModel.setQuery(strQuery, m_Database);
			bSuccess = !oSqlQueryModel.lastError().isValid();
		}
	}

	if (!bSuccess)
		return false;

	int nRowCount = oSqlQueryModel.rowCount();
	if (nRowCount <= 0)
	{
		GDSLogOutput(E_LOG_ERROR, "DB type table record was empty at GDSGeoNameDBManager::InitTableData.");
		return false;
	}

	ReleaseTableData();

	m_pTableList = new ST_DBTABLEINFO[nRowCount];
	if (m_pTableList == NULL)
		return false;

	m_nTableCount = nRowCount;
	for (int i = 0; i < m_nTableCount; i++)
	{
		m_pTableList[i].id		=          oSqlQueryModel.record(i).field("id").value().toInt();
		m_pTableList[i].type		=      oSqlQueryModel.record(i).field("type").value().toInt();
		QString strName	= oSqlQueryModel.record(i).field("table_name").value().toString();
		
		if(strName.length() + 1>TABLE_NAME_LENGTH)
		{
			qDebug("Program Exited in GDSGeoNameDBManager::InitTableData");
			qFatal("Error");
		}

		memcpy(m_pTableList[i].table_name, strName.toAscii().data(), strName.length() + 1);

		m_pTableList[i].start_level	= oSqlQueryModel.record(i).field("start_level").value().toInt();
		m_pTableList[i].end_level	= oSqlQueryModel.record(i).field("end_level").value().toInt();
		m_pTableList[i].shape_type	= (ShapeType)oSqlQueryModel.record(i).field("shape_type").value().toInt();
		m_pTableList[i].record_count = GetRecordCount(m_pTableList[i].table_name);

		m_pTableList[i].line_color = oSqlQueryModel.record(i).field("line_color").value().toUInt();
		m_pTableList[i].linewidth = oSqlQueryModel.record(i).field("line_width").value().toFloat();
		m_pTableList[i].fillcolor = oSqlQueryModel.record(i).field("fill_color").value().toUInt();
	}

	return true;
}

int GDSGeoNameDBManager::GetRecordCount(IN char* a_ptTableName)
{
	if (a_ptTableName == NULL)
		return -1;

	QString strQuery = "SELECT count(*) FROM ";
	strQuery += a_ptTableName;

	QSqlQueryModel oSqlQueryModel;
	oSqlQueryModel.setQuery(strQuery, m_Database);
	bool bSuccess = !oSqlQueryModel.lastError().isValid();
	if (!bSuccess)
	{
		m_Database.close();
		if (m_Database.open())
		{
			oSqlQueryModel.setQuery(strQuery, m_Database);
			bSuccess = !oSqlQueryModel.lastError().isValid();
		}
	}

	if (!bSuccess)
		return -1;

	int nRowCount = oSqlQueryModel.rowCount();
	if (nRowCount <= 0)
	{
		GDSLogOutput(E_LOG_ERROR, "DB type table record was empty at GDSGeoNameDBManager::GetRecordCount.");
		return -1;
	}

	return oSqlQueryModel.record(0).value(0).toInt();
}

void GDSGeoNameDBManager::ReleaseTableData()
{
	if (m_pTableList != NULL)
	{
		delete[] m_pTableList;
		m_pTableList = NULL;
	}
}

//////////////////////////////////////////////////////////////////////////
bool GDSGeoNameDBManager::GetGeoIdList(IN ST_REQUEST_SEARCH_IDLIST* a_pRequest, 
									   OUT ST_RESPONSE_SEARCH_IDLIST* a_pResponse, 
									   OUT unsigned int* a_pBytes, 
									   OUT E_RESPONSE_ERROR_TYPE* a_pErrorCode)
{
	if (!m_Database.isOpen())
	{
		if (!m_Database.open())
		{
			*a_pErrorCode = E_RESPONSE_ERROR_DATABASE_OPEN;
			GDSLogOutput(E_LOG_ERROR, "Database engine open failed at GDSGeoNameDBManager::GetGeoNameRecordData.[error : %s]", m_Database.lastError().text().toAscii().data());
			return false;
		}
	}

	QString strUnionQuery, strQuery;
	bool bFinished;
	int  nUnionTable;

	int req_index = a_pRequest->req_index;
	if (req_index == 0)
	{
		if (!MakeQueryInSight(a_pRequest, strUnionQuery))
		{
			a_pResponse->count = 0;
			a_pResponse->finished = false;
			*a_pBytes = sizeof(a_pResponse->count) + sizeof(a_pResponse->finished);
			return true;
		}

#ifdef GEONAME_MODIFY_TEMP
		bFinished = true;
#else
		bFinished = false;
#endif
	}
	else	 //split search
	{
		int nStart = (req_index-1) * RECORD_SPLIT_COUNT;
		MakeQueryOutSight(a_pRequest, &bFinished, &nUnionTable, strUnionQuery);
	}

	// Get Record Data in Need
	unsigned int nBytes = 0;

	if (nUnionTable == 1)
		strQuery.sprintf("%s LIMIT 0, %d", strUnionQuery.toUtf8().data(), DB_IDCOUNT_LIMIT);
	else
		strQuery.sprintf("SELECT class, id FROM ( %s ) AS world LIMIT 0, %d", strUnionQuery.toUtf8().data(), DB_IDCOUNT_LIMIT);


	m_Lock.lock();

	QSqlQueryModel oSqlQueryModel;

#ifdef GEONAME_MODIFY_TEMP
	oSqlQueryModel.setQuery(strUnionQuery, m_Database);
#else
	oSqlQueryModel.setQuery(strQuery, m_Database);
#endif

	bool bSuccess = !oSqlQueryModel.lastError().isValid();
	if (!bSuccess)
	{
		m_Database.close();
		if (m_Database.open())
		{
			oSqlQueryModel.setQuery(strQuery, m_Database);
			bSuccess = !oSqlQueryModel.lastError().isValid();
		}
	}

	if (bSuccess)
	{
		while (oSqlQueryModel.canFetchMore())
			oSqlQueryModel.fetchMore();

		int nRowCount = oSqlQueryModel.rowCount();

		a_pResponse->count = nRowCount;
		a_pResponse->finished = bFinished;

		unsigned char* pBuff = (unsigned char*)a_pResponse->id_list;
		for (unsigned int i=0; i<nRowCount; i++)
		{
			ST_RECORD_ID* pRecord = (ST_RECORD_ID*)pBuff;

			pRecord->sub_type = (E_GDM_SUBDATA_TYPE)oSqlQueryModel.record(i).value(0).toInt();
			pRecord->id = oSqlQueryModel.record(i).value(1).toInt();

			nBytes += sizeof(ST_RECORD_ID);
			pBuff += sizeof(ST_RECORD_ID);
		}
	}
	else
	{
		*a_pErrorCode = E_RESPONSE_ERROR_SQL_QUERY;
		GDSLogOutput(E_LOG_ERROR, "Query is invalid at GDSGeoNameDBManager::GetGeoNameRecordData.[error : %s]", oSqlQueryModel.lastError().text().toAscii().data());
	}

	m_Lock.unlock();

	*a_pBytes = nBytes + sizeof(a_pResponse->count) + sizeof(a_pResponse->finished);
  
	return bSuccess;
}

bool GDSGeoNameDBManager::MakeQueryInSight(IN ST_REQUEST_SEARCH_IDLIST* a_pRequest, OUT QString& a_strQuery)
{
	QString strKeyword;
	strKeyword = QString::fromUtf16(a_pRequest->key_string);
	
	EncodeKeyword(strKeyword);

	double fRightOver = 180.0 - std::max(a_pRequest->right, a_pRequest->left);

	QString strLeftLongitude = QString::number(std::min(a_pRequest->left, a_pRequest->right));
	QString strRightLongitude = QString::number(std::max(a_pRequest->right, a_pRequest->left));
	QString strBottomLatitude = QString::number(std::min(a_pRequest->bottom, a_pRequest->top));
	QString strTopLatitude = QString::number(std::max(a_pRequest->top, a_pRequest->bottom));

	// Main Condition
	QString strWhereQuery = "";
	QString strWhereQuerySame = "";


#ifdef GEONAME_MODIFY_TEMP
	a_strQuery = "SELECT type AS class, id FROM tb_mountain";

	bool bMakedQuery = true;
#else
	if (fRightOver > 0.0)
	{
		strWhereQuery += "(longitude >=" + strLeftLongitude + " AND longitude<=" + strRightLongitude;
		strWhereQuery += " AND latitude >=" + strBottomLatitude + " AND latitude<=" + strTopLatitude + ") ";
		strWhereQuery += " AND ((name LIKE '%" + strKeyword + "%') OR (name_ko LIKE '%" + strKeyword + "%')) ";

		strWhereQuerySame += "(longitude >=" + strLeftLongitude + " AND longitude<=" + strRightLongitude;
		strWhereQuerySame += " AND latitude >=" + strBottomLatitude + " AND latitude<=" + strTopLatitude + ") ";
		strWhereQuerySame += " AND ((name ='%" + strKeyword + "%') OR (name_ko ='%" + strKeyword + "%')) ";

	}
	else
	{
		fRightOver =  -180.0 - fRightOver;
		QString strRightLongitudeOver = QString::number(fRightOver);

		strWhereQuery += "( ((longitude>=" + strLeftLongitude + " AND longitude<=180.0) OR (longitude>=-180.0 AND longitude <=" + strRightLongitudeOver + ")) ";
		strWhereQuery += " AND latitude >=" + strBottomLatitude + " AND latitude<=" + strTopLatitude + ") ";
		strWhereQuery += " AND ((name LIKE '%" + strKeyword + "%') OR (name_ko LIKE '%" + strKeyword + "%')) ";

		strWhereQuerySame += "( ((longitude>=" + strLeftLongitude + " AND longitude<=180.0) OR (longitude>=-180.0 AND longitude <=" + strRightLongitudeOver + ")) ";
		strWhereQuerySame += " AND latitude >=" + strBottomLatitude + " AND latitude<=" + strTopLatitude + ") ";
		strWhereQuerySame += " AND ((name ='%" + strKeyword + "%') OR (name_ko ='%" + strKeyword + "%')) ";
	}

	a_strQuery.clear();

	QString strTableType="";
	bool bMakedQuery = false;
#ifdef USE_MYSQL
	for (int i=0; i<m_nTableCount; i++)
	{
		if (bMakedQuery)
			a_strQuery += "UNION ";

		if (m_pTableList[i].type == E_GDM_SUBDATA_TYPE_NAME_COUNTRY
			|| m_pTableList[i].type == E_GDM_SUBDATA_TYPE_NAME_POPULAR)
		{
			strTableType.sprintf("%d", m_pTableList[i].type);
			a_strQuery += "(SELECT ";
			a_strQuery += strTableType;
			a_strQuery += " AS class, id FROM ";
			a_strQuery += m_pTableList[i].table_name;
			a_strQuery += " WHERE ((name LIKE '%" + strKeyword + "%') OR (name_ko LIKE '%" + strKeyword + "%'))) ";			
		}
		else if (a_pRequest->level < m_pTableList[i].start_level)
		{
			strTableType.sprintf("%d", m_pTableList[i].type);
			a_strQuery += "(SELECT ";
			a_strQuery += strTableType;
			a_strQuery += " AS class, id FROM ";
			a_strQuery += m_pTableList[i].table_name;
			a_strQuery += " WHERE " + strWhereQuerySame + ") ";			
		}
		else
		{
			strTableType.sprintf("%d", m_pTableList[i].type);
			a_strQuery += "(SELECT ";
			a_strQuery += strTableType;
			a_strQuery += " AS class, id FROM ";
			a_strQuery += m_pTableList[i].table_name;
			a_strQuery += " WHERE " + strWhereQuery + ") ";
		}

		bMakedQuery = true;
	}
#else
	for (int i=0; i<m_nTableCount; i++)
	{
		if (bMakedQuery)
			a_strQuery += "UNION ";

		if (m_pTableList[i].type == E_GDM_SUBDATA_TYPE_NAME_COUNTRY
			|| m_pTableList[i].type == E_GDM_SUBDATA_TYPE_NAME_POPULAR)
		{
			strTableType.sprintf("%d", m_pTableList[i].type);
			a_strQuery += " SELECT ";
			a_strQuery += strTableType;
			a_strQuery += " AS class, id FROM ";
			a_strQuery += m_pTableList[i].table_name;
			a_strQuery += " WHERE ((name LIKE '%" + strKeyword + "%') OR (name_ko LIKE '%" + strKeyword + "%')) ";			
		}
		else if (a_pRequest->level < m_pTableList[i].start_level)
		{
			strTableType.sprintf("%d", m_pTableList[i].type);
			a_strQuery += " SELECT ";
			a_strQuery += strTableType;
			a_strQuery += " AS class, id FROM ";
			a_strQuery += m_pTableList[i].table_name;
			a_strQuery += " WHERE " + strWhereQuerySame + " ";			
		}
		else
		{
			strTableType.sprintf("%d", m_pTableList[i].type);
			a_strQuery += " SELECT ";
			a_strQuery += strTableType;
			a_strQuery += " AS class, id FROM ";
			a_strQuery += m_pTableList[i].table_name;
			a_strQuery += " WHERE " + strWhereQuery + " ";
		}

		bMakedQuery = true;
	}
#endif

#endif

	return bMakedQuery;
}

void GDSGeoNameDBManager::MakeQueryOutSight(IN ST_REQUEST_SEARCH_IDLIST* a_pRequest, OUT bool* a_pFinished, OUT int* a_pUnionTables , OUT QString& a_strQuery)
{
	int ntble_begin = 0;
	int ntble_end = 0;

	int nStartPos = (a_pRequest->req_index - 1) * RECORD_SPLIT_COUNT;
	int nEndPos = nStartPos + RECORD_SPLIT_COUNT;

	*a_pFinished = true;

	QString strKeyword;
	strKeyword = QString::fromUtf16(a_pRequest->key_string);
	EncodeKeyword(strKeyword);

	double fRightOver = 180.0 - std::max(a_pRequest->right, a_pRequest->left);

	QString strLeftLongitude = QString::number(std::min(a_pRequest->left, a_pRequest->right));
	QString strRightLongitude = QString::number(std::max(a_pRequest->right, a_pRequest->left));
	QString strBottomLatitude = QString::number(std::min(a_pRequest->bottom, a_pRequest->top));
	QString strTopLatitude = QString::number(std::max(a_pRequest->top, a_pRequest->bottom));

	// Main Condition
	QString strWhereQuery = "";

	if (fRightOver > 0.0){
		strWhereQuery += "(longitude <" + strLeftLongitude + " OR longitude>" + strRightLongitude + " ";
		strWhereQuery += "OR latitude <" + strBottomLatitude + " OR latitude>" + strTopLatitude + ") ";
		strWhereQuery += "AND ((name='%" + strKeyword + "%') OR (name_ko='%" + strKeyword + "%')) ";
	}else{

		fRightOver =  -180.0 - fRightOver;
		QString strRightLongitudeOver = QString::number(fRightOver);

		strWhereQuery += "( (longitude >" + strRightLongitudeOver + " AND longitude<" + strLeftLongitude + ") ";
		strWhereQuery += "OR latitude <" + strBottomLatitude + " OR latitude>" + strTopLatitude + ") ";
		strWhereQuery += "AND ((name='%" + strKeyword + "%') OR (name_ko='%" + strKeyword + "%')) ";
	}

	a_strQuery.clear();

	int nUnionTable = 0;
	QString strTable="";

#ifdef USE_MYSQL
	for (int i=0; i<m_nTableCount; i++)
	{
		//if (pSrcTableList[i].is_serach == 0)
		//	continue;
		
		if (nUnionTable > 0)
			a_strQuery += " UNION ";

		if (m_pTableList[i].type == E_GDM_SUBDATA_TYPE_NAME_COUNTRY
			|| m_pTableList[i].type == E_GDM_SUBDATA_TYPE_NAME_POPULAR)
			continue;

/*
 		strTable.sprintf("(SELECT %s AS class, id FROM %s WHERE (id >=%d AND id <=%d AND %s)) ", 
						strTableType.toUtf8().data(), 
						m_pTableList[i].table_name, 
						pSrcTableList[i].start_id,
						pSrcTableList[i].end_id,
						strWhereQuery.toUtf8().data());
*/
		strTable.sprintf("(SELECT %d AS class, id FROM %s WHERE %s) ", 
			m_pTableList[i].type,
			m_pTableList[i].table_name, 
			strWhereQuery.toUtf8().data());


		a_strQuery += strTable;
		nUnionTable++;
	}
#else
	for (int i=0; i<m_nTableCount; i++)
	{
		//if (pSrcTableList[i].is_serach == 0)
		//	continue;

		if (nUnionTable > 0)
			a_strQuery += " UNION ";

		if (m_pTableList[i].type == E_GDM_SUBDATA_TYPE_NAME_COUNTRY
			|| m_pTableList[i].type == E_GDM_SUBDATA_TYPE_NAME_POPULAR)
			continue;

/*
		strTable.sprintf(" SELECT %s AS class, id FROM %s WHERE (id >=%d AND id <=%d AND %s) ",
			strTableType.toUtf8().data(), 
			m_pTableList[i].table_name, 
			pSrcTableList[i].start_id,
			pSrcTableList[i].end_id,
			strWhereQuery.toUtf8().data());
*/
		strTable.sprintf(" SELECT %d AS class, id FROM %s WHERE %s ",
			m_pTableList[i].type,
			m_pTableList[i].table_name, 
			strWhereQuery.toUtf8().data());

		a_strQuery += strTable;
		nUnionTable++;
	}
#endif

	//delete[] pSrcTableList;

	*a_pUnionTables = nUnionTable;
}

bool GDSGeoNameDBManager::MakeQueryNameList(IN ST_REQUEST_SEARCH_DATA* a_pRequest, OUT int* a_pUnionTables, OUT QString& a_strQuery)
{

	int nId;
	int ntble_org = -1;
	int nUnionTables = 1;

	QString strUnionQuery="";
	QString strIdArr="";

	char* pBuff = (char*)a_pRequest->id_list;

#ifdef USE_MYSQL
	for (int i=0; i<a_pRequest->count; i++)
	{
		ST_RECORD_ID* pRecord = (ST_RECORD_ID*)pBuff;
		
		int nId = pRecord->id;
		int ntble_no;

		if ( GetTableInfoFromType(pRecord->sub_type, &ntble_no) == false ){
			return false;
		}

		if (i == 0)
			ntble_org = ntble_no; //initialize

		if (ntble_org != ntble_no)
		{
			if (strUnionQuery != ""){
				nUnionTables++;
				strUnionQuery += " UNION ";
			}

			strUnionQuery += " (SELECT id, type, name, name_ko, latitude, longitude, height, view_height FROM ";
			strUnionQuery += m_pTableList[ntble_org].table_name;
			strUnionQuery += " WHERE id in (" + strIdArr + ") ) ";

			ntble_org = ntble_no;
			strIdArr.clear();
		}

		if (strIdArr == "")
			strIdArr.sprintf("%d", nId);
		else
			strIdArr.sprintf("%s,%d", strIdArr.toAscii().data(), nId);

		pBuff+= sizeof(ST_RECORD_ID);
	}

	if (strUnionQuery != ""){
		strUnionQuery += " UNION ";
		nUnionTables++;
	}

	strUnionQuery += " (SELECT id, type, name, name_ko, latitude, longitude, height, view_height FROM ";
	strUnionQuery += m_pTableList[ntble_org].table_name;
	strUnionQuery += " WHERE id in (" + strIdArr + ") ) ";
#else

	for (int i=0; i<a_pRequest->count; i++)
	{
		ST_RECORD_ID* pRecord = (ST_RECORD_ID*)pBuff;

		int nId = pRecord->id;
		int ntble_no;

		if ( GetTableInfoFromType(pRecord->sub_type, &ntble_no) == false ){
			return false;
		}

		if (i == 0)
			ntble_org = ntble_no; //initialize

		if (ntble_org != ntble_no)
		{
			if (strUnionQuery != ""){
				nUnionTables++;
				strUnionQuery += " UNION ";
			}

			strUnionQuery += " SELECT id, type, name, name_ko, latitude, longitude, height, view_height FROM ";
			strUnionQuery += m_pTableList[ntble_org].table_name;
			strUnionQuery += " WHERE id in (" + strIdArr + ") ";

			ntble_org = ntble_no;
			strIdArr.clear();
		}

		if (strIdArr == "")
			strIdArr.sprintf("%d", nId);
		else
			strIdArr.sprintf("%s,%d", strIdArr.toAscii().data(), nId);

		pBuff+= sizeof(ST_RECORD_ID);
	}

	if (strUnionQuery != ""){
		strUnionQuery += " UNION ";
		nUnionTables++;
	}

	strUnionQuery += " SELECT id, type, name, name_ko, latitude, longitude, height, view_height FROM ";
	strUnionQuery += m_pTableList[ntble_org].table_name;
	strUnionQuery += " WHERE id in (" + strIdArr + ") ";

#endif

	a_strQuery.clear();
	a_strQuery = strUnionQuery;

	*a_pUnionTables = nUnionTables;

	return true;
}

bool GDSGeoNameDBManager::GetTableInfoFromType(IN int a_nType, OUT int* a_ptbleNo)
{	
	for (int i=0; i<m_nTableCount; i++)
	{
		if (m_pTableList[i].type == a_nType)
		{
			*a_ptbleNo = i;
			return true;
		}
	}

	*a_ptbleNo = 0;
	return false;
}

bool GDSGeoNameDBManager::GetGeoNameList(IN ST_REQUEST_SEARCH_DATA* a_pRequest, 
										   OUT ST_GEODATA_NAMELIST* a_pResponse, 
										   OUT unsigned int* a_pBytes, 
										   OUT E_RESPONSE_ERROR_TYPE* a_pErrorCode)
{
	if (!m_Database.isOpen())
	{
		if (!m_Database.open())
		{
			*a_pErrorCode = E_RESPONSE_ERROR_DATABASE_OPEN;
			GDSLogOutput(E_LOG_ERROR, "Database engine open failed at GDSGeoNameDBManager::GetGeoNameRecordData.[error : %s]", m_Database.lastError().text().toAscii().data());
			return false;
		}
	}

	QString strMainQuery, strQuery;

	if (a_pRequest->count < 1){	 //exception
		*a_pErrorCode = E_RESPONSE_ERROR_SQL_QUERY;
		GDSLogOutput(E_LOG_ERROR, "Request ID count is zero at GDSGeoNameDBManager::GetGeoNameList");
		return false;
	}

	int nUnionTables;
	// Edit Main Query String
	if (MakeQueryNameList(a_pRequest, &nUnionTables, strMainQuery) ==false)
	{
		*a_pErrorCode = E_RESPONSE_ERROR_SQL_QUERY;
		GDSLogOutput(E_LOG_ERROR, "Invalid Table type at GDSGeoNameDBManager::GetGeoNameList");
		return false;
	}

 	// Get Record Data in Need
	unsigned int nBytes = 0;

	if (nUnionTables == 1)		//without UNION table
		strQuery = strMainQuery;
	else						// UNION table
		strQuery = "SELECT id, type, name, name_ko, latitude, longitude, height, view_height FROM (" + strMainQuery + ") AS world ";


	m_Lock.lock();

	QSqlQueryModel oSqlQueryModel;
	oSqlQueryModel.setQuery(strQuery, m_Database);
	bool bSuccess = !oSqlQueryModel.lastError().isValid();
	if (!bSuccess)
	{
		m_Database.close();
		if (m_Database.open())
		{
			oSqlQueryModel.setQuery(strQuery, m_Database);
			bSuccess = !oSqlQueryModel.lastError().isValid();
		}
	}

	if (bSuccess)
	{
		// Fill Response Struct
		a_pResponse->count = oSqlQueryModel.rowCount();

		unsigned char* pBuff = (unsigned char*)a_pResponse->data_list;

		for (unsigned int i = 0; i < oSqlQueryModel.rowCount(); i++)
		{
			ST_GEODATA_NAME* pRecord = (ST_GEODATA_NAME*)pBuff;

			pRecord->id				= oSqlQueryModel.record(i).value(0).toInt();
			pRecord->type			= (E_GDM_SUBDATA_TYPE)oSqlQueryModel.record(i).value(1).toInt();
			QString strName			= oSqlQueryModel.record(i).value(2).toString();
			QString strNameKo		= oSqlQueryModel.record(i).value(3).toString();
			pRecord->latitude		= (float)oSqlQueryModel.record(i).value(4).toDouble();
			pRecord->longitude		= (float)oSqlQueryModel.record(i).value(5).toDouble();
			pRecord->height			= (float)oSqlQueryModel.record(i).value(6).toDouble();
			pRecord->view_height	= (float)oSqlQueryModel.record(i).value(7).toDouble();
			pRecord->length			= (strName.length() + 1) * 2;
			pRecord->length_ko		= (strNameKo.length() + 1) * 2;
			memcpy(pRecord->name, strName.utf16(), pRecord->length);
			memcpy((char*)pRecord->name + pRecord->length, strNameKo.utf16(), pRecord->length_ko);

			nBytes += sizeof(ST_GEODATA_NAME) - sizeof(unsigned short) * 2 + pRecord->length + pRecord->length_ko;

			Q_ASSERT(nBytes < 10*1024);

			pBuff += sizeof(ST_GEODATA_NAME) - sizeof(unsigned short) * 2 + pRecord->length + pRecord->length_ko;
		}
	}
	else
	{
		*a_pErrorCode = E_RESPONSE_ERROR_SQL_QUERY;
		GDSLogOutput(E_LOG_ERROR, "Query is invalid at GDSGeoNameDBManager::GetGeoNameRecordData.[error : %s]", oSqlQueryModel.lastError().text().toAscii().data());
	}

	m_Lock.unlock();


	*a_pBytes = nBytes + sizeof(ST_GEODATA_NAMELIST) - sizeof(ST_GEODATA_NAME);

	return bSuccess;
}

//////////////////////////////////////////////////////////////////////////

bool GDSGeoNameDBManager::GetGeoNameRecordData(IN ST_REQUEST_GEONAMESEARCH* a_pRequest, 
											  OUT ST_RESPONSE_GEONAMESEARCH* a_pResponse, 
											  OUT unsigned int* a_pBytes, 
											  OUT E_RESPONSE_ERROR_TYPE* a_pErrorCode)
{
	bool bSuccess;

	E_SEARCH_TYPE nReqType = a_pRequest->type;
	if (a_pRequest->type == 0) // request id list
	{
		ST_REQUEST_SEARCH_IDLIST* pRequest = (ST_REQUEST_SEARCH_IDLIST*)a_pRequest->reguest;
		ST_RESPONSE_SEARCH_IDLIST* pResponse = (ST_RESPONSE_SEARCH_IDLIST*)a_pResponse->response;

		bSuccess = GetGeoIdList(pRequest, pResponse, a_pBytes, a_pErrorCode);
	}
	else // request record data
	{
		ST_REQUEST_SEARCH_DATA* pRequest = (ST_REQUEST_SEARCH_DATA*)a_pRequest->reguest;
		ST_GEODATA_NAMELIST* pResponse = (ST_GEODATA_NAMELIST*)a_pResponse->response;
		bSuccess = GetGeoNameList(pRequest, pResponse, a_pBytes, a_pErrorCode);
	}

	a_pResponse->version = GetSubVersionBySearchName();
	a_pResponse->type = nReqType;

	if (bSuccess)
		*a_pBytes += sizeof(ST_RESPONSE_GEONAMESEARCH) - (sizeof(char) * 4);
	else
		*a_pBytes = sizeof(ST_RESPONSE_GEONAMESEARCH) - (sizeof(char) * 4);

	return bSuccess;
}

bool GDSGeoNameDBManager::GetGeoNameListInTile(IN E_GDM_SUBDATA_TYPE a_eSubDataType,
										IN unsigned int a_nLevel, 
										IN unsigned int a_nColumn, 
										IN unsigned int a_nRow, 
										OUT ST_GEODATA_COMMON* a_pResponse, 
										OUT unsigned int* a_pBytes, 
										OUT E_RESPONSE_ERROR_TYPE* a_pErrorCode)
{
	if (!m_Database.isOpen())
	{
		if (!m_Database.open())
		{
			*a_pErrorCode = E_RESPONSE_ERROR_DATABASE_OPEN;
			GDSLogOutput(E_LOG_ERROR, "Database engine open failed at GDSGeoNameDBManager::GetGeoNameListInTile.[error : %s]", m_Database.lastError().text().toAscii().data());
			return false;
		}
	}

	ST_GEODATA_NAMELIST* pNameListData = (ST_GEODATA_NAMELIST*)a_pResponse->buffer;

	QString strMainQuery, strQuery;
	double nLeftLongitude, nRightLongitude, nTopLatitude, nBottomLatitude;
	
	// Get Coordinates
	GetCoordinateByLevel(a_nLevel, a_nColumn, a_nRow, &nLeftLongitude, &nRightLongitude, &nTopLatitude, &nBottomLatitude);

	// Edit Query String By Level
	if (!EditQueryStringByLevel(a_eSubDataType, a_nLevel, nLeftLongitude, nRightLongitude, nTopLatitude, nBottomLatitude, strMainQuery))
	{
		pNameListData->count = 0;
		a_pResponse->bytes = sizeof(ST_GEODATA_NAMELIST) - sizeof(ST_GEODATA_NAME);
		*a_pBytes = sizeof(ST_GEODATA_COMMON) - sizeof(unsigned char)*4 + a_pResponse->bytes;
		return true;
	}

	// Get List Data
	strQuery = "SELECT id, type, name, name_ko, latitude, longitude, height, view_height FROM (" + strMainQuery + ") AS world";

	m_Lock.lock();

	QSqlQueryModel oSqlQueryModel;
	oSqlQueryModel.setQuery(strQuery, m_Database);
	bool bSuccess = !oSqlQueryModel.lastError().isValid();
	if (!bSuccess)
	{
		m_Database.close();
		if (m_Database.open())
		{
			oSqlQueryModel.setQuery(strQuery, m_Database);
			bSuccess = !oSqlQueryModel.lastError().isValid();
		}
	}

	if (bSuccess)
	{
		unsigned int nBytes = 0;
		pNameListData->count = oSqlQueryModel.rowCount();

		if (pNameListData->count > GEONAME_MAX_COUNT_IN_TILE)
		{
			pNameListData->count = GEONAME_MAX_COUNT_IN_TILE;
			GDSLogOutput(E_LOG_ERROR, "Name count was overflow. [type : %d, level : %d, x : %d, y : %d]", (int)a_eSubDataType, a_nLevel, a_nColumn, a_nRow);
		}

		if (pNameListData->count > 0)
		{
			unsigned char* pBuff = (unsigned char*)pNameListData->data_list;

			for (unsigned int i = 0; i < pNameListData->count; i++)
			{
				ST_GEODATA_NAME* pRecord = (ST_GEODATA_NAME*)pBuff;

				pRecord->id				= oSqlQueryModel.record(i).value(0).toInt();
				pRecord->type			= (E_GDM_SUBDATA_TYPE)oSqlQueryModel.record(i).value(1).toInt();
				QString strName			= oSqlQueryModel.record(i).value(2).toString();
				QString strNameKo		= oSqlQueryModel.record(i).value(3).toString();
				pRecord->latitude		= (float)oSqlQueryModel.record(i).value(4).toDouble();
				pRecord->longitude		= (float)oSqlQueryModel.record(i).value(5).toDouble();
				pRecord->height			= (float)oSqlQueryModel.record(i).value(6).toDouble();
				pRecord->view_height	= (float)oSqlQueryModel.record(i).value(7).toDouble();
				pRecord->length			= (strName.length() + 1) * 2;
				pRecord->length_ko		= (strNameKo.length() + 1) * 2;
				memcpy(pRecord->name, strName.utf16(), pRecord->length);
				memcpy((char*)pRecord->name + pRecord->length, strNameKo.utf16(), pRecord->length_ko);

				nBytes += sizeof(ST_GEODATA_NAME) - sizeof(unsigned short) * 2 + pRecord->length + pRecord->length_ko;
				pBuff += sizeof(ST_GEODATA_NAME) - sizeof(unsigned short) * 2 + pRecord->length + pRecord->length_ko;
			}
		}
		
		if(nBytes>PlaceNameLayer::MaxTileSize)
		{
			qDebug("Program Exited in GetGeoNameListInTile");
			qFatal("Program Exited in GetGeoNameListInTile!");
		}
		a_pResponse->bytes = sizeof(ST_GEODATA_NAMELIST) - sizeof(ST_GEODATA_NAME) + nBytes;
		*a_pBytes = sizeof(ST_GEODATA_COMMON) - sizeof(unsigned char)*4 + a_pResponse->bytes;
	}
	else
	{
		*a_pErrorCode = E_RESPONSE_ERROR_SQL_QUERY;
		GDSLogOutput(E_LOG_ERROR, "Query is invalid at GDSGeoNameDBManager::GetGeoNameListInTile.[error : %s]", oSqlQueryModel.lastError().text().toAscii().data());
	}


	m_Lock.unlock();

	return bSuccess;

}

bool GDSGeoNameDBManager::AddGeoNameData(IN ST_ADDNAME_DATA* a_pCommand, 
										 OUT ST_RESPONSE_DBCOMMAND* a_pResponse, 
										 OUT E_RESPONSE_ERROR_TYPE* a_pErrorCode)
{
	if (!m_Database.isOpen())
	{
		if (!m_Database.open())
		{
			*a_pErrorCode = E_RESPONSE_ERROR_DATABASE_OPEN;
			GDSLogOutput(E_LOG_ERROR, "Database engine open failed at GDSGeoNameDBManager::AddGeoNameData.[error : %s]", m_Database.lastError().text().toAscii().data());
			return false;
		}
	}

	bool bSuccess;
 	QString strQuery;
	QSqlQuery oSqlQuery(m_Database);

	if (EditAddQueryString(a_pCommand, strQuery)==false) 
	{
		*a_pErrorCode = E_RESPONSE_ERROR_SQL_QUERY;
		GDSLogOutput(E_LOG_ERROR, "Query is invalid at GDSGeoNameDBManager::AddGeoNameData.[error : %s]", oSqlQuery.lastError().text().toAscii().data());
		return false;
	}

	m_Lock.lock();
	bSuccess = oSqlQuery.exec(strQuery);
	m_Lock.unlock();

	if (bSuccess==false || EditGetMaxIdQueryString(a_pCommand, strQuery) ==false)
	{
		*a_pErrorCode = E_RESPONSE_ERROR_SQL_QUERY;
		GDSLogOutput(E_LOG_ERROR, "Invalid Table type at GDSGeoNameDBManager::AddGeoNameData");
		return false;
	}

	m_Lock.lock();
	QSqlQueryModel oSqlQueryModel;
	oSqlQueryModel.setQuery(strQuery, m_Database);
	bSuccess = !oSqlQueryModel.lastError().isValid();
	if (!bSuccess)
	{
		m_Database.close();
		if (m_Database.open())
		{
			oSqlQueryModel.setQuery(strQuery, m_Database);
			bSuccess = !oSqlQueryModel.lastError().isValid();
		}
	}
	m_Lock.unlock();

	if (bSuccess)
		a_pResponse->success = oSqlQueryModel.record(0).value(0).toInt();
	else
		a_pResponse->success = 0;

	return true;
}


bool GDSGeoNameDBManager::UpdateGeoNameData(IN ST_UPDATENAME_DATA* a_pCommand, 
											OUT ST_RESPONSE_DBCOMMAND* a_pResponse, 
											OUT E_RESPONSE_ERROR_TYPE* a_pErrorCode)
{
	if (!m_Database.isOpen())
	{
		if (!m_Database.open())
		{
			*a_pErrorCode = E_RESPONSE_ERROR_DATABASE_OPEN;
			GDSLogOutput(E_LOG_ERROR, "Database engine open failed at GDSGeoNameDBManager::UpdateGeoNameData.[error : %s]", m_Database.lastError().text().toAscii().data());
			return false;
		}
	}

	QString strQuery;
	QSqlQuery oSqlQuery(m_Database);

	// Edit Update Query String

	if (EditUpdateQueryString(a_pCommand, strQuery)==false)
	{
		*a_pErrorCode = E_RESPONSE_ERROR_SQL_QUERY;
		GDSLogOutput(E_LOG_ERROR, "Query is invalid at GDSGeoNameDBManager::UpdateGeoNameData.[error : %s]", oSqlQuery.lastError().text().toAscii().data());
		return false;
	}

	m_Lock.lock();
	bool bsuccess = oSqlQuery.exec(strQuery);
	m_Lock.unlock();

	if (bsuccess== false)
	{
		*a_pErrorCode = E_RESPONSE_ERROR_SQL_QUERY;
		GDSLogOutput(E_LOG_ERROR, "Query is invalid at GDSGeoNameDBManager::DeleteGeoNameData.[error : %s]", oSqlQuery.lastError().text().toAscii().data());
		return false;
	}

	return true;
}

bool GDSGeoNameDBManager::DeleteGeoNameData(IN ST_DELETENAME_DATA* a_pCommand, 
											OUT ST_RESPONSE_DBCOMMAND* a_pResponse, 
											OUT E_RESPONSE_ERROR_TYPE* a_pErrorCode)
{
	if (!m_Database.isOpen())
	{
		if (!m_Database.open())
		{
			*a_pErrorCode = E_RESPONSE_ERROR_DATABASE_OPEN;
			GDSLogOutput(E_LOG_ERROR, "Database engine open failed at GDSGeoNameDBManager::DeleteGeoNameData.[error : %s]", m_Database.lastError().text().toAscii().data());
			return false;
		}
	}

	QString strQuery;
	QSqlQuery oSqlQuery(m_Database);

	// Edit Update Query String
	if (EditDeleteQueryString(a_pCommand, strQuery) == false)
	{
		*a_pErrorCode = E_RESPONSE_ERROR_SQL_QUERY;
		GDSLogOutput(E_LOG_ERROR, "Query is invalid at GDSGeoNameDBManager::DeleteGeoNameData.[error : %s]", oSqlQuery.lastError().text().toAscii().data());
		return false;
	}

	m_Lock.lock();
	bool bsuccess = oSqlQuery.exec(strQuery);
	m_Lock.unlock();

	if (bsuccess== false)
	{
		*a_pErrorCode = E_RESPONSE_ERROR_SQL_QUERY;
		GDSLogOutput(E_LOG_ERROR, "Query is invalid at GDSGeoNameDBManager::DeleteGeoNameData.[error : %s]", oSqlQuery.lastError().text().toAscii().data());
		return false;
	}

	a_pResponse->success = 1;

	return true;
}

/*
void GDSGeoNameDBManager::EditMainQueryString(IN ST_REQUEST_GEONAMESEARCH* a_pRequest, 
											  OUT QString& a_strQuery)
{
	QString strKeyword;
	strKeyword = QString::fromUtf16(a_pRequest->key_string);

	QString strLeftLongitude = QString::number(std::min(a_pRequest->left, a_pRequest->right));
	QString strRightLongitude = QString::number(std::max(a_pRequest->right, a_pRequest->left));
	QString strBottomLatitude = QString::number(std::min(a_pRequest->bottom, a_pRequest->top));
	QString strTopLatitude = QString::number(std::max(a_pRequest->top, a_pRequest->bottom));
	
	// Main Condition
	QString strWhereQuery = "((name LIKE '%" + strKeyword + "%') OR (name_ko LIKE '%" + strKeyword + "%')) ";
	strWhereQuery += "AND (longitude BETWEEN " + strLeftLongitude + " AND " + strRightLongitude + ") ";
	strWhereQuery += "AND (latitude BETWEEN " + strBottomLatitude + " AND " + strTopLatitude + ")";
	
	// Extra Condition
	QString strExtraQuery = "((name LIKE '%" + strKeyword + "%') OR (name_ko LIKE '%" + strKeyword + "%')) ";
	strExtraQuery += "AND ((longitude NOT BETWEEN " + strLeftLongitude + " AND " + strRightLongitude + ") ";
	strExtraQuery += "OR (latitude NOT BETWEEN " + strBottomLatitude + " AND " + strTopLatitude + "))";

	a_strQuery.clear();

	char szNum[2] = {0};
#ifdef USE_MYSQL
	for (int i=0; i<m_nTableCount; i++)
	{
		if (i != 0)
			a_strQuery += " UNION ";
		
		szNum[0] = 0x31 + i;
		a_strQuery += "(SELECT ";
		a_strQuery += szNum;
		a_strQuery += " AS class, type, name, name_ko, latitude, longitude, height, view_height FROM ";
		a_strQuery += m_pTableList[i].table_name;
		a_strQuery += " WHERE " + strWhereQuery + ") ";
	}
	
	// Extra Data
	for (int i=0; i<m_nTableCount; i++)
	{
		szNum[0] = 0x31 + i;
		a_strQuery += "UNION ";
		a_strQuery += "(SELECT ";
		a_strQuery += szNum;
		a_strQuery += " AS class, type, name, name_ko, latitude, longitude, height, view_height FROM ";
		a_strQuery += m_pTableList[i].table_name;
		a_strQuery += " WHERE " + strExtraQuery + ") ";
	}
#else
	for (int i=0; i<m_nTableCount; i++)
	{
		if (i != 0)
			a_strQuery += " UNION ";

		szNum[0] = 0x31 + i;
		a_strQuery += "SELECT ";
		a_strQuery += szNum;
		a_strQuery += " AS class, type, name, name_ko, latitude, longitude, height, view_height FROM ";
		a_strQuery += m_pTableList[i].table_name;
		a_strQuery += " WHERE " + strWhereQuery + " ";
	}

	// Extra Data
	for (int i=0; i<m_nTableCount; i++)
	{
		szNum[0] = 0x31 + i;
		a_strQuery += " UNION ";
		a_strQuery += "SELECT ";
		a_strQuery += szNum;
		a_strQuery += " AS class, type, name, name_ko, latitude, longitude, height, view_height FROM ";
		a_strQuery += m_pTableList[i].table_name;
		a_strQuery += " WHERE " + strExtraQuery + " ";
	}
#endif
}

*/
bool GDSGeoNameDBManager::EditQueryStringByLevel(IN E_GDM_SUBDATA_TYPE a_eSubDataType,
												 IN unsigned int a_nLevel,
												 IN double a_nLeftLongitude,
												 IN double a_nRightLongitude,
												 IN double a_nTopLatitude,
												 IN double a_nBottomLatitude,
												 OUT QString& a_strQuery)
{
	QString strLeftLongitude = QString::number(std::min(a_nLeftLongitude, a_nRightLongitude));
	QString strRightLongitude = QString::number(std::max(a_nRightLongitude, a_nLeftLongitude));
	QString strBottomLatitude = QString::number(std::min(a_nBottomLatitude, a_nTopLatitude));
	QString strTopLatitude = QString::number(std::max(a_nTopLatitude, a_nBottomLatitude));

	QString strWhereQuery = "(longitude >=" + strLeftLongitude + " AND longitude<=" + strRightLongitude;
	strWhereQuery += " AND latitude >=" + strBottomLatitude + " AND latitude<=" + strTopLatitude + ") ";

	
	bool bFirst = true;
	int nCount = 0;
	a_strQuery.clear();

#ifdef USE_MYSQL
	for (int i=0; i<m_nTableCount; i++)
	{

		if (a_eSubDataType == m_pTableList[i].type && a_nLevel >= m_pTableList[i].start_level)
		{
			if (!bFirst)
				a_strQuery += "UNION ";
			else
				bFirst = false;

			a_strQuery += "(SELECT id, type, name, name_ko, latitude, longitude, height, view_height FROM ";
			a_strQuery += m_pTableList[i].table_name;
			a_strQuery += " WHERE " + strWhereQuery + ") ";
			nCount ++;
		}
	}
#else
	for (int i=0; i<m_nTableCount; i++)
	{
		if (a_eSubDataType == m_pTableList[i].type && a_nLevel >= m_pTableList[i].start_level)
		{
			if (!bFirst)
				a_strQuery += "UNION ";
			else
				bFirst = false;

			a_strQuery += "SELECT id, type, name, name_ko, latitude, longitude, height, view_height FROM ";
			a_strQuery += m_pTableList[i].table_name;
			a_strQuery += " WHERE " + strWhereQuery + " ";
			nCount ++;
		}
	}
#endif

	return (nCount > 0);
}

void GDSGeoNameDBManager::GetCoordinateByLevel(IN unsigned int a_nLevel, 
											   IN unsigned int a_nColumn, 
											   IN unsigned int a_nRow, 
											   OUT double* a_pdLeftLongitude, 
											   OUT double* a_pdRightLongitude, 
											   OUT double* a_pdTopLatitude,
											   OUT double* a_pdBottomLatitude)
{
	double nLongitudeRange = 180;
	double nLatitudeRange = 180;

	if (a_nLevel == 0)
	{
		*a_pdLeftLongitude	= -nLongitudeRange;
		*a_pdRightLongitude	= nLongitudeRange;
		*a_pdTopLatitude	= nLatitudeRange;
		*a_pdBottomLatitude	= -nLatitudeRange;

		return;
	}

	unsigned int nColumn = pow((double)2, (double)(a_nLevel - 1));
	unsigned int nRow = nColumn;

	unsigned int nColumnIndex = (a_nColumn >= nColumn)? a_nColumn - nColumn: nColumn - a_nColumn - 1;
	unsigned int nRowIndex = (a_nRow >= nRow)? a_nRow - nRow: nRow - a_nRow - 1;

	double nLeftLongitude = (nLongitudeRange / nColumn) * nColumnIndex;
	double nRightLongitude = nLeftLongitude + (nLongitudeRange / nColumn);
	double nTopLatitude = (nLatitudeRange / nRow) * nRowIndex;
	double nBottomLatitude = nTopLatitude + (nLatitudeRange / nRow);
	
	if (a_nColumn >= nColumn)
	{
		*a_pdLeftLongitude = nLeftLongitude;
		*a_pdRightLongitude = nRightLongitude;
	}
	else
	{
		*a_pdLeftLongitude = -nLeftLongitude;
		*a_pdRightLongitude = -nRightLongitude;
	}

	if (a_nRow >= nRow)
	{
		*a_pdTopLatitude = nTopLatitude;
		*a_pdBottomLatitude = nBottomLatitude;
	}
	else
	{
		*a_pdTopLatitude = -nTopLatitude;
		*a_pdBottomLatitude = -nBottomLatitude;
	}
}

bool GDSGeoNameDBManager::EditAddQueryString(IN ST_GEODATA_NAME* a_pstGeoNameData, OUT QString& a_strQuery)
{
 	int ntble_no;

	if ( GetTableInfoFromType(a_pstGeoNameData->type, &ntble_no) == false ){
		return false;
	}

	unsigned short* pNamePtr = a_pstGeoNameData->name;
	QString strNameEn = QString::fromUtf16(pNamePtr, a_pstGeoNameData->length);
	EncodeKeyword(strNameEn);

	int nLen = a_pstGeoNameData->length;
	nLen >>= 1;

	pNamePtr += nLen;
	QString strName = QString::fromUtf16(pNamePtr, a_pstGeoNameData->length_ko);
	EncodeKeyword(strName);

	QString strLongitude = QString::number(a_pstGeoNameData->longitude);
	QString strLatitude = QString::number(a_pstGeoNameData->latitude);
	QString strHeight = QString::number(a_pstGeoNameData->height);
	QString strViewHeight = QString::number(a_pstGeoNameData->view_height);
	
#ifdef USE_MYSQL
	a_strQuery.sprintf("INSERT INTO %s (type, name, name_ko, latitude, longitude, height, view_height) VALUES(%d,N'%s',N'%s','%s','%s','%s','%s') ", 
						m_pTableList[ntble_no].table_name,
						a_pstGeoNameData->type,
						strNameEn.toUtf8().data(),
						strName.toUtf8().data(),
						strLatitude.toAscii().data(),
						strLongitude.toAscii().data(),
						strHeight.toAscii().data(),
						strViewHeight.toAscii().data()
						);
#else
	a_strQuery.sprintf("INSERT INTO %s (type, name, name_ko, latitude, longitude, height, view_height) VALUES(%d,'%s','%s','%s','%s','%s','%s') ", 
						m_pTableList[ntble_no].table_name,
						a_pstGeoNameData->type,
						strNameEn.toUtf8().data(),
						strName.toUtf8().data(),
						strLatitude.toAscii().data(),
						strLongitude.toAscii().data(),
						strHeight.toAscii().data(),
						strViewHeight.toAscii().data()
						);
#endif


	return true;
}

bool GDSGeoNameDBManager::EditGetMaxIdQueryString(IN ST_GEODATA_NAME* a_pstGeoNameData, OUT QString& a_strQuery)
{
	int ntble_no;

	if ( GetTableInfoFromType(a_pstGeoNameData->type, &ntble_no) == false ){
		return false;
	}

	a_strQuery.sprintf("SELECT MAX(id) FROM %s", m_pTableList[ntble_no].table_name);

	return true;
}

bool GDSGeoNameDBManager::EditUpdateQueryString(IN ST_UPDATENAME_DATA* a_pstUpdateNameData, OUT QString& a_strQuery)
{
 	int ntble_no;

	if ( GetTableInfoFromType(a_pstUpdateNameData->type, &ntble_no) == false ){
		return false;
	}

	unsigned short* pNamePtr = a_pstUpdateNameData->name;
	QString strNameEn = QString::fromUtf16(pNamePtr, a_pstUpdateNameData->length);
	EncodeKeyword(strNameEn);

	int nLen = a_pstUpdateNameData->length;
	nLen >>= 1;

	pNamePtr += nLen;
	QString strName = QString::fromUtf16(pNamePtr, a_pstUpdateNameData->length_ko);
	EncodeKeyword(strName);

	QString strLongitude = QString::number(a_pstUpdateNameData->longitude);
	QString strLatitude = QString::number(a_pstUpdateNameData->latitude);
	QString strHeight = QString::number(a_pstUpdateNameData->height);
	QString strViewHeight = QString::number(a_pstUpdateNameData->view_height);


#ifdef USE_MYSQL
	a_strQuery.sprintf("UPDATE %s SET type=%d, name=N'%s', name_ko=N'%s', latitude='%s', longitude='%s', height='%s', view_height='%s' WHERE id=%d ", 
		m_pTableList[ntble_no].table_name,
		a_pstUpdateNameData->type,
		strNameEn.toUtf8().data(),
		strName.toUtf8().data(),
		strLatitude.toAscii().data(),
		strLongitude.toAscii().data(),
		strHeight.toAscii().data(),
		strViewHeight.toAscii().data(),
		a_pstUpdateNameData->id
		);
#else

	a_strQuery.sprintf("UPDATE %s SET type=%d, name='%s', name_ko='%s', latitude='%s', longitude='%s', height='%s', view_height='%s' WHERE id=%d ", 
		m_pTableList[ntble_no].table_name,
		a_pstUpdateNameData->type,
		strNameEn.toUtf8().data(),
		strName.toUtf8().data(),
		strLatitude.toAscii().data(),
		strLongitude.toAscii().data(),
		strHeight.toAscii().data(),
		strViewHeight.toAscii().data(),
		a_pstUpdateNameData->id
		);
#endif

	return true;
}

bool GDSGeoNameDBManager::EditDeleteQueryString(IN ST_DELETENAME_DATA* a_pstDelNameData, OUT QString& a_strQuery)
{
	int ntble_no;

	if ( GetTableInfoFromType(a_pstDelNameData->sub_type, &ntble_no) == false ){
		return false;
	}
 
	a_strQuery.sprintf("DELETE FROM %s WHERE id=%d", m_pTableList[ntble_no].table_name, a_pstDelNameData->id);

	return true;
}

void GDSGeoNameDBManager::EncodeKeyword(INOUT QString& a_strKeyword)
{
	QString strKeyword = a_strKeyword;

	strKeyword = strKeyword.replace("'", "''");
	a_strKeyword = strKeyword;
}

bool GDSGeoNameDBManager::AddGeoBuildingData(GeoBuildingProperty* p)
{
	if (!m_Database.isOpen())
	{
		if (!m_Database.open())
		{
			GDSLogOutput(E_LOG_ERROR, "Database engine open failed at GDSGeoNameDBManager::AddGeoBuildingData");
			return false;
		}
	}

	bool bSuccess;
	QString strQuery;
	QSqlQuery oSqlQuery(m_Database);

	strQuery="INSERT INTO tb_building ( id ,name,postname,loc_id,ban,floor,minlatitude,minlongitude,maxlatitude,maxlongitude,misc)";
	strQuery+="VALUES                 ( %lld ,'%s','%s'     ,%d   ,'%s',%d   ,%f,         %f          ,%f         ,%f          ,'%s')";

	strQuery.sprintf(strQuery.toUtf8().data(),
					p->_ID,
					p->_Name.toUtf8().data(),
					p->_PostName.toUtf8().data(),
					p->_Loc_ID,
					p->_Ban.toUtf8().data(), 
					p->_Floor,
					p->_MinLatitude,
					p->_MinLongitude,
					p->_MaxLatitude,
					p->_MaxLongitude,
					p->_Misc.toUtf8().data());
  
	m_Lock.lock();
	bSuccess = oSqlQuery.exec(strQuery);
	m_Lock.unlock();

	return bSuccess;
}

bool GDSGeoNameDBManager::ExistGeoBuildingData(qint64 id)
{
	if (!m_Database.isOpen())
	{
		if (!m_Database.open())
		{
			GDSLogOutput(E_LOG_ERROR, "Database engine open failed at GDSGeoNameDBManager::AddGeoBuildingData");
			return false;
		}
	}

	bool bSuccess;
	QString strQuery;
	QSqlQueryModel oSqlQueryModel;

	strQuery.sprintf("SELECT * FROM tb_building WHERE id=%lld",id);

	m_Lock.lock();
	oSqlQueryModel.setQuery(strQuery,m_Database);

	m_Lock.unlock();

	bSuccess = !oSqlQueryModel.lastError().isValid();

	if(!bSuccess)
		return false;

	int nRowCount = oSqlQueryModel.rowCount();

	if(nRowCount==0)
		return false;

	Q_ASSERT(nRowCount==1);

	return true;
}
bool GDSGeoNameDBManager::UpdateGeoBuildingData(GeoBuildingProperty* p)
{
	if (!m_Database.isOpen())
	{
		if (!m_Database.open())
		{
			GDSLogOutput(E_LOG_ERROR, "Database engine open failed at GDSGeoNameDBManager::AddGeoBuildingData");
			return false;
		}
	}

	bool bSuccess;
	QString strQuery;
	QSqlQuery oSqlQuery(m_Database);

	strQuery="UPDATE tb_building SET minlongitude=%f, maxlongitude=%f,minlatitude=%f,maxlatitude=%f,name='%s',postname='%s',loc_id=%d,ban='%s',floor=%d,misc='%s' WHERE id=%lld";

	strQuery.sprintf(strQuery.toUtf8().data(),
					p->_MinLongitude,
					p->_MaxLongitude,
					p->_MinLatitude,
					p->_MaxLatitude,
					p->_Name.toUtf8().data(),
					p->_PostName.toUtf8().data(),
					p->_Loc_ID,
					p->_Ban.toUtf8().data(),
					p->_Floor,
					p->_Misc.toUtf8().data(),
					p->_ID);

	bSuccess = oSqlQuery.exec(strQuery);

	return bSuccess;
}

bool GDSGeoNameDBManager::DeleteGeoBuildingData(quint64 id)
{
	if (!m_Database.isOpen())
	{
		if (!m_Database.open())
		{
			GDSLogOutput(E_LOG_ERROR, "Database engine open failed at GDSGeoNameDBManager::AddGeoBuildingData");
			return false;
		}
	}

	bool bSuccess;
	QString strQuery;
	QSqlQuery oSqlQuery(m_Database);

	strQuery.sprintf("DELETE FROM tb_building WHERE id=%lld",id);

	m_Lock.lock();
	bSuccess = oSqlQuery.exec(strQuery);
	m_Lock.unlock();

	return bSuccess;
}

bool GDSGeoNameDBManager::GetGeoBuildingData(quint64 id,GeoBuildingProperty *p)
{
	if (!m_Database.isOpen())
	{
		if (!m_Database.open())
		{
			GDSLogOutput(E_LOG_ERROR, "Database engine open failed at GDSGeoNameDBManager::GetGeoBuildingData");
			return false;
		}
	}

	bool bSuccess;
	QString strQuery;
	QSqlQueryModel oSqlQueryModel;

	strQuery.sprintf("SELECT * FROM tb_building WHERE id=%lld",id);

	m_Lock.lock();
	oSqlQueryModel.setQuery(strQuery,m_Database);
	
	m_Lock.unlock();

	bSuccess = !oSqlQueryModel.lastError().isValid();

	if(!bSuccess)
		return false;

	int nRowCount = oSqlQueryModel.rowCount();

	//We must guarrant that record with given id is one in database. 
	if(nRowCount!=1)
		return false;
	
	QSqlRecord record=oSqlQueryModel.record(0);

	p->_ID=id;
	p->_MinLongitude=record.field("minlongitude").value().toFloat();
	p->_MaxLongitude=record.field("maxlongitude").value().toFloat();
	p->_MinLatitude=record.field("minlatitude").value().toFloat();
	p->_MaxLatitude=record.field("maxlatitude").value().toFloat();

	p->_Name=record.field("name").value().toString();
	p->_PostName=record.field("postname").value().toString();
	p->_Loc_ID=record.field("loc_id").value().toUInt();

	QString ban = record.field("ban").value().toString();
	if(ban.contains("|")) {
		ban = ban.mid(1, ban.length() - 2);
		ban = ban.replace("|", ",");
	}
	p->_Ban = ban;

	p->_Floor=record.field("floor").value().toUInt();
	p->_Misc=record.field("misc").value().toString();

	strQuery.sprintf("SELECT * FROM lst_location_name WHERE loc_id=%d",p->_Loc_ID);

	m_Lock.lock();
	oSqlQueryModel.setQuery(strQuery,m_Database);

	m_Lock.unlock();

	bSuccess = !oSqlQueryModel.lastError().isValid();

	if(!bSuccess)
		return false;

	record=oSqlQueryModel.record(0);
	p->_Address=record.field("full_name").value().toString();
	p->_Address+=" ";
	p->_Address+=p->_Ban;
   
	return true;
}

bool GDSGeoNameDBManager::GetGeoPostBuildingData(QList<QString>& namelist,QList<int>& idlist)
{
	namelist.clear();
	idlist.clear();

	if (!m_Database.isOpen())
	{
		if (!m_Database.open())
		{
			GDSLogOutput(E_LOG_ERROR, "Database engine open failed at GDSGeoNameDBManager::AddGeoBuildingData");
			return false;
		}
	}

	bool bSuccess;
	QString strQuery;
	QSqlQueryModel oSqlQueryModel;

	strQuery.sprintf("SELECT * FROM tb_building WHERE postname!=''");

	m_Lock.lock();
	oSqlQueryModel.setQuery(strQuery,m_Database);
	
	m_Lock.unlock();

	bSuccess = !oSqlQueryModel.lastError().isValid();

	if(!bSuccess)
		return false;

	int nRowCount = oSqlQueryModel.rowCount();

	//We must guarrant that record with given id is one in database. 
	if(nRowCount==0)
		return false;

	for(int i=0;i<nRowCount;i++)
	{
		QSqlRecord record=oSqlQueryModel.record(i);

		namelist.push_back(record.field("name").value().toString());

	    idlist.push_back(record.field("id").value().toUInt());
	}

	return true;
}

bool GDSGeoNameDBManager::GetGeoBuildingData(QList<QString>& namelist,QList<int>& idlist,QString dong,int ban)
{
	namelist.clear();
	idlist.clear();

	if (!m_Database.isOpen())
	{
		if (!m_Database.open())
		{
			GDSLogOutput(E_LOG_ERROR, "Database engine open failed at GDSGeoNameDBManager::AddGeoBuildingData");
			return false;
		}
	}

	bool bSuccess;
	QString strQuery;
	QSqlQueryModel oSqlQueryModel;

	strQuery.sprintf("SELECT * FROM tb_building WHERE dong='%s' AND ban=%d",dong.toUtf8().data(), ban);

	m_Lock.lock();
	oSqlQueryModel.setQuery(strQuery,m_Database);

	m_Lock.unlock();

	bSuccess = !oSqlQueryModel.lastError().isValid();

	if(!bSuccess)
		return false;

	int nRowCount = oSqlQueryModel.rowCount();

	//We must guarrant that record with given id is one in database. 
	if(nRowCount==0)
		return false;

	for(int i=0;i<nRowCount;i++)
	{
		QSqlRecord record=oSqlQueryModel.record(i);

		QString name=record.field("name").value().toString();

		if(name.isEmpty())
			name="NoName";

		namelist.push_back(name);

		idlist.push_back(record.field("id").value().toUInt());
	}

	return true;

}

bool GDSGeoNameDBManager::GetGeoBuildingPosintion(quint64 id,float& longi,float& lat)
{
	if (!m_Database.isOpen())
	{
		if (!m_Database.open())
		{
			GDSLogOutput(E_LOG_ERROR, "Database engine open failed at GDSGeoNameDBManager::AddGeoBuildingData");
			return false;
		}
	}

	bool bSuccess;
	QString strQuery;
	QSqlQueryModel oSqlQueryModel;

	strQuery.sprintf("SELECT * FROM tb_building WHERE id=%lld",id);

	m_Lock.lock();
	oSqlQueryModel.setQuery(strQuery,m_Database);

	m_Lock.unlock();

	bSuccess = !oSqlQueryModel.lastError().isValid();

	if(!bSuccess)
		return false;

	int nRowCount = oSqlQueryModel.rowCount();

	//We must guarrant that record with given id is one in database. 
	if(nRowCount!=1)
		return false;

	QSqlRecord record=oSqlQueryModel.record(0);


	float minLongitude=record.field("minlongitude").value().toFloat();
	float maxLongitude=record.field("maxlongitude").value().toFloat();
	float minLatitude=record.field("minlatitude").value().toFloat();
	float maxLatitude=record.field("maxlatitude").value().toFloat();

	longi=(minLongitude+maxLongitude)/2.0;
	lat=(minLatitude+maxLatitude)/2.0;
	return true;
}

//type represents one table in database
//id represents one record in the table

CGIS_Polygon* GDSGeoNameDBManager::CreateFeature(E_GDM_SUBDATA_TYPE type,int id,CGIS_Polygon*& link)
{
	if (!m_Database.isOpen())
	{
		if (!m_Database.open())
		{
			GDSLogOutput(E_LOG_ERROR, "Database engine open failed at GDSGeoNameDBManager::AddGeoBuildingData");
			return NULL;
		}
	}

	int ntableno;

	if(!GetTableInfoFromType(type,&ntableno))
		return NULL;

	QString tablename=QString::fromAscii(m_pTableList[ntableno].table_name);
	
	ShapeType shapetype=(ShapeType)m_pTableList[ntableno].shape_type;

	Q_ASSERT(shapetype==GISPolygon || shapetype==GISPath);

	CGIS_Polygon* feature=NULL;

	if(shapetype==GISPolygon)
		feature=new CGIS_Area();
	else
		feature=new CGIS_Path();

	bool bSuccess;
	QString strQuery;
	QSqlQueryModel oSqlQueryModel;

	strQuery.sprintf("SELECT * FROM %s WHERE id=%d",m_pTableList[ntableno].table_name,id);

	m_Lock.lock();
	oSqlQueryModel.setQuery(strQuery,m_Database);

	m_Lock.unlock();

	bSuccess = !oSqlQueryModel.lastError().isValid();

	Q_ASSERT(bSuccess==true);

	int nRowCount = oSqlQueryModel.rowCount();

	Q_ASSERT(nRowCount==1);
	
	QSqlRecord record=oSqlQueryModel.record(0);

	QByteArray arr=record.field("points").value().toByteArray();

	QDataStream in(&arr,QIODevice::ReadOnly);
	
	CGDM_Location3D pt;
	while(!in.atEnd())
	{
		in>>pt.m_tX>>pt.m_tY;
		feature->AddPoint(pt);
	}	

	ST_DBTABLEINFO* tabledef=GetTableDef(type);

	feature->m_sPen.color=tabledef->line_color;
	feature->m_sPen.width=tabledef->linewidth;

	feature->m_sBrush.color=tabledef->fillcolor;

	feature->Refresh();


	if(type==E_GDM_SUBDATA_TYPE_NAME_POLUTANT)
	{
		pt.m_tX=record.field("accidentlong").value().toDouble()*gdm_DEGTORAD;
		pt.m_tY=record.field("accidentlat").value().toDouble()*gdm_DEGTORAD;

		if(pt.m_tX!=0 && pt.m_tY!=0)
		{
			link=new CGIS_Path();

			link->AddPoint(pt);

			pt.m_tX=record.field("longitude").value().toDouble()*gdm_DEGTORAD;
			pt.m_tY=record.field("latitude").value().toDouble()*gdm_DEGTORAD;

			
			link->AddPoint(pt);
			link->Refresh();
		}
		else
			link=NULL;
	}
	else
		link=NULL;

	return feature;
}


ST_DBTABLEINFO* GDSGeoNameDBManager::GetTableDef(E_GDM_SUBDATA_TYPE type)
{
	int index=0;

	bool ret=GetTableInfoFromType(type,&index);

	Q_ASSERT(ret==true);
	if(ret)
		return &m_pTableList[index];
	return NULL;
}

QSqlRecord GDSGeoNameDBManager::GetTableData(int type)
{
	bool isopen=m_Database.isOpen();

	Q_ASSERT(isopen);

	bool bSuccess;
	QString strQuery;
	QSqlQueryModel oSqlQueryModel;

	strQuery.sprintf("SELECT * FROM tb_type WHERE type=%d",type);

	oSqlQueryModel.setQuery(strQuery,m_Database);

	bSuccess = !oSqlQueryModel.lastError().isValid();

	Q_ASSERT(bSuccess==true);
	
	int nRowCount = oSqlQueryModel.rowCount();

	Q_ASSERT(nRowCount==1);

	QSqlRecord record=oSqlQueryModel.record(0);

	return record;
}

QSqlRecord GDSGeoNameDBManager::GetTableRecord(E_GDM_SUBDATA_TYPE type,int id)
{
	ST_DBTABLEINFO *tabledef=GetTableDef(type);

	bool bSuccess;
	QString strQuery;
	QSqlQueryModel oSqlQueryModel;

	strQuery.sprintf("SELECT * FROM %s WHERE id=%d",tabledef->table_name,id);

	oSqlQueryModel.setQuery(strQuery,m_Database);

	bSuccess = !oSqlQueryModel.lastError().isValid();

	Q_ASSERT(bSuccess==true);

	int nRowCount = oSqlQueryModel.rowCount();

	Q_ASSERT(nRowCount==1);

	QSqlRecord record=oSqlQueryModel.record(0);

	return record;
}