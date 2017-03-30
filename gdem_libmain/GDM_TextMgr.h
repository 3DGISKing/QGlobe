#pragma once

#include "gdemclientbase.h"
#include "gdem_libmain_global.h"
#include "GDM_Array.h"
#include "GDM_Vector3D.h"
#include "gdem_protocol.h"

#define MAX_GEO_TEXT_LEN		40
#define GDM_MAX_TEXT_COUNT	    200

typedef struct _gdm_text_cell
{
	E_GDM_SUBDATA_TYPE		datatype;
	RENTER_TEXT_TYPE        rendertype;
	unsigned short			strName[MAX_GEO_TEXT_LEN];
	float					posNameX;
	float					posNameY;
	float					posIconX;
	float					posIconY;
	float					left, right, top, bottom;
	int						iconSizeX;
	int						iconSizeY;
	int						fontID;
	bool					drawing;
	bool					blKorean;
	bool					blOrgHeight;
	double					dLongitude; 
	double					dLatitude;  
	double					height;
	float					drawScale;	// 0~1
	float					focusScale; // 0~1
	int						key;		// for auto hide key
	
	unsigned short			iconName[GDM_MAX_TEXT_COUNT]; //by RSH 2013.7.25
	double                  iconscale;
} GDM_TextCell;

class CGDM_TextNode
{
public:
	CGDM_TextNode(void);
	~CGDM_TextNode(void);
	CGDM_TextNode(const CGDM_TextNode& a_cOther) : m_data(a_cOther.m_data) {}
public:
	bool operator<(const CGDM_TextNode &a_cOther) const		{return Compare(a_cOther) < 0;}
	bool operator>(const CGDM_TextNode &a_cOther) const		{return Compare(a_cOther) > 0;}
	bool operator==(const CGDM_TextNode &a_cOther) const	{return Compare(a_cOther) == 0;}
	bool operator!=(const CGDM_TextNode &a_cOther) const	{return Compare(a_cOther) != 0;}
	CGDM_TextNode& operator=(const CGDM_TextNode& a_cOther) { m_data = a_cOther.m_data; return *this; }
	bool    IsFullView();
	int	    Compare(const CGDM_TextNode &a_cOther) const;
	void	IncreaseScale(bool blFocus = false);
	void	DecreaseScale(bool blFocus = false);
private:
	bool	IsEnableFocusScale(RENTER_TEXT_TYPE	type);

public:
	GDM_TextCell m_data;
};

typedef gdm_array<CGDM_TextNode>	CGDM_TextNodeArr;

class GDEM_LIBMAIN_EXPORT CGDM_TextMgr
{
public:
	CGDM_TextMgr(void);
	~CGDM_TextMgr(void);
public:
	void	AddText(E_GDM_SUBDATA_TYPE datatype,CGDM_Location3D &loc, RENTER_TEXT_TYPE type, const unsigned short* str, int key = 0, bool blKorean = true);
	void	AddText(E_GDM_SUBDATA_TYPE datatype,CGDM_Location3D &loc, RENTER_TEXT_TYPE type, QString &str, int key = 0, bool blKorean = true);

	//by RSH 2013.7.25
	void	AddText(CGDM_Location3D &loc, RENTER_TEXT_TYPE type, QString &str, int key, bool blKorean, QString iconName,double iconscale);
	//end edition by RSH

	void	Init(int width, int height);
	void	Init(void);

	void	GetDrawingText(CGDM_TextNodeArr &cArr);
	void	SetLableSize(int aLableSize) { lableSize = aLableSize; }
	void	SetFontSize(int aFontSize) { fontSize = aFontSize; }
	int		GetFontSize(int fontID);
	inline  int		GetFocusID(){return nFocus_ID;};
	E_GDM_SUBDATA_TYPE		GetFocusType(){return n_FocusType;};
	
private:
	int				 screenWidth;
	int				 screenHeight;
	int				 lableSize;
	int				 fontSize;

	CGDM_TextNodeArr m_cOldList;
	CGDM_TextNodeArr m_cAddList;
	int				 nFocus_ID;
	E_GDM_SUBDATA_TYPE n_FocusType;

private:
	void	SetInfo(GDM_TextCell* pTextCell , double screenX, double screenY, RENTER_TEXT_TYPE type,double scale=1.0);
	bool	isIntersects(GDM_TextCell* pCell1, GDM_TextCell* pCell2);
	void	ResetScreenPt(CGDM_TextNode *pNode);
	void	SortAndArrange();
	void	SetFocusState();
};
