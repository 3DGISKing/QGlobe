#include <QtGui/QPainter>

#include "GIS_GenerateTexture.h"
#include "GIS_Polygon.h"
#include "gdemclient.h"

using namespace geo;
using namespace core;

GIS_TextureGenerator g_gisTextureGenerator;

GIS_TextureGenerator* GIS_GetTextureGenerator()
{
	return &g_gisTextureGenerator;
}

GIS_TextureGenerator::GIS_TextureGenerator()
{
	_TextureWidth = 256;
	_TextureHeight = 256;
	_ColorPixmapWidth = 4;
	_ColorPixmapHeight = 4;
	_RenderEmpty = true;
}

GIS_TextureGenerator::~GIS_TextureGenerator()
{
	if(_Pixmap)
		delete _Pixmap;

	if(_ColorPixmap)
		delete _ColorPixmap;

	QHashIterator<QString, QImage*> i(_TextureHash);
	while (i.hasNext()) 
	{
		i.next();
		delete i.value();	
	}

	_TextureHash.clear();
}

void GIS_TextureGenerator::InitPixmap()
{
	_Pixmap = new QPixmap(_TextureWidth, _TextureHeight);
	_ColorPixmap = new QPixmap(_ColorPixmapWidth, _ColorPixmapHeight);
}

void GIS_TextureGenerator::RenderBegin(QRectF rect)
{
	if(!_Pixmap)
		InitPixmap();

	Q_ASSERT(rect.isEmpty()==false);

	_RenderBound.setRect(rect.left() + gdm_PI, gdm_PI - rect.bottom(), rect.width(), rect.height());

	_Pixmap->fill(QColor(0, 0, 0, 0));
	_ColorPixmap->fill(QColor(0, 0, 0, 0));

	_RenderEmpty = true;
	_ContainComplete = true;

}

QImage* GIS_TextureGenerator::RenderEnd()
{
	if(_RenderEmpty)
		return NULL;

	if(_ContainComplete)
		_Image = _ColorPixmap->toImage().convertToFormat(QImage::Format_ARGB32);
	else
		_Image = _Pixmap->toImage().convertToFormat(QImage::Format_ARGB32);

	Q_ASSERT(_Image.isNull()==false);

	return &_Image;
}

QImage* GIS_TextureGenerator::GetTexture(QString texturename)
{
	if(_TextureHash.contains(texturename))
		return _TextureHash.value(texturename);
	
	if(!QFile::exists(texturename))
		return NULL;

	QImage *im=new QImage;

	if(im->load(texturename))
	{
		_TextureHash.insert(texturename,im);
		return im;
	}
	else
	{
		delete im;
		return NULL;
	}
}

bool GIS_TextureGenerator::Render(GIS_PathData* pathData, GIS_POLYGON_RENDER_INFO renderInfo)
{
	Q_ASSERT(_Pixmap!=NULL);

	QImage* texture=NULL;
	
	QString texturename=QString::fromUtf16(renderInfo.polyTextureName);

	if(!texturename.isEmpty())
		texture=GetTexture(g_app->GetWorkDir()+texturename);

	QRectF pathBound = pathData->GetPath()->boundingRect();

	if(!_RenderBound.contains(pathBound) && !_RenderBound.intersects(pathBound))
		return false;

	bool blContains = pathData->Contains(_RenderBound);
	bool blChangeContainInfo = false;

	if(!blContains && _ContainComplete)
	{
		_ContainComplete = false;
		blChangeContainInfo = true;
	}

	if(!blContains && !pathData->Intersect(_RenderBound))
		return false;

	unsigned int clr = renderInfo.fillColor;
	QColor color = QColor((clr >> 16) & 0xff, (clr >> 8) & 0xff, (clr) & 0xff, (clr >> 24) & 0xff);

	if(_ContainComplete)
	{
		if(_RenderEmpty)
			_ColorPixmap->fill(color);
	}
	else
	{
		if(blChangeContainInfo)
			*_Pixmap = _ColorPixmap->scaled(_TextureWidth, _TextureHeight);

		QPainter painter(_Pixmap);
		painter.setRenderHint(QPainter::Antialiasing);
		if(blContains)
		{
			if(texture)
			{
				painter.drawImage(0,0,*texture);
			}
			else
				painter.fillRect(0, 0, _TextureWidth, _TextureHeight, color);
		}
		else
		{
			QMatrix mat2;
			mat2.scale(_TextureWidth / _RenderBound.width(), _TextureWidth / _RenderBound.height());
			mat2.translate(-_RenderBound.x(), -_RenderBound.y());

			QPainterPath path = mat2.map(*pathData->GetPath());

			painter.setBrush(QBrush(color));

			if(renderInfo.strokeThickness == 0)
				painter.setPen(Qt::NoPen);
			else
			{
				clr = renderInfo.strokeColor;
				color = QColor((clr >> 16) & 0xff, (clr >> 8) & 0xff, (clr) & 0xff, (clr >> 24) & 0xff);
				painter.setPen(QPen(color, renderInfo.strokeThickness, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
			}

			if(texture)
			{
				painter.setClipPath(path);
				painter.drawImage(0,0,*texture);
			}
			else
				painter.drawPath(path);
		}
	}

	_RenderEmpty = false;

	return true;
}

//precondition 
//polygonË¼ _renderboundËæ ºÐÂÙ³Þ.
//_Pixmap²÷ Ë±Ã£Â×³Þ.

bool GIS_TextureGenerator::RenderShapeData(LineSegment* polygon,QColor color,int width)
{
	QMatrix mat2;
	mat2.scale(_TextureWidth / _RenderBound.width(), _TextureWidth / _RenderBound.height());
	mat2.translate(-_RenderBound.x(), -_RenderBound.y());

	QPainter painter(_Pixmap);
	painter.setPen(QPen(color, width, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
	painter.setRenderHint(QPainter::Antialiasing);

	qreal x,y;

	array<qreal> xarr,yarr;
	for(unsigned int i=0;i<polygon->pts.size();i++)
	{
		mat2.map((qreal)polygon->pts[i].X,(qreal)polygon->pts[i].Y,&x,&y);
		xarr.push_back(x);
		yarr.push_back(y);
	}

	for(unsigned int i=0;i<polygon->pts.size()-1;i++)
	{
		painter.drawLine(xarr[i],yarr[i],
			             xarr[i+1],yarr[i+1]);

	}

	this->_RenderEmpty=false;
	this->_ContainComplete=false;
	return true;
}
