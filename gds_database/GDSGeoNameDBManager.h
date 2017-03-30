#ifndef __GDSGEONAMEDBMANAGER_H__
#define __GDSGEONAMEDBMANAGER_H__

#include <QtCore/QString>
#include <QtCore/QDate>
#include <QtCore/QMutex>

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlTableModel>

#include "../gdem_common/gdem_protocol.h"
#include "../gds_common/GDSType.h"
#include "../gdem_libmain/Building.h"

class CGIS_Polygon;

enum ShapeType
{
	PlaceMark=0,
	GISPoint=1,
	GISPath=2,
	GISPolygon=3
};

#define TABLE_NAME_LENGTH 50

typedef struct tagDBTABLEINFO
{
	int		   id;
	int		   type;
	ShapeType  shape_type;
	int		   start_level;
	int		   end_level;
	int		   record_count;
	char	   table_name[TABLE_NAME_LENGTH];

	unsigned int        line_color;
	unsigned int        fillcolor;
	float      linewidth;

} ST_DBTABLEINFO;

class GDSGeoNameDBManager
{
public: 
	GDSGeoNameDBManager();
	~GDSGeoNameDBManager();

public:
	bool	Initialize();
	void	Release();

	bool GetGeoNameRecordData(IN ST_REQUEST_GEONAMESEARCH* a_pRequest, 
							  OUT ST_RESPONSE_GEONAMESEARCH* a_pResponse, 
							  OUT unsigned int* a_pBytes, 
							  OUT E_RESPONSE_ERROR_TYPE* a_pErrorCode);

	bool GetGeoNameListInTile(IN E_GDM_SUBDATA_TYPE a_eSubDataType,
							IN unsigned int a_nLevel, 
							IN unsigned int a_nColumn, 
							IN unsigned int a_nRow, 
							OUT ST_GEODATA_COMMON* a_pResponse, 
							OUT unsigned int* a_pBytes, 
							OUT E_RESPONSE_ERROR_TYPE* a_pErrorCode);

	bool AddGeoNameData(IN ST_ADDNAME_DATA* a_pCommand, 
							OUT ST_RESPONSE_DBCOMMAND* a_pResponse, 
							OUT E_RESPONSE_ERROR_TYPE* a_pErrorCode);

	bool UpdateGeoNameData(IN ST_UPDATENAME_DATA* a_pCommand, 
							OUT ST_RESPONSE_DBCOMMAND* a_pResponse, 
							OUT E_RESPONSE_ERROR_TYPE* a_pErrorCode);

	bool DeleteGeoNameData(IN ST_DELETENAME_DATA* a_pCommand, 
							OUT ST_RESPONSE_DBCOMMAND* a_pResponse, 
							OUT E_RESPONSE_ERROR_TYPE* a_pErrorCode);

	bool AddGeoBuildingData(GeoBuildingProperty* p);
	bool ExistGeoBuildingData(qint64 id);
	bool UpdateGeoBuildingData(GeoBuildingProperty* p);
	bool DeleteGeoBuildingData(quint64 id);
	bool GetGeoBuildingData(quint64 id,GeoBuildingProperty *p);
	bool GetGeoPostBuildingData(QList<QString>& namelist,QList<int>& idlist);
	bool GetGeoBuildingData(QList<QString>& namelist,QList<int>& idlist,QString dong,int ban);
	bool GetGeoBuildingPosintion(quint64 id,float& longi,float& lat);
	CGIS_Polygon* CreateFeature(E_GDM_SUBDATA_TYPE type,int id,CGIS_Polygon*& p);
	ST_DBTABLEINFO* GetTableDef(E_GDM_SUBDATA_TYPE type);

	QSqlDatabase& GetDataBase(){return m_Database;};
	QSqlRecord GetTableData(int type);
	QSqlRecord GetTableRecord(E_GDM_SUBDATA_TYPE type,int id);

private:
	bool	connect_mysql();
	void	disconnect_mysql();

	bool	InitTableData();
	void	ReleaseTableData();

	void GetCoordinateByLevel(IN unsigned int a_nLevel, 
							  IN unsigned int a_nColumn, 
							  IN unsigned int a_nRow, 
							  OUT double* a_pdLeftLongitude,
							  OUT double* a_pdRightLongitude,
							  OUT double* a_pdTopLatitude,
							  OUT double* a_pdBottomLatitude);

	//void EditMainQueryString(IN ST_REQUEST_GEONAMESEARCH* a_pRequest, 
	//						 OUT QString& a_strQuery);

	bool EditQueryStringByLevel(IN E_GDM_SUBDATA_TYPE a_eSubDataType,
								IN unsigned int a_nLevel,
								IN double a_nLeftLongitude, 
								IN double a_nRightLongitude,
								IN double a_nTopLatitude,
								IN double a_nBottomLatitude,
								OUT QString& a_strQuery);

	bool EditAddQueryString(IN ST_GEODATA_NAME* a_pstGeoNameData, OUT QString& a_strQuery);
	bool EditUpdateQueryString(IN ST_UPDATENAME_DATA* a_pstUpdateNameData, OUT QString& a_strQuery);
	bool EditDeleteQueryString(IN ST_DELETENAME_DATA* a_pstDelNameData, OUT QString& a_strQuery);
	bool EditGetMaxIdQueryString(IN ST_GEODATA_NAME* a_pstGeoNameData, OUT QString& a_strQuery);

	//////////////////////////////////////////////////////////////////////////
	int GetRecordCount(IN char* a_ptTableName);
	bool GetGeoIdList(IN ST_REQUEST_SEARCH_IDLIST* a_pRequest, 
						OUT ST_RESPONSE_SEARCH_IDLIST* a_pResponse, 
						OUT unsigned int* a_pBytes, 
						OUT E_RESPONSE_ERROR_TYPE* a_pErrorCode);


	bool GetGeoNameList(IN ST_REQUEST_SEARCH_DATA* a_pRequest, 
						OUT ST_GEODATA_NAMELIST* a_pResponse, 
						OUT unsigned int* a_pBytes, 
						OUT E_RESPONSE_ERROR_TYPE* a_pErrorCode);


	bool MakeQueryInSight(IN ST_REQUEST_SEARCH_IDLIST* a_pRequest, OUT QString& a_strQuery);
	void MakeQueryOutSight(IN ST_REQUEST_SEARCH_IDLIST* a_pRequest, OUT bool* a_pFinished, OUT int* a_pUnionTables , OUT QString& a_strQuery);
	bool MakeQueryNameList(IN ST_REQUEST_SEARCH_DATA* a_pRequest, OUT int* a_pUnionTables, OUT QString& a_strQuery);
	bool GetTableInfoFromType(IN int a_nType, OUT int* a_ptbleNo);


	void EncodeKeyword(INOUT QString& a_strKeyword);
	//////////////////////////////////////////////////////////////////////////


private:
	QSqlDatabase		m_Database;

	QMutex				m_Lock;

	int					m_nTableCount;
	ST_DBTABLEINFO*		m_pTableList;
};

#endif // __GDSGEONAMEDBMANAGER_H__
