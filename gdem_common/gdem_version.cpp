#include "gdem_version.h"

#include <QString>
#include <QStringList>

class GDEMVersionMgr
{
public:
	GDEMVersionMgr();
	virtual ~GDEMVersionMgr();

	int			m_nMainVersion;
	int			m_SubVersionCount;
	int*		m_SubVersionList;
};


GDEMVersionMgr::GDEMVersionMgr()
{
	m_nMainVersion = (GDEM_VERSION_MAJOR * 0x01000000)
					+ (GDEM_VERSION_MINOR * 0x00010000)
					+ (GDEM_VERSION_MICRO * 0x00000100)
					+ (GDEM_VERSION_EXTRA * 0x00000001);

	QString tVerDesc(GDEM_SUB_VERSION);
	QStringList fields = tVerDesc.split('.');

	m_SubVersionCount = fields.count();

	m_SubVersionList = new int[m_SubVersionCount];
	for (int i=0; i<m_SubVersionCount; i++)
	{
		m_SubVersionList[i] = fields.takeFirst().toInt();
	}
}

GDEMVersionMgr::~GDEMVersionMgr()
{
	delete[] m_SubVersionList;
}


static GDEMVersionMgr g_VersionMgr;



// version management functions
//////////////////////////////////////////////////////////////////////

// use at server
int GetMainVersion()
{
	return g_VersionMgr.m_nMainVersion;
}

int GetSubVersionByGeoDataType(E_GDM_MAINDATA_TYPE a_eGeoDataType)
{
	if (a_eGeoDataType >= (g_VersionMgr.m_SubVersionCount-1))
		return 0;

	return g_VersionMgr.m_SubVersionList[a_eGeoDataType+1];
}

int GetSubVersionBySearchName()
{
	return g_VersionMgr.m_SubVersionList[0];
}

// use at client
E_VERSION_MISMATCH_TYPE CheckMainVersion(int a_nVersion)
{
	int tClientVersion = g_VersionMgr.m_nMainVersion;
	int tServerVersion = a_nVersion;

	if ((tClientVersion & 0xFF000000) != (tServerVersion & 0xFF000000))
		return E_VERSION_MISMATCH_MAJOR;
	if ((tClientVersion & 0x00FF0000) != (tServerVersion & 0x00FF0000))
		return E_VERSION_MISMATCH_MINOR;
	if ((tClientVersion & 0x0000FF00) != (tServerVersion & 0x0000FF00))
		return E_VERSION_MISMATCH_MICRO;
	if ((tClientVersion & 0x000000FF) != (tServerVersion & 0x000000FF))
		return E_VERSION_MISMATCH_EXTRA;

	return E_VERSION_EQUAL;
}

bool IsEqualSubVersionByGeoDataType(E_GDM_MAINDATA_TYPE a_eGeoDataType, int a_nSubVersion)
{
	if (a_eGeoDataType >= (g_VersionMgr.m_SubVersionCount-1))
		return 0;

	int tSubVersion = g_VersionMgr.m_SubVersionList[a_eGeoDataType+1];

	return (a_nSubVersion == tSubVersion);
}

bool IsEqualSubVersionBySearchName(int a_nSubVersion)
{
	return (a_nSubVersion == g_VersionMgr.m_SubVersionList[0]);
}