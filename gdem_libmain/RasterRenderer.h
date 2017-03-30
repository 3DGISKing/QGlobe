#ifndef RASTERRENDER_H
#include <QtGui/QColor>

class RasterLayer;
class IColorRamp;

class IRasterRenderer
{
public:
	IRasterRenderer()
	{
		_rasterLayer=0;
	}

	virtual ~IRasterRenderer()
	{
		
	}
	RasterLayer* GetLayer(){return _rasterLayer;}
	void         SetLayer(RasterLayer* l){_rasterLayer=l;}
	virtual      QRgb GetColorFromData(float val)=0;
	void SetColorRamp(IColorRamp* r){_colorRamp=r;}
	IColorRamp* GetColorRamp(){return _colorRamp;}
protected: 
	RasterLayer* _rasterLayer;
	IColorRamp*  _colorRamp;	
};
#endif