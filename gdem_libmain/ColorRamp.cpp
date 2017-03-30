#include "ColorRamp.h"
#include <QtGui/QImage>

IColorRamp::IColorRamp()
{
	_name="Unknown";
	_size=0;
}

AlgorithmicColorRamp::AlgorithmicColorRamp()
{
	_type=Algorithmic;
	_fromColor=QColor(Qt::black);
	_toColor=QColor(Qt::white);
	_algorithm=HSV;
};

AlgorithmicColorRamp::AlgorithmicColorRamp(Qt::GlobalColor f,Qt::GlobalColor t,QString name/* = */)
{
	_fromColor=QColor(f);
	_toColor=QColor(t);
	_name=name;
	_size=256;
	CreateRamp();
}


void AlgorithmicColorRamp::CreateRamp()
{
	if(_size==0)
		qFatal("must be setted size!");
	
	qreal fromh,froms,fromv;

	_fromColor.getHsvF(&fromh,&froms,&fromv);

	qreal toh,tos,tov;

	_toColor.getHsvF(&toh,&tos,&tov);
	
	qreal delh,dels,delv;

	delh=toh-fromh;
	dels=tos-froms;
	delv=tov-fromv;

	qreal h,s,v;

	for(int i=0;i<_size;i++)
	{
		h=fromh+delh/_size*i;
		s=froms+dels/_size*i;
		v=fromv+delv/_size*i;

		QColor c=QColor::fromHsvF(h,s,v);
		_colorRamp.push_back(c);
	}
}

QColor AlgorithmicColorRamp::Color(int index)
{
	if(_colorRamp.size()==0)
	{
		qFatal("Create ColorRamp!");
		return QColor(Qt::black);
	}

	Q_ASSERT(index>=0 && index<_size);

	return _colorRamp[index];
}

QPixmap AlgorithmicColorRamp::ToPixmap()//for layerdlg
{
	if(_colorRamp.size()==0)
	{
		qFatal("Create ColorRamp!");
		return QPixmap();
	}

	int height=10;

	QImage im(_size,height,QImage::Format_RGB32);

	for (int x=0;x<_size;x++)
	{
		QColor c=Color(x);

		for(int y=0;y<height;y++)
		{
			im.setPixel(x,y,c.rgb());
		}
	}

	return QPixmap::fromImage(im);
}

void AlgorithmicColorRamp::MakeImage(QString path)
{
	if(_colorRamp.size()==0)
	{
		qFatal("Create ColorRamp!");
		return;
	}
	
	int height=10;

	QImage im(_size,height,QImage::Format_RGB32);

	for (int x=0;x<_size;x++)
	{
		QColor c=Color(x);

		for(int y=0;y<height;y++)
		{
			im.setPixel(x,y,c.rgb());
		}
	}

	im.save(path);
}

QImage AlgorithmicColorRamp::GetImage() //for terrainsecne render
{
	int width=10;

	QImage im(width,_size,QImage::Format_RGB32);

	for (int y=0;y<_size;y++)
	{
		QColor c=Color(y);

		for(int x=0;x<width;x++)
			im.setPixel(x,y,c.rgb());
	}

	return im;
}