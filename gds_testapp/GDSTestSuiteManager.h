#ifndef _GDSTESTSUITE_MNGR_H_
#define _GDSTESTSUITE_MNGR_H_

#include <QList>
#include <QString>

class GDSTestSuiteManager
{
public:
	GDSTestSuiteManager();
	~GDSTestSuiteManager();

public:
	QString GetFlowTestResult();
	QString GetSyncTestResult();
	QString GetAsyncTestResult();

private:
	void AppendTestStr(QString &a_strTest, const char* a_szFuncName, bool a_bResult, QString a_strComment = QString());

	QString			m_strRet;
	QList<QString> *m_pAsyncFuncList;
};

#endif // _GDSTESTSUITE_MNGR_H_
