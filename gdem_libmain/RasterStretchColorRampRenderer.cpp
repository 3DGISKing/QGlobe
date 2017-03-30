#include "RasterStretchColorRampRenderer.h"
#include "ColorRamp.h"
#include "Layer.h"

RasterStretchColorRampRender::RasterStretchColorRampRender()
{
	_colorRamp=new AlgorithmicColorRamp(Qt::white,Qt::black,"White to Black");	
	
	_stretchtype=StandardDeviation;

	//white color
	_nodatacolor=0xffffff;
};

RasterStretchColorRampRender::~RasterStretchColorRampRender()
{
	delete _colorRamp;
}

QRgb RasterStretchColorRampRender::GetColorFromDataByMinMax(float val)
{
	Q_ASSERT(_rasterLayer!=NULL);

	if(val==_rasterLayer->GetNoDataValue())
		return _nodatacolor;

	double max=_rasterLayer->GetStatistics()._maxvalue;
	double min=_rasterLayer->GetStatistics()._minvalue;

	double ratio=val-min;

	ratio=ratio/(max-min);

	int index=ratio*(_colorRamp->GetSize()-1);

	return _colorRamp->Color(index).rgb();
}

QRgb RasterStretchColorRampRender::GetColorFromDataByStdDev(float val)
{
	Q_ASSERT(_rasterLayer!=NULL);

	if(val==_rasterLayer->GetNoDataValue())
		return _nodatacolor;

	double mean=_rasterLayer->GetStatistics()._meanvalue;
	double stddev=_rasterLayer->GetStatistics()._stddev;

	double ratio=val-(mean-stddev);

	ratio=ratio/(2*stddev);

	ratio=qBound(0.0,ratio,1.0);
	int index=ratio*(_colorRamp->GetSize()-1);

	return _colorRamp->Color(index).rgb();
}

QRgb RasterStretchColorRampRender::GetColorFromData(float val)
{
	if(_stretchtype==MinimumMaximum)
		return GetColorFromDataByMinMax(val);
	else if(_stretchtype==StandardDeviation)
		return GetColorFromDataByStdDev(val);
	
	qFatal("Unknown Stretch Type");

	return 0xffffffff;
}

