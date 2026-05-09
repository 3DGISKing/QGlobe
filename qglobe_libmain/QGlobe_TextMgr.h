#pragma once

#include "qglobeclientbase.h"
#include "qglobe_libmain_global.h"
#include "QGlobe_Array.h"
#include "QGlobe_Vector3D.h"
#include "qglobe_protocol.h"

#define MAX_GEO_TEXT_LEN		40
#define QGlobe_MAX_TEXT_COUNT	    200

typedef struct _qglobe_text_cell
{
	E_QGlobe_SUBDATA_TYPE		datatype;
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
	
	unsigned short			iconName[QGlobe_MAX_TEXT_COUNT]; //by RSH 2013.7.25
	double                  iconscale;
} QGlobe_TextCell;

class CQGlobe_TextNode
{
public:
	CQGlobe_TextNode(void);
	~CQGlobe_TextNode(void);
	CQGlobe_TextNode(const CQGlobe_TextNode& a_cOther) : m_data(a_cOther.m_data) {}
public:
	bool operator<(const CQGlobe_TextNode &a_cOther) const		{return Compare(a_cOther) < 0;}
	bool operator>(const CQGlobe_TextNode &a_cOther) const		{return Compare(a_cOther) > 0;}
	bool operator==(const CQGlobe_TextNode &a_cOther) const	{return Compare(a_cOther) == 0;}
	bool operator!=(const CQGlobe_TextNode &a_cOther) const	{return Compare(a_cOther) != 0;}
	CQGlobe_TextNode& operator=(const CQGlobe_TextNode& a_cOther) { m_data = a_cOther.m_data; return *this; }
	bool    IsFullView();
	int	    Compare(const CQGlobe_TextNode &a_cOther) const;
	void	IncreaseScale(bool blFocus = false);
	void	DecreaseScale(bool blFocus = false);
private:
	bool	IsEnableFocusScale(RENTER_TEXT_TYPE	type);

public:
	QGlobe_TextCell m_data;
};

typedef qglobe_array<CQGlobe_TextNode>	CQGlobe_TextNodeArr;

class QGLOBE_LIBMAIN_EXPORT CQGlobe_TextMgr
{
public:
	CQGlobe_TextMgr(void);
	~CQGlobe_TextMgr(void);
public:
	void	AddText(E_QGlobe_SUBDATA_TYPE datatype,CQGlobe_Location3D &loc, RENTER_TEXT_TYPE type, const unsigned short* str, int key = 0, bool blKorean = true);
	void	AddText(E_QGlobe_SUBDATA_TYPE datatype,CQGlobe_Location3D &loc, RENTER_TEXT_TYPE type, QString &str, int key = 0, bool blKorean = true);

	//by RSH 2013.7.25
	void	AddText(CQGlobe_Location3D &loc, RENTER_TEXT_TYPE type, QString &str, int key, bool blKorean, QString iconName,double iconscale);
	//end edition by RSH

	void	Init(int width, int height);
	void	Init(void);

	void	GetDrawingText(CQGlobe_TextNodeArr &cArr);
	void	SetLableSize(int aLableSize) { lableSize = aLableSize; }
	void	SetFontSize(int aFontSize) { fontSize = aFontSize; }
	int		GetFontSize(int fontID);
	inline  int		GetFocusID(){return nFocus_ID;};
	E_QGlobe_SUBDATA_TYPE		GetFocusType(){return n_FocusType;};
	
private:
	int				 screenWidth;
	int				 screenHeight;
	int				 lableSize;
	int				 fontSize;

	CQGlobe_TextNodeArr m_cOldList;
	CQGlobe_TextNodeArr m_cAddList;
	int				 nFocus_ID;
	E_QGlobe_SUBDATA_TYPE n_FocusType;

private:
	void	SetInfo(QGlobe_TextCell* pTextCell , double screenX, double screenY, RENTER_TEXT_TYPE type,double scale=1.0);
	bool	isIntersects(QGlobe_TextCell* pCell1, QGlobe_TextCell* pCell2);
	void	ResetScreenPt(CQGlobe_TextNode *pNode);
	void	SortAndArrange();
	void	SetFocusState();
};
