#ifndef _GIS_GENERATE_TEXTURE_H_
#define _GIS_GENERATE_TEXTURE_H_

#include <QtCore/QRectF>
#include "QtCore/QHash"
#include <QtGui/QColor>
#include <QtGui/QImage>

#include "gdem_scene_global.h"
#include "GeoShapeSceneNode.h"

typedef struct _GIS_POLYGON_RENDER_INFO
{
	unsigned int strokeColor;
	double strokeThickness;
	unsigned int fillColor;
	unsigned short polyTextureName[PMD_STR_MAX_SIZE]; //by RSH 2013.8.1
} GIS_POLYGON_RENDER_INFO;

class GIS_PathData;
class QPainter;
class QPixmap;

class GDEM_SCENE_EXPORT GIS_TextureGenerator
{
public:
	GIS_TextureGenerator();
	virtual ~GIS_TextureGenerator();

	void RenderBegin(QRectF rect);

	bool RenderShapeData(LineSegment* polygon,QColor color,int width);

	bool Render(GIS_PathData* pathData, GIS_POLYGON_RENDER_INFO renderInfo);
	QImage* RenderEnd();
	QImage* GetTexture(QString texturename);

protected:
	QPixmap*	_Pixmap;
	QPixmap*	_ColorPixmap;
	int			_TextureWidth;
	int			_TextureHeight;
	int			_ColorPixmapWidth;
	int			_ColorPixmapHeight;
	QRectF		_RenderBound;
	QImage		_Image;
	bool		_RenderEmpty;
	bool		_ContainComplete;
private:
	void        InitPixmap();
	QHash<QString,QImage*> _TextureHash;
};

GIS_TextureGenerator* GIS_GetTextureGenerator();

#endif //_GIS_GENERATE_TEXTURE_H_