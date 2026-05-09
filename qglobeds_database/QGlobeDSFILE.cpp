#include "QGlobeDSFile.h"
#include <QtCore/QFile>
#include "../qglobeds_common/QGlobeDSLog.h"
#include "../qglobeds_common/QGlobeDSConfigMgr.h"
#include "../qglobeds_common/QGlobeDSCommon.h"
#include "../qglobe_libmain/QGlobe_DataMgr.h"
#include "../qglobe_libmain/Layer.h"

//����������� ʭ���� ���뼬�� �����溷 ��궦 �в���
bool GetGeoDataFromFile(IN E_QGlobe_MAINDATA_TYPE a_eMainDataType, 
						IN E_QGlobe_SUBDATA_TYPE a_eSubDataType, 
						IN unsigned int a_nLevel,
						IN unsigned int a_nColumn, 
						IN unsigned int a_nRow,
						OUT ST_GEODATA_COMMON* a_pResponse, 
						OUT unsigned int* a_pBytes, 
						OUT E_RESPONSE_ERROR_TYPE* a_pErrorCode)
{
	QString tFileName;

	if (!GetFileName(a_eMainDataType, a_eSubDataType, a_nLevel, a_nColumn, a_nRow, tFileName))
	{
		*a_pErrorCode = E_RESPONSE_ERROR_FILENAME;
		QGlobeDSLogOutput(E_LOG_ERROR, "file name is unknown at GetGeoDataFromFile! [type : %d]", (int)a_eMainDataType);
		return false;
	}

	if (!QFile::exists(tFileName))
	{
		a_pResponse->bytes = 0;
		*a_pBytes = sizeof(ST_GEODATA_COMMON) - sizeof(unsigned char)*4;
		*a_pErrorCode=E_RESPONSE_ERROR_FILE_NOEXIST;
		return false;
	}

	QFile* newFile = new QFile(tFileName);

	if (newFile == NULL)
	{
		QGlobeDSLogOutput(E_LOG_ERROR, "new QFile failed at GetGeoDataFromFile.[file : %s]", tFileName.toUtf8().data());
		*a_pBytes = sizeof(ST_GEODATA_COMMON) - sizeof(unsigned char)*4;
		return false;
	}

	unsigned int tFileSize = newFile->size();

	if (tFileSize==0)
	{
		delete newFile;

		QGlobeDSLogOutput(E_LOG_ERROR, "File size is zero at GetGeoDataFromFile.[file : %s]", tFileName.toUtf8().data());
		return false;
	}

	if ( !newFile->open(QIODevice::ReadOnly) )
	{
		delete newFile;

		QGlobeDSLogOutput(E_LOG_ERROR, "File open failed at GetGeoDataFromFile.[file : %s]", tFileName.toUtf8().data());
		return false;
	}

	unsigned char* lpData = NULL;

	lpData = newFile->map(0, tFileSize);
	if (lpData != NULL)
	{
		memcpy(a_pResponse->buffer, lpData, tFileSize);
		newFile->unmap(lpData);
	}
	
	newFile->close();
	delete newFile;

	a_pResponse->bytes = tFileSize;
	a_pResponse->compress_type = GetCompressType(a_eMainDataType);

	*a_pBytes = sizeof(ST_GEODATA_COMMON) - sizeof(unsigned char)*4 + tFileSize;
	return true;
}

bool GetFileName(IN E_QGlobe_MAINDATA_TYPE a_eMainDataType,
				 IN E_QGlobe_SUBDATA_TYPE a_eSubDataType,
				 IN unsigned int a_nLevel,
				 IN unsigned int a_nColumn,
				 IN unsigned int a_nRow,
				 OUT QString& a_strFileName)
{
	unsigned int m = a_nColumn / TILE_ROW_COUNT_IN_BLOCK;
	unsigned int n = a_nRow / TILE_ROW_COUNT_IN_BLOCK;

	switch (a_eSubDataType)
	{
		case E_QGlobe_SUBDATA_TYPE_3D_BUILDING:
			if(a_eSubDataType==E_QGlobe_SUBDATA_TYPE_3D_BUILDING)
				a_strFileName=g_ServerConfig.GetFileDBPath()+a_strFileName.sprintf(MESH_BUILDING_FILE_PATH_FMT, a_nLevel, m, n,a_nLevel,a_nColumn,a_nRow, a_nLevel);
			return true;
		case E_QGlobe_SUBDATA_TYPE_BUILDING_SIMPLEBUILDING:
			if(a_eSubDataType==E_QGlobe_SUBDATA_TYPE_BUILDING_SIMPLEBUILDING)
				a_strFileName=g_ServerConfig.GetFileDBPath()+a_strFileName.sprintf(SIMPLEBUILDING_FILE_PATH, a_nLevel, m, n,a_nLevel,a_nColumn,a_nRow, a_nLevel);
			return true;
	default:
		break;
	}

	Layer* layer=g_pGDMDataMgr->m_LayerHash[a_eSubDataType];
	Q_ASSERT(layer!=NULL);

	QString datapath=layer->GetDataPath();

	if(a_nLevel<8)
		a_strFileName = g_ServerConfig.GetFileDBPath() + "/"+datapath+a_strFileName.sprintf(RASTER_FILE_PATH_FMTEX, a_nLevel, a_nColumn,a_nRow, a_nLevel);
	else
		a_strFileName = g_ServerConfig.GetFileDBPath() + "/"+datapath+a_strFileName.sprintf(RASTER_FILE_PATH_FMT, a_nLevel, m, n,a_nLevel,a_nColumn,a_nRow, a_nLevel);

	return true;

}
