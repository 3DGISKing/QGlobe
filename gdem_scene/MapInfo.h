#ifndef MAPINFO_H

#define MAPINFO_H

#include <QtCore/QString>
#include "../gdem_base/GDM_Rect.h"

class MapInfo
{
public:
	MapInfo(QString path);

	inline     QString    getMapName()        {return m_MapName;}
	inline     bool       isNull()            {return m_IsNull;} 
	inline     double     getMaxHeight()      {return m_EnabledMaxHeight;}
	bool       isContained(CGDM_Rect2D rec)   {return m_MapRect.isContained(rec); }
	bool       isIntersected(CGDM_Rect2D rec) {return m_MapRect.isRectCollided(rec); }
	bool       isContain(double longi, double lat){return m_MapRect.isPointInside(longi,lat);}
	inline     double     left()              {return m_MapRect.m_cMinPt.m_tX;}
	inline     double     right()             {return m_MapRect.m_cMaxPt.m_tX;}
	inline     double     top()               {return m_MapRect.m_cMaxPt.m_tY;}
	inline     double     bottom()            {return m_MapRect.m_cMinPt.m_tY;}

	friend inline     bool operator  <(const MapInfo a,const MapInfo b)
	{
		return a.m_MapRect.getArea()<b.m_MapRect.getArea();
	}
private:
	void        read(QString path);

	QString     m_MapName;
	int         m_Width;     //in pixels
	int         m_Height;    //in pixels
	bool        m_IsNull;

	CGDM_Rect2D m_MapRect;   //express the bounding rect of map
	double      m_EnabledMaxHeight;
};

#endif