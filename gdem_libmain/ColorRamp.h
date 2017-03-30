#ifndef COLORRAMP_H
#define COLORRAMP_H

#include <QtCore/QList>
#include <QtGui/QImage>
#include <QtGui/QPixmap>

class IColorRamp
{
public:
	enum ColorRampType
	{
		Algorithmic,
	};

	IColorRamp();
	virtual QColor Color(int index)=0;
	virtual void   CreateRamp()=0;
	virtual QImage GetImage()=0;   
	
	QString GetName(){return _name;}
	int     GetSize(){return _size;}
	void    SetName(QString s){_name=s;}
	void    SetSize(int i)   {_size=i;}

	void   SetType(ColorRampType type) {_type=type;}
	ColorRampType GetType() {return _type;}
	
protected:
	int            _size;
	QString        _name;
	QList<QColor>  _colorRamp;
	ColorRampType   _type;
};

class AlgorithmicColorRamp :public IColorRamp
{
public:
	enum AlgorithmType
	{
		HSV,
	};

	AlgorithmicColorRamp();
	AlgorithmicColorRamp(Qt::GlobalColor f,Qt::GlobalColor t,QString name="Unkonwn");
	
	virtual QColor Color(int index);
	virtual void CreateRamp();
	virtual QImage GetImage();

	QColor  FromColor() {return _fromColor;}
	QColor  ToColor()   {return _toColor;}

	void    SetFromColor(QColor c){_fromColor=c;}
	void    SetToColor(QColor c) {_toColor=c;}

	QPixmap ToPixmap();
	void    MakeImage(QString path);

private:
	AlgorithmType _algorithm;
	QColor        _fromColor;
	QColor        _toColor;
};

#endif