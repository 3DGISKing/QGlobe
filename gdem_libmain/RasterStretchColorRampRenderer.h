#ifndef RASTERSTRETCHCOLORRAMPRENDER_H 
#define RASTERSTRETCHCOLORRAMPRENDER_H 

#include "RasterRenderer.h"
#include "QtGui/QColor"


class RasterStretchColorRampRender :public IRasterRenderer
{
public:

	enum StretchType
	{
		MinimumMaximum,
		StandardDeviation
	};
	RasterStretchColorRampRender();
	virtual ~RasterStretchColorRampRender();
	virtual QRgb GetColorFromData(float val);
private:
	QRgb GetColorFromDataByMinMax(float val);
	QRgb GetColorFromDataByStdDev(float val);

	QRgb         _nodatacolor;
	StretchType  _stretchtype;
};

#endif