#ifndef GDEMCUSTOMSTYLE_H
#define GDEMCUSTOMSTYLE_H

#include <QPlastiqueStyle>

#ifdef Q_WS_MAC
#include <QMacStyle>
class GDEMCustomStyle : public QMacStyle
#else
class GDEMCustomStyle : public QPlastiqueStyle
#endif
{
public:
	GDEMCustomStyle();
	~GDEMCustomStyle();

	void polish(QWidget *widget);
	void polish(QPalette &palette);
	int styleHint(StyleHint hint, const QStyleOption *option, const QWidget *widget,
                               QStyleHintReturn *returnData) const;
	void drawPrimitive(PrimitiveElement element, const QStyleOption *option,
                       QPainter *painter, const QWidget *widget) const;
	void setTexture(QPalette &palette, QPalette::ColorRole role, const QBrush &brush);

};


#endif GDEMCUSTOMSTYLE_H
