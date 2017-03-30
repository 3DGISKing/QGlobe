#ifndef SEARCHTHREAD_H
#define SEARCHTHREAD_H

#include <QThread>
#include <QMutex>
#include "../gdem_common/gdem_protocol.h"
#include "../gdem_common/gdem_config.h"
#include "GDM_Array.h"

//search thread state
#define STS_IDLE		0
#define STS_NEW			1
#define STS_RECEIVED	2
#define STS_ANALYZE		3
#define STS_STOP		4
#define STS_STOPED		5

enum SEARCH_THREAD_RESULT
{
	SEARCH_THREAD_ERROR_NONE,
	SEARCH_THREAD_ERROR_MEMORY_FAIL,
	SEARCH_THREAD_ERROR_SERVER,
	SEARCH_THREAD_ERROR_NODATA,
};

#ifdef GEONAME_MODIFY_TEMP
#define MAX_SEARCH_RESULT_CNT	1500
#else
#define MAX_SEARCH_RESULT_CNT	200
#endif

typedef struct  
{
	int		total_count;
	int		start_index;
	int		end_index;
	int		reserved;
} SEARCH_INDEX_INFO_HEADER;

class CGDM_SearchNode
{
public:
	CGDM_SearchNode(void);
	~CGDM_SearchNode(void);
	CGDM_SearchNode(const CGDM_SearchNode& a_cOther)
	{
		itemkey = a_cOther.itemkey;
		idCnt = a_cOther.idCnt;
		memcpy(idList, a_cOther.idList, sizeof(ST_RECORD_ID) * MAX_SEARCH_RESULT_CNT);
	}
public:
	bool operator<(const CGDM_SearchNode &a_cOther) const		{return Compare(a_cOther) < 0;}
	bool operator>(const CGDM_SearchNode &a_cOther) const		{return Compare(a_cOther) > 0;}
	bool operator==(const CGDM_SearchNode &a_cOther) const	{return Compare(a_cOther) == 0;}
	bool operator!=(const CGDM_SearchNode &a_cOther) const	{return Compare(a_cOther) != 0;}
	CGDM_SearchNode& operator=(const CGDM_SearchNode& a_cOther)
	{
		itemkey = a_cOther.itemkey;
		idCnt = a_cOther.idCnt;
		memcpy(idList, a_cOther.idList, sizeof(ST_RECORD_ID) * MAX_SEARCH_RESULT_CNT);
		return *this;
	}
	int		Compare(const CGDM_SearchNode &a_cOther) const;
public:
	int				itemkey;
	int				idCnt;
	ST_RECORD_ID	idList[MAX_SEARCH_RESULT_CNT];
};

typedef gdm_array<CGDM_SearchNode>	CGDM_SearchNodeArr;	

class SearchThread : public QThread
{
	Q_OBJECT

public:
	SearchThread();
	~SearchThread();

	QString& GetKeyStr()
	{	return m_key; }
	int		GetItemKey() {return m_itemkey;}

	int GetState();
	void SetState(int state);
	void SetKey(const QString &key, int start, int itemkey);
	void GetResponseData(int *res, void **data);
	void WaiteStop();

	void AddSearchNode(int itemkey);
	void RemoveAllSearchNode();

protected:
	unsigned char *m_response;

	QMutex		m_mutex;
	int			m_state;
	QString		m_key;
	int			m_start;
	int			m_res;

	int			m_itemkey;
	int			m_nTotalIDCnt;
	int			m_nCurIndex;
	ST_RECORD_ID		m_sIDList[MAX_SEARCH_RESULT_CNT];
	CGDM_SearchNodeArr	m_searchArray;


	virtual void run();

	int RequestData(const QString &key, int start);
	int RequestIDList(const QString &key, int req_index);
	int ReceiveIDList();
	int RequestSearchData(int start);
};

#endif // SEARCHTHREAD_H
