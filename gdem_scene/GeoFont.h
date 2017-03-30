#pragma once

#include <QString>
#include <QChar>
#include "GeoFontTypeFace.h"
#include "IVideoDriver.h"
#include "IReferenceCounted.h"
using namespace geo;

class CGeoFont : public virtual IReferenceCounted
{
public:
	CGeoFont( float sizeI, const QString& ttfFileName );
	~CGeoFont() {;}

	void print(float x, float y, const QString& s, int upsidedown = 1, video::SColor aColor = video::SColor(255, 255, 255, 255)	) const
	{
		m_typeFace.Render(s, Vec2f(x, y), upsidedown==1, aColor);		
	}

	void printOutlinedString(float x, float y, const QString& s, bool speedOptimize = 1, float xshift = 0, float yshift = 0, 
		video::SColor aColor = video::SColor(255, 255, 255, 255),
		unsigned int afontflag = 0, float scale = 1.0f) const;

	void printChar(const QChar c) const
	{
		m_typeFace.renderGlyphs(c);
	}

	void drawUnderline(float x, float y, const QString& s, video::SColor aColor = video::SColor(255, 255, 255, 255), bool bUnderline = true) const;
	
	void printCharOutlined(const QChar c, unsigned int afontflag = 0) const;
	float getStrLen(const QString& s) const {return m_typeFace.getWidgth(s);}
	float getLineHeight(void) const {return m_typeFace.getLineHeight();}
	float getAscent(void) const {return m_typeFace.getAscent();}
	float getDescent(void) const {return m_typeFace.getDescent();}
	float getSize(void) const {return m_typeFace.getPointSize();}

	void setResolution(size_t aResolution) { m_typeFace.setResolution(aResolution); }
	void setPointSize(size_t aPointSize) { m_typeFace.setPointSize(aPointSize); }
	
	mutable CGeoFontTypeFace m_typeFace; 	
};
