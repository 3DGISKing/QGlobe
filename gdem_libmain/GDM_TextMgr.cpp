#include "stable.h"
#include "GDM_TextMgr.h"
#include "GDM_DataMgr.h"
#include "GDM_Mem.h"
#include "GDM_Camera.h"
#include "Layer.h"

#define GDM_FONSIZE_STEP1	3000000
#define GDM_FONSIZE_STEP2	5000000
#define GDM_FONSIZE_STEP3	7000000

int CGDM_TextNode::Compare(const CGDM_TextNode &a_cOther) const
{
	int ret = 0;

	// compare type
	if(m_data.datatype < a_cOther.m_data.datatype)
		return -1;
	if(m_data.datatype > a_cOther.m_data.datatype)
		return 1;

	// compare key
	if(m_data.key < a_cOther.m_data.key)
		return -1;
	if(m_data.key > a_cOther.m_data.key)
		return 1;

	// compare name
	if(m_data.rendertype == RTT_GRID)
		ret = memcmp(m_data.strName, a_cOther.m_data.strName, MAX_GEO_TEXT_LEN);

	if(m_data.rendertype == RTT_CONTOUR)
		ret = memcmp(&m_data.dLongitude, &a_cOther.m_data.dLongitude, sizeof(double));

	return ret;
}

CGDM_TextNode::CGDM_TextNode(void)
{
	memset(&m_data, 0, sizeof(m_data));
}

CGDM_TextNode::~CGDM_TextNode(void)
{
}

void CGDM_TextNode::IncreaseScale(bool blFocus)
{
	if(!blFocus)
	{
		m_data.drawScale += 0.05f;

		if(m_data.drawScale > 1.f)
			m_data.drawScale = 1.f;
	}
	else if(IsEnableFocusScale(m_data.rendertype))
	{
		m_data.focusScale += 0.05f;

		if(m_data.focusScale > 0.5f)
			m_data.focusScale = 0.5f;

	}
}

void CGDM_TextNode::DecreaseScale(bool blFocus)
{
	if(!blFocus)
	{
		m_data.drawScale -= 0.1f;

		if(m_data.drawScale < 0.f)
			m_data.drawScale = 0.f;
	}
	else  if(IsEnableFocusScale(m_data.rendertype))
	{
		m_data.focusScale-= 0.1f;

		if(m_data.focusScale < 0.f)
			m_data.focusScale = 0.f;

	}
}

bool CGDM_TextNode::IsFullView()
{
	return m_data.drawScale > 0.99f;
}

bool CGDM_TextNode::IsEnableFocusScale(RENTER_TEXT_TYPE	type)
{
	return true;
	switch(type)
	{
	case RTT_PLACE_MARK_NORMAL:
		return true;
	}

	return false;
}

////////////////////////////////////////////////////////////////////

CGDM_TextMgr::CGDM_TextMgr(void)
{
	lableSize = RTS_NORMAL;
	fontSize = 0;
	nFocus_ID = -1;
}

CGDM_TextMgr::~CGDM_TextMgr(void)
{

}

void CGDM_TextMgr::Init(int width, int height)
{
	screenWidth = width;
	screenHeight= height;

	m_cAddList.clear();
	nFocus_ID = -1;
}

void CGDM_TextMgr::Init(void)
{
	m_cAddList.clear();
	nFocus_ID = -1;
}

void CGDM_TextMgr::AddText(E_GDM_SUBDATA_TYPE datatype,CGDM_Location3D &loc, RENTER_TEXT_TYPE type, const unsigned short* str, int key, bool blKorean)
{
	QString strName;

	strName = strName.fromUtf16(str);
	AddText(datatype,loc, type, strName, key, blKorean);
}

void CGDM_TextMgr::ResetScreenPt(CGDM_TextNode *pNode)
{
	GDM_ERROR_NO ret;
	CGDM_Vector3DF pos;
	if(pNode == NULL)
		return;

	double height = pNode->m_data.height;

	if(!g_pGDMDataMgr->IsVisible(E_GDM_SUBDATA_TYPE_DEM))
		height = 0;

	// get screen point
	ret = g_pGDMDataMgr->m_pCamera->WorldToScreen(pNode->m_data.dLongitude, pNode->m_data.dLatitude, height, &pos);

	if(ret != GDM_SUCCESS)
	{
		pos.m_tX = -1;
		pos.m_tY = -1;
	}

	SetInfo(&pNode->m_data, pos.m_tX, pos.m_tY, pNode->m_data.rendertype,pNode->m_data.iconscale);
}


void CGDM_TextMgr::AddText(E_GDM_SUBDATA_TYPE datatype,CGDM_Location3D &loc, RENTER_TEXT_TYPE type, QString &str, int key, bool blKorean)
{
	int len;

	CGDM_TextNode textNode;

	len = str.length();

	if(len < 1 || len > MAX_GEO_TEXT_LEN - 1)
		return;

	//revised by RSH 2013.3.13
	if(loc.m_tZ <= 0)
		loc.m_tZ = gdm_getExactHeight(loc.m_tX, loc.m_tY) ;

	//if(loc.m_tZ <= 0)
	//	loc.m_tZ = gdm_getMaxHeight(loc.m_tX, loc.m_tY) / g_pGDMDataMgr->m_sOption.dem_detail_rate;

	textNode.m_data.datatype        =datatype; 
	textNode.m_data.rendertype		= type;
	textNode.m_data.dLongitude	= loc.m_tX;
	textNode.m_data.dLatitude	= loc.m_tY;
	textNode.m_data.key			= key;
	textNode.m_data.blKorean	= blKorean;
	textNode.m_data.height		= loc.m_tZ;
	textNode.m_data.iconscale   = 1.0; //by ugi 2013.8.1
	if(loc.m_tZ > 0)
		textNode.m_data.blOrgHeight = true;
	else
		textNode.m_data.blOrgHeight = false;
		
	memcpy(textNode.m_data.strName, str.utf16(), sizeof(unsigned short) * len);
	textNode.m_data.strName[len] = 0;

	ResetScreenPt(&textNode);

	if(textNode.m_data.posNameX < 0 || textNode.m_data.posNameY < 0 || 
		textNode.m_data.posNameX > screenWidth || textNode.m_data.posNameY > screenHeight)
		return;

	m_cAddList.push_back(textNode);
}

void CGDM_TextMgr::SetInfo(GDM_TextCell* pTextCell , double	screenX, double screenY, RENTER_TEXT_TYPE type,double scale)
{
	QString strTmp;
	strTmp = strTmp.fromUtf16(pTextCell->strName);
	int len = strTmp.length();
	int fontID , _fontSize;
	double textWidth;
	double textHeight;
	double maxwidth;

	//double scale = 1.0;

	pTextCell->iconSizeX = pTextCell->iconSizeY = 0;

	double dist = g_pGDMDataMgr->m_pCamera->m_location.m_dDist;

	switch(type)
	{
		case RTT_PYONGYANG:
			{
				fontID = FSI_LARGE;
				pTextCell->iconSizeX = 25.f;
				pTextCell->iconSizeY = 25.f;
			}
			break;
		case RTT_PLACE_MARK_NORMAL:				
		case RTT_PLACE_MARK_SELECT:
			{
				fontID = FSI_TINY;
				pTextCell->iconSizeX = 20.f;
				pTextCell->iconSizeY = 20.f;
			}
			break;
		case RTT_CONTOUR:
			{
				fontID = FSI_TOPTINY;
				break;
			}	
		case RTT_GRID:
			{
				fontID = FSI_TINY;
			}
			break;
		case RTT_PLACE_AREA:
		case RTT_PLACE_LENGTH:
		case RTT_NAME_DEFAULT:
			fontID=FSI_NORMAL;
			{
				if(pTextCell->datatype!=E_GDM_SUBDATA_TYPE_NONE)
				{
					PlaceNameLayer* layer=(PlaceNameLayer*)g_pGDMDataMgr->m_LayerHash[pTextCell->datatype];
					Q_ASSERT(layer!=NULL);
					fontID = layer->GetFontSizeID();
					pTextCell->iconSizeX = layer->GetIconSizeX();
					pTextCell->iconSizeY = layer->GetIconSizeY();
				}
			}
		
	}

	switch(type)
	{
	case RTT_PYONGYANG:
	{
			if(dist > GDM_FONSIZE_STEP3)
			{
				fontID += 3;
				scale = 0.7;
			}
			else if(dist > GDM_FONSIZE_STEP2)
			{
				fontID += 2;
				scale = 0.8;
			}
			else if(dist > GDM_FONSIZE_STEP1)
			{
				fontID += 1;
				scale = 0.9;
			}
			break;
		}
	}

	pTextCell->iconSizeX *= scale;
	pTextCell->iconSizeY *= scale;

	if(fontID < FSI_TOPBIG )
		fontID = FSI_TOPBIG;
	if(fontID > FSI_TOPTINY)
		fontID = FSI_TOPTINY;

	// ¸ÆË©´Å¸ôº÷ ·ñ Ê­ËË¿º ±¡»ôÌ© Ì¡¾·º¼¼³

	_fontSize = GetFontSize(fontID);
	pTextCell->posNameX = (float)screenX - (_fontSize * len) / 2;
	pTextCell->posNameY = (float)screenY - 5;
	pTextCell->rendertype = type;
	pTextCell->drawing   = true;
	pTextCell->fontID = fontID;

	// TextWidth is _fontSize * len, but Number, Korean Charactor and English Charactor's width are not equal.
	// So TextWidth is calculated differently.
	if (type == RTT_PLACE_AREA || type == RTT_PLACE_LENGTH)
	{
		textWidth = _fontSize * len / 2.f;
	}
	else 
	{
		if(pTextCell->blKorean)
			textWidth = _fontSize * 2.f * len;
		else
			textWidth = _fontSize * 0.8f * len;
	}

	textHeight = _fontSize;
	maxwidth = textWidth > pTextCell->iconSizeX ? textWidth : pTextCell->iconSizeX;
	float offset = 2.0f;

	if(pTextCell->iconSizeX) // Ê­ËË¿ºËË ËØ²÷ °ÒË§
	{
		pTextCell->posIconX = (float)screenX - pTextCell->iconSizeX / 2.0f;
		pTextCell->posIconY = (float)screenY - pTextCell->iconSizeY / 2.0f;

		if (pTextCell->blKorean)
		{
			pTextCell->posNameX = (float)screenX - (float)(textWidth / 1.6f) / 2.0f;
			pTextCell->right = (float) pTextCell->posNameX + (float)(textWidth / 1.6f);
		}
		else
		{
			pTextCell->posNameX = (float)screenX - (float)(textWidth / 1.1f) / 2.0f;
			pTextCell->right = (float) pTextCell->posNameX + (float)(textWidth / 1.1f);
		}
		pTextCell->posNameY = (float)pTextCell->posIconY - 5.f;	
		
		pTextCell->left = (float) pTextCell->posNameX;		
		pTextCell->top = (float) pTextCell->posNameY - textHeight;
		pTextCell->bottom = (float) pTextCell->posIconY + (float)pTextCell->iconSizeY;
	}
	else
	{
		pTextCell->posIconX = 0.f;
		pTextCell->posIconY = 0.f;
		pTextCell->posNameX = (float)screenX - (float)textWidth / 2.0f;
		pTextCell->posNameY = (float)screenY + (float)textHeight / 2.0f;
		pTextCell->left = (float)pTextCell->posNameX;
		pTextCell->top = (float)pTextCell->posNameY - (float)textHeight;

		if (type == RTT_PLACE_AREA || type == RTT_PLACE_LENGTH)
		{
			pTextCell->right = (float)pTextCell->posNameX + textWidth;
		}
		else
		{
			if (pTextCell->blKorean)
				pTextCell->right = (float)pTextCell->posNameX + (float)(textWidth / 1.6f);
			else
				pTextCell->right = (float)pTextCell->posNameX + (float)(textWidth / 1.1f);
		}
		
		pTextCell->bottom = (float)pTextCell->top + (float)textHeight;			
	}

	pTextCell->left -= offset;
	pTextCell->top  -= offset;
	pTextCell->right += offset;
	pTextCell->bottom += offset;
}

int CGDM_TextMgr::GetFontSize(int fontID)
{
	int nOffset = 0, size;
	switch(fontID)
	{
	case FSI_TOPBIG:
		nOffset = FSI_TOPBIG_OFFSET;
		break;
	case FSI_BIG:
		nOffset = FSI_BIG_OFFSET;
		break;
	case FSI_LARGE:
		nOffset = FSI_LARGE_OFFSET;
		break;
	case FSI_NORMAL:
		nOffset = FSI_NORMAL_OFFSET;
		break;
	case FSI_SAMLL:
		nOffset = FSI_SAMLL_OFFSET;
		break;
	case FSI_TINY:
		nOffset = FSI_TINY_OFFSET;
		break;
	case FSI_TOPTINY:
		nOffset = FSI_TOPTINY_OFFSET;
		break;
	}

	size = fontSize + nOffset;
	switch(lableSize)
	{
	case RTS_NORMAL:
		size = (int)(size * 1.2);
		break;
	case RTS_SMALL:
		size = size;
		break;
	case RTS_LARGE:
		size = size * 1.5;
		break;
	}
	return size;
}

bool CGDM_TextMgr::isIntersects(GDM_TextCell* pCell1, GDM_TextCell* pCell2)
{
	if(pCell1->right < pCell2->left)
		return false;
	if(pCell1->left > pCell2->right)
		return false;
	if(pCell1->bottom < pCell2->top)
		return false;
	if(pCell1->top > pCell2->bottom)
		return false;

	return true;
}

void CGDM_TextMgr::GetDrawingText(CGDM_TextNodeArr &cArr)
{
	SortAndArrange();
	SetFocusState();
	cArr = m_cOldList;
}

#define MAX_SAME_ADD_COUNT	2

void CGDM_TextMgr::SortAndArrange()
{
	unsigned int i, k, nCount;
	CGDM_Vector3DF pos;
	int same_add_cnt = 0;
	int same_del_cnt = 0;

	CGDM_TextNodeArr newList;

	// sort
	m_cAddList.sort();
	m_cOldList.sort();

	nCount = m_cAddList.size();

	// check intersect rect
	for (i = 0; i < nCount; i++)
	{
		if(!m_cAddList[i].m_data.drawing)
			continue;
	
		for (k = i + 1; k < nCount; k++)
		{
			if (isIntersects(&m_cAddList[i].m_data, &m_cAddList[k].m_data))
				m_cAddList[k].m_data.drawing = false;	
		}	
	}

	// decide drawing text and size

	// check add list
	for(i = 0; i < m_cAddList.size(); i++)
	{
		CGDM_TextNode &node = m_cAddList[i];
		if(!node.m_data.drawing)
		{
			m_cAddList.erase(i);
			i--;
			continue;
		}

		int findIndex = m_cOldList.binary_search_const(node);
		if(findIndex >= 0)
		{
			node.m_data.drawScale = m_cOldList[findIndex].m_data.drawScale;
			node.m_data.focusScale = m_cOldList[findIndex].m_data.focusScale;

			double deltaHeight = node.m_data.height - m_cOldList[findIndex].m_data.height;
			if(deltaHeight > 1)
			{
				deltaHeight = deltaHeight / 10;
				node.m_data.height = m_cOldList[findIndex].m_data.height + deltaHeight;
				ResetScreenPt(&node);
			}

			m_cOldList.erase(findIndex);
		}
		else
			same_add_cnt++;

		if(findIndex >= 0 || same_add_cnt < MAX_SAME_ADD_COUNT)
		{
			node.IncreaseScale();
			newList.push_back(node);		// add drawing text
		}
	}

	// check old list
	for(i = 0; i < m_cOldList.size(); i++)
	{
		CGDM_TextNode &node = m_cOldList[i];
		int findIndex = m_cAddList.binary_search_const(node);
		if(findIndex >= 0)
			continue;

		if(node.IsFullView())
		{
			if(same_del_cnt < MAX_SAME_ADD_COUNT)
				node.DecreaseScale();
			same_del_cnt++;
		}
		else
			node.DecreaseScale();

		if(node.m_data.drawScale > 0)
		{
			// reset screen pos
			ResetScreenPt(&node);

			newList.push_back(node);	// add drawing text
		}
	}

	m_cOldList = newList;
	m_cAddList.clear();
}

void CGDM_TextMgr::SetFocusState()
{
	unsigned int i, nCount;
	MOUSE_POSITION	mousePos;
	nFocus_ID = -1;
	bool findSelect	= false;
	n_FocusType=E_GDM_SUBDATA_TYPE_NONE;

	mousePos = g_pGDMDataMgr->m_pCamera->GetMousePosition();
	nCount = m_cOldList.size();

	// find focus index
	for(i = 0; i < nCount; i++)
	{
		if(mousePos.x >= m_cOldList[i].m_data.left && 
			mousePos.x <= m_cOldList[i].m_data.right && 
			mousePos.y >= m_cOldList[i].m_data.top && 
			mousePos.y <= m_cOldList[i].m_data.bottom)
		{
			nFocus_ID = (int)i;
			break;
		}
	}

	// set focus state
	for(i = 0; i < nCount; i++)
	{
		if((int)i == nFocus_ID)
			m_cOldList[i].IncreaseScale(true);
		else
			m_cOldList[i].DecreaseScale(true);
	}

	if(nFocus_ID!=-1)
	{
		n_FocusType=m_cOldList[nFocus_ID].m_data.datatype;
		nFocus_ID=m_cOldList[nFocus_ID].m_data.key;
	}
}

//by RSH 2013.7.25
void CGDM_TextMgr::AddText(CGDM_Location3D &loc, RENTER_TEXT_TYPE type, QString &str, int key, bool blKorean, QString iconName,double iconscale)
{
	int len;

	CGDM_TextNode textNode;

	len = str.length();

	if(len < 1 || len > MAX_GEO_TEXT_LEN - 1)
		return;

	//revised by RSH 2013.3.13
	if(loc.m_tZ <= 0)
		loc.m_tZ = gdm_getExactHeight(loc.m_tX, loc.m_tY) ;

	//if(loc.m_tZ <= 0)
	//	loc.m_tZ = gdm_getMaxHeight(loc.m_tX, loc.m_tY) / g_pGDMDataMgr->m_sOption.dem_detail_rate;

	textNode.m_data.rendertype		= type;
	textNode.m_data.dLongitude	= loc.m_tX;
	textNode.m_data.dLatitude	= loc.m_tY;
	textNode.m_data.key			= key;
	textNode.m_data.blKorean	= blKorean;
	textNode.m_data.height		= loc.m_tZ;
	textNode.m_data.iconscale   = iconscale;//by ugi 2013.8.1
	if(loc.m_tZ > 0)
		textNode.m_data.blOrgHeight = true;
	else
		textNode.m_data.blOrgHeight = false;

	memcpy(textNode.m_data.strName, str.utf16(), sizeof(unsigned short) * len);
	textNode.m_data.strName[len] = 0;

	ResetScreenPt(&textNode);

	if(textNode.m_data.posNameX < 0 || textNode.m_data.posNameY < 0 || 
		textNode.m_data.posNameX > screenWidth || textNode.m_data.posNameY > screenHeight)
		return;

	if (!iconName.isEmpty())
	{
		len = iconName.length();
		memcpy(textNode.m_data.iconName, iconName.utf16(), sizeof(unsigned short) * len);
	}

	m_cAddList.push_back(textNode);
}
//end addition by RSH

