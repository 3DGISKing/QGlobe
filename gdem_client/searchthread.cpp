#include "gdemclient.h"
#include "searchthread.h"
#include "treeitem.h"
#include "guiuty.h"
#include "proxy.h"

// for get camera state
#include "GDM_DataMgr.h"
#include "GDM_Camera.h"
#include "GDM_PyramidMgr.h"
// end


#define MAX_REQ_LEVEL	17

void gdm_Get_CameraInfo(ST_REQUEST_SEARCH_IDLIST *data)
{
	CGDM_DataMgr *pMgr = gdm_GetDataMgr();
	if(data == NULL)
		return;
	if(pMgr && pMgr->m_pCamera)
	{
		data->level		= pMgr->m_pPyramidMgr->GetMaxLevel();

		data->level += 4;
		if(data->level > MAX_REQ_LEVEL)
			data->level = MAX_REQ_LEVEL;

		data->bottom	= (pMgr->m_pCamera->m_dMinLati) * 180 / PAI;;
		data->top		= (pMgr->m_pCamera->m_dMaxLati) * 180 / PAI;;

		if(fabs(pMgr->m_pCamera->m_dMinLong1 + PAI) < 0.001)
		{
			data->left = (pMgr->m_pCamera->m_dMinLong2) * 180 / PAI;
			data->right = (pMgr->m_pCamera->m_dMaxLong1 + 2 * PAI) * 180 / PAI;
		}
		else
		{
			data->left = (pMgr->m_pCamera->m_dMinLong1) * 180 / PAI;
			data->right = (pMgr->m_pCamera->m_dMaxLong1) * 180 / PAI;
		}
	}
}

int CGDM_SearchNode::Compare(const CGDM_SearchNode &a_cOther) const
{
	int ret = 0;

	// compare type
	if (itemkey < a_cOther.itemkey)
		return -1;

	if (itemkey > a_cOther.itemkey)
		return 1;

	return ret;
}

CGDM_SearchNode::CGDM_SearchNode(void)
{
	itemkey = 0;
	idCnt = 0;
	memset(idList, 0x00, sizeof(ST_RECORD_ID) * MAX_SEARCH_RESULT_CNT);
}

CGDM_SearchNode::~CGDM_SearchNode(void)
{
}


#define SEARCH_THREAD_SLEEP	20

SearchThread::SearchThread()
{
	m_state = STS_IDLE;
	m_response = NULL;
	m_nTotalIDCnt = 0;
	m_nCurIndex = 0;
}

SearchThread::~SearchThread()
{
	m_searchArray.clear();
}

int SearchThread::GetState()
{
	int state = 0;

	m_mutex.lock();
	state = m_state;
	m_mutex.unlock();

	return state;
}

void SearchThread::SetState(int state)
{
	m_mutex.lock();
	m_state = state;
	m_mutex.unlock();
}

void SearchThread::SetKey(const QString &key, int start, int itemkey)
{
	CGDM_SearchNode searchNode;
	int nIndex;

	m_mutex.lock();
	m_nTotalIDCnt = 0;
	if (itemkey)
	{
		searchNode.itemkey = itemkey;
		nIndex = m_searchArray.binary_search(searchNode);
		if (nIndex != -1)	/* found it */
		{
			m_nTotalIDCnt = m_searchArray[nIndex].idCnt;
			memcpy(&m_sIDList, m_searchArray[nIndex].idList, sizeof(ST_RECORD_ID) * m_nTotalIDCnt);
		}
	}
	m_key = key;
	m_start = start;
	m_itemkey = itemkey;
	m_mutex.unlock();
}

void SearchThread::GetResponseData(int *res, void **data)
{
	m_mutex.lock();
	*res = m_res;
	*data = m_response;
	m_mutex.unlock();
}

void SearchThread::WaiteStop()
{
	if (GetState() != STS_STOPED)
	{
		SetState(STS_STOP);

		do
		{
			msleep(SEARCH_THREAD_SLEEP);
		}
		while (GetState() != STS_STOPED);
	}
}

void SearchThread::run()
{
	while (1)
	{
		int state = GetState();
		if (state == STS_STOP)
			break;

		if (state == STS_NEW)
		{
			RequestData(m_key, m_start);
			SetState(STS_RECEIVED);
		}

		msleep(SEARCH_THREAD_SLEEP);
	}

	if (m_response)
		free(m_response);
	m_response = NULL;

	SetState(STS_STOPED);
}

int SearchThread::RequestData(const QString &key, int start)
{
#if 0
	// request buffer
	int totalsize = sizeof(ST_PACKET_HEADER) - sizeof(unsigned char) * 4
					+ sizeof(ST_REQUEST_HEADER) - sizeof(unsigned char) * 4
					+ sizeof(ST_REQUEST_GEONAMESEARCH) - sizeof(unsigned short) * 2
					+ sizeof(unsigned short) * (key.count() + 1);
	ST_PACKET_HEADER *request =(ST_PACKET_HEADER * )malloc(totalsize);
	if (request == NULL)
	{
		m_res = THREAD_ERROR;
		return m_res;
	}
	memset(request,0x00, totalsize);
	request->bytes = totalsize;
	ST_REQUEST_HEADER *header = (ST_REQUEST_HEADER*)request->buffer;
		header->type = E_REQUEST_TYPE_GEONAMELIST;
	ST_REQUEST_GEONAMESEARCH *data = (ST_REQUEST_GEONAMESEARCH*)header->body;
	data->level = 0;
	data->left = -180;
	data->top = 180;
	data->right = 180;
	data->bottom = -180;
	data->start_index = start;
	data->end_index = start + TI_MAX_SEARCH_COUNT -1;
	data->length = (key.count() + 1) * sizeof(unsigned short) ;
	wchar_t tempkey[254];
	key.toWCharArray(tempkey);
	tempkey[key.count()] = 0;

	gu_wchar2ushort(tempkey, data->key_string, key.count());

	// response buffer
	if (m_response == NULL)
		m_response = (unsigned char *)malloc(PACKET_MAX_BUFFER_SIZE);

	if (m_response == NULL)
	{
		m_res = THREAD_ERROR;
		free(request);
		return m_res;
	}

	memset(m_response,0x00, PACKET_MAX_BUFFER_SIZE);

	ST_PACKET_HEADER *response = (ST_PACKET_HEADER *)m_response;
	response->bytes = PACKET_MAX_BUFFER_SIZE;

	DataProxy *datasrv = g_app->GetDataProxy();
	if (datasrv)
		m_res = datasrv->SendMessage(PM_GET_SEARCHRESULT, (PrxParam)request, (PrxParam)response);
	else
		m_res = THREAD_ERROR;
	qDebug("SEARCH ERROR:result =%d", m_res);

	free(request);
	
	return m_res;
#else
	int req_index;

	m_res = SEARCH_THREAD_ERROR_NONE;

	if (m_nTotalIDCnt)	/* Record index is already exist */
	{
		m_res = RequestSearchData (start);
	}
	else
	{
		int res;

		for (req_index = 0;;req_index++)
		{
			if (m_res = RequestIDList (key, req_index))
				break;

			if ((res = ReceiveIDList()) < 0)
			{
				m_res = SEARCH_THREAD_ERROR_SERVER;
				break;
			}
			else if (res == 1)
				break;
		}
		if (m_res != SEARCH_THREAD_ERROR_NONE)
			return m_res;

		/* No Data Found */
		if ((m_nTotalIDCnt == 0) || (m_nTotalIDCnt <= start))
		{
			m_res = SEARCH_THREAD_ERROR_NODATA;
			return m_res;
		}

		m_res = RequestSearchData(start);
	}

	return m_res;
#endif
}


int SearchThread::RequestIDList(const QString &key, int req_index)
{
	int res = 0;

	// request buffer
	int totalsize = sizeof(ST_PACKET_HEADER) - sizeof(unsigned char) * 4
		+ sizeof(ST_REQUEST_HEADER) - sizeof(unsigned char) * 4
		+ sizeof(ST_REQUEST_GEONAMESEARCH) - sizeof(unsigned char) * 4 
		+ sizeof(ST_REQUEST_SEARCH_IDLIST) - sizeof(unsigned short) * 2
		+ sizeof(unsigned short) * (key.count() + 1);

	ST_PACKET_HEADER *request =(ST_PACKET_HEADER * )malloc(totalsize);
	if (request == NULL)
		return  SEARCH_THREAD_ERROR_MEMORY_FAIL;

	memset(request,0x00, totalsize);
	request->bytes = totalsize;
	ST_REQUEST_HEADER *header = (ST_REQUEST_HEADER*)request->buffer;
	header->type = E_REQUEST_TYPE_GEONAMELIST;
	ST_REQUEST_GEONAMESEARCH *search_header = (ST_REQUEST_GEONAMESEARCH*)header->body;
	search_header->type = E_SEARCH_TYPE_ID;
	ST_REQUEST_SEARCH_IDLIST *data = (ST_REQUEST_SEARCH_IDLIST*)search_header->reguest;

	//data->level = 0;
	//data->left = -180;
	//data->top = 180;
	//data->right = 180;
	//data->bottom = -180;
	gdm_Get_CameraInfo(data);

	data->req_index = req_index;
	data->length = (key.count() + 1) * sizeof(unsigned short) ;
	wchar_t tempkey[254];
	key.toWCharArray(tempkey);
	tempkey[key.count()] = 0;

	gu_wchar2ushort(tempkey, data->key_string, key.count());

	// response buffer
	if (m_response == NULL)
		m_response = (unsigned char *)malloc(PACKET_MAX_BUFFER_SIZE);

	if (m_response == NULL)
	{
		free(request);
		return SEARCH_THREAD_ERROR_MEMORY_FAIL;
	}

	memset(m_response,0x00, PACKET_MAX_BUFFER_SIZE);

	ST_PACKET_HEADER *response = (ST_PACKET_HEADER *)m_response;
	response->bytes = PACKET_MAX_BUFFER_SIZE;

	DataProxy *datasrv = g_app->GetDataProxy();
	if (datasrv)
	{
		res = datasrv->SendMessage(PM_GET_SEARCHRESULT, (PrxParam)request, (PrxParam)response);
		if (res)
			res = SEARCH_THREAD_ERROR_SERVER;
	}
	else
		res = SEARCH_THREAD_ERROR_SERVER;
#ifdef DEBUG
	qDebug("SEARCH ERROR:result =%d", res);
#endif

	free(request);

	return res;
}

/* 
Return Value
	 1 : finished
	 0 : normal
	-1 : error 
*/
int SearchThread::ReceiveIDList()
{
	if (m_response == NULL)
		return -1;

	ST_PACKET_HEADER *response = (ST_PACKET_HEADER *)m_response;
	ST_RESPONSE_HEADER *responseHeader = (ST_RESPONSE_HEADER *)response->buffer;
	if (responseHeader->result != E_RESPONSE_ERROR_NONE)
		return -1;

	ST_RESPONSE_GEONAMESEARCH* search_header = (ST_RESPONSE_GEONAMESEARCH*)responseHeader->body;
	if (search_header->type != E_SEARCH_TYPE_ID)
		return -1;

	ST_RESPONSE_SEARCH_IDLIST* responseData = (ST_RESPONSE_SEARCH_IDLIST*)search_header->response;

	for (int i = 0; i < responseData->count; i++)
	{
		memcpy(&m_sIDList[m_nTotalIDCnt], &responseData->id_list[i], sizeof(ST_RECORD_ID));
		m_nTotalIDCnt++;
		if (m_nTotalIDCnt == MAX_SEARCH_RESULT_CNT)
			return 1;
	}

	if (responseData->finished)
		return 1;

	return 0;
}

int SearchThread::RequestSearchData(int start)
{
	int nCnt, res;

	if (m_nTotalIDCnt < start + TI_MAX_SEARCH_COUNT)
		nCnt = m_nTotalIDCnt - start;
	else
		nCnt = TI_MAX_SEARCH_COUNT;

	// request buffer
	int totalsize = sizeof(ST_PACKET_HEADER) - sizeof(unsigned char) * 4
		+ sizeof(ST_REQUEST_HEADER) - sizeof(unsigned char) * 4
		+ sizeof(ST_REQUEST_GEONAMESEARCH) - sizeof(unsigned char) * 4 
		+ sizeof(ST_REQUEST_SEARCH_DATA) - sizeof(ST_RECORD_ID)
		+ sizeof(ST_RECORD_ID) * nCnt;

	ST_PACKET_HEADER *request =(ST_PACKET_HEADER * )malloc(totalsize);
	if (request == NULL)
		return SEARCH_THREAD_ERROR_MEMORY_FAIL;

	memset(request,0x00, totalsize);
	request->bytes = totalsize;
	ST_REQUEST_HEADER *header = (ST_REQUEST_HEADER*)request->buffer;
	header->type = E_REQUEST_TYPE_GEONAMELIST;
	ST_REQUEST_GEONAMESEARCH *search_header = (ST_REQUEST_GEONAMESEARCH*)header->body;
	search_header->type = E_SEARCH_TYPE_DATA;
	ST_REQUEST_SEARCH_DATA *data = (ST_REQUEST_SEARCH_DATA*)search_header->reguest;

	data->count = nCnt;
	for (int i = 0; i < nCnt; i++)
	{
		memcpy(&data->id_list[i], &m_sIDList[i+start], sizeof(ST_RECORD_ID));
	}

	// response buffer
	if (m_response == NULL)
		m_response = (unsigned char *)malloc(PACKET_MAX_BUFFER_SIZE);

	if (m_response == NULL)
	{
		free(request);
		return SEARCH_THREAD_ERROR_MEMORY_FAIL;
	}

	memset(m_response,0x00, PACKET_MAX_BUFFER_SIZE);

	SEARCH_INDEX_INFO_HEADER *info_header = (SEARCH_INDEX_INFO_HEADER*)m_response;
	info_header->total_count = m_nTotalIDCnt;
	info_header->start_index = start;
	info_header->end_index	 = start + nCnt - 1;

	ST_PACKET_HEADER *response = (ST_PACKET_HEADER *)(m_response + sizeof(SEARCH_INDEX_INFO_HEADER));
	response->bytes = PACKET_MAX_BUFFER_SIZE;

	DataProxy *datasrv = g_app->GetDataProxy();
	if (datasrv)
	{
		res = datasrv->SendMessage(PM_GET_SEARCHRESULT, (PrxParam)request, (PrxParam)response);
		if (res)
			res = SEARCH_THREAD_ERROR_SERVER;
	}
	else
		res = SEARCH_THREAD_ERROR_SERVER;
#ifdef DEBUG
	qDebug("SEARCH ERROR:result =%d", res);
#endif

	free(request);

	return res;
}

void SearchThread::AddSearchNode(int itemkey)
{
	CGDM_SearchNode searchNode;

	if (m_nTotalIDCnt <= TI_MAX_SEARCH_COUNT)
		return;

	searchNode.itemkey = itemkey;
	searchNode.idCnt   = m_nTotalIDCnt;
	memcpy(searchNode.idList, m_sIDList, sizeof(ST_RECORD_ID) * m_nTotalIDCnt);

	m_searchArray.push_back(searchNode);
	//m_searchArray.sort();
}

void SearchThread::RemoveAllSearchNode()
{
	m_searchArray.clear();
}