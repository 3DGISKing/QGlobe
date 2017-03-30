#include "MapInfo.h"
#include <QtCore/QFile>
#include <QtCore/QTextStream>

MapInfo::MapInfo(QString path)
{
	m_IsNull=false;
	read(path);
}

void MapInfo::read(QString path)
{
	int i=path.lastIndexOf("/");
	int j=path.lastIndexOf(".coord");
	m_MapName=path.mid(i+1,j-i-1);

	QFile file(path);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		m_IsNull=true;
		return ;
	}
	else
	{
		QTextStream in(&file);
		QString line;

		double val[9];
		double xresolution,yresolution;
		int k=0;
		do 
		{
			line=in.readLine();	
			if (!line.isEmpty())			
			{
				val[k] = line.toDouble();
				k++;
			}
		} while (!line.isNull());

		m_MapRect.m_cMinPt.m_tX= val[4];//left
		m_MapRect.m_cMaxPt.m_tY= val[5];//top

		xresolution = val[0];
		yresolution = val[3];

		m_Width=val[6];
		m_Height=val[7];
		m_EnabledMaxHeight=val[8];
	
		m_MapRect.m_cMaxPt.m_tX= val[4]+(double)m_Width*xresolution;  //right
		m_MapRect.m_cMinPt.m_tY= val[5]+(double)m_Height*yresolution; //bottom
	}
	file.close();
}

