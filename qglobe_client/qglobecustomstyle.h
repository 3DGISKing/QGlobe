#ifndef QGLOBECUSTOMSTYLE_H
#define QGLOBECUSTOMSTYLE_H

#include <QProxyStyle>

#ifdef Q_WS_MAC
#include <QMacStyle>
class QGlobeCustomStyle : public QMacStyle
#else
class QGlobeCustomStyle : public QProxyStyle
#endif
{
public:
	QGlobeCustomStyle();
	~QGlobeCustomStyle();

	void polish(QWidget *widget);
	void polish(QPalette &palette);
	int styleHint(StyleHint hint, const QStyleOption *option, const QWidget *widget,
                               QStyleHintReturn *returnData) const;
	void drawPrimitive(PrimitiveElement element, const QStyleOption *option,
                       QPainter *painter, const QWidget *widget) const;
	void setTexture(QPalette &palette, QPalette::ColorRole role, const QBrush &brush);

};


#endif // QGLOBECUSTOMSTYLE_H
