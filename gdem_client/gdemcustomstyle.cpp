#include "gdemcustomstyle.h"

static bool UsePixmapCache = true;

GDEMCustomStyle::GDEMCustomStyle()
{
}

GDEMCustomStyle::~GDEMCustomStyle()
{
}

static QLinearGradient qMapGradientToRect(const QLinearGradient &gradient, const QRectF &rect)
{
    QLinearGradient tmpGrad(rect.center().x(), rect.top(),
                            rect.center().x(), rect.bottom());
    tmpGrad.setStops(gradient.stops());
    return tmpGrad;
}

static QBrush qMapBrushToRect(const QBrush &brush, const QRectF &rect)
{
    if (!brush.gradient())
        return brush;

    // ### Ugly assumption that it's a linear gradient
    QBrush tmp(qMapGradientToRect(*static_cast<const QLinearGradient *>(brush.gradient()), rect));
    return tmp;
}

static void qBrushSetAlphaF(QBrush *brush, qreal alpha)
{
    if (const QGradient *gradient = brush->gradient()) {
        // Use the gradient. Call QColor::setAlphaF() on all color stops.
        QGradientStops stops = gradient->stops();
        QMutableVectorIterator<QGradientStop> it(stops);
        QColor tmpColor;
        while (it.hasNext()) {
            it.next();
            tmpColor = it.value().second;
            tmpColor.setAlphaF(alpha * tmpColor.alphaF());
            it.setValue(QPair<qreal, QColor>(it.value().first, tmpColor));
        }

        switch (gradient->type()) {
        case QGradient::RadialGradient: {
            QRadialGradient grad = *static_cast<const QRadialGradient *>(gradient);
            grad.setStops(stops);
            *brush = QBrush(grad);
            break;
        }
        case QGradient::ConicalGradient: {
            QConicalGradient grad = *static_cast<const QConicalGradient *>(gradient);
            grad.setStops(stops);
            *brush = QBrush(grad);
            break;
        }
        default:
            qWarning("CustomStyle::qBrushLight() - unknown gradient type"
                     " - falling back to QLinearGradient");
        case QGradient::LinearGradient: {
            QLinearGradient grad = *static_cast<const QLinearGradient *>(gradient);
            grad.setStops(stops);
            *brush = QBrush(grad);
            break;
        }
        }
    } else if (!brush->texture().isNull()) {
        // Modify the texture - ridiculously expensive.
        QPixmap texture = brush->texture();
        QPixmap pixmap;
        QString name = QString::fromLatin1("qbrushtexture-alpha-%1-%2").arg(alpha).arg(texture.cacheKey());
        if (UsePixmapCache && !QPixmapCache::find(name, pixmap)) {
            QImage image = texture.toImage();
            QRgb *rgb = reinterpret_cast<QRgb *>(image.bits());
            int pixels = image.width() * image.height();
            QColor tmpColor;
            while (pixels--) {
                tmpColor.setRgb(*rgb);
                tmpColor.setAlphaF(alpha * tmpColor.alphaF());
                *rgb++ = tmpColor.rgba();
            }
            pixmap = QPixmap::fromImage(image);
            QPixmapCache::insert(name, pixmap);
        }
        brush->setTexture(pixmap);
    } else {
        // Use the color
        QColor tmpColor = brush->color();
        tmpColor.setAlphaF(alpha * tmpColor.alphaF());
        brush->setColor(tmpColor);
    }
}

static QColor mergedColors(const QColor &colorA, const QColor &colorB, int factor = 50)
{
    const int maxFactor = 100;
    QColor tmp = colorA;
    tmp.setRed((tmp.red() * factor) / maxFactor + (colorB.red() * (maxFactor - factor)) / maxFactor);
    tmp.setGreen((tmp.green() * factor) / maxFactor + (colorB.green() * (maxFactor - factor)) / maxFactor);
    tmp.setBlue((tmp.blue() * factor) / maxFactor + (colorB.blue() * (maxFactor - factor)) / maxFactor);
    return tmp;
}

static QString uniqueName(const QString &key, const QStyleOption *option, const QSize &size)
{
    QString tmp;
    const QStyleOptionComplex *complexOption = qstyleoption_cast<const QStyleOptionComplex *>(option);
    tmp.sprintf("%s-%d-%d-%d-%lld-%dx%d", key.toLatin1().constData(), uint(option->state),
                option->direction, complexOption ? uint(complexOption->activeSubControls) : uint(0),
                option->palette.cacheKey(), size.width(), size.height());
    return tmp;
}

void GDEMCustomStyle::polish(QWidget *widget)
{
#ifdef Q_WS_MAC
	QMacStyle::polish(widget);

	if (qobject_cast<QMenu*>(widget) || qobject_cast<QComboBox *>(widget)) {
		widget->setWindowOpacity(0.94);

		if (!widget->testAttribute(Qt::WA_SetPalette)) {
			QPixmap px(64, 64);
			px.fill(Qt::white);
			
			//HIThemeMenuDrawInfo mtinfo;
			//mtinfo.version = 0;
			//mtinfo.menuType = kThemeMenuTypePopUp;
			//HIRect rect = CGRectMake(0, 0, px.width(), px.height());
			//HIThemeDrawMenuBackground(&rect, &mtinfo, QCFType<CGContextRef>(qt_mac_cg_context(&px)),
										// kHIThemeOrientationNormal);
			
			QPalette pal = widget->palette();
			QBrush background(px);
			pal.setBrush(QPalette::All, QPalette::Window, background);
			pal.setBrush(QPalette::All, QPalette::Button, background);
			widget->setPalette(pal);
			widget->setAttribute(Qt::WA_SetPalette, false);
		}
	}
#else
	QPlastiqueStyle::polish(widget);
#endif

#ifndef Q_WS_WIN
	if (qobject_cast<QLineEdit *>(widget)
		|| qobject_cast<QComboBox *>(widget)
		|| qobject_cast<QAbstractSpinBox *>(widget)
		|| qobject_cast<QTextEdit *>(widget)
		|| qobject_cast<QPlainTextEdit *>(widget))
	{
		widget->setContextMenuPolicy(Qt::NoContextMenu);
	}
#endif
}

void GDEMCustomStyle::polish(QPalette &palette)
{
    QWindowsStyle::polish(palette);
    palette.setBrush(QPalette::AlternateBase, palette.base().color().darker(110));
#ifdef Q_WS_MAC
    palette.setBrush(QPalette::Shadow, Qt::black);
#endif

	QColor backgroundColor(228, 228, 228);
    palette = QPalette(backgroundColor);
	QBrush brush(backgroundColor, Qt::SolidPattern);

#ifdef Q_WS_MAC
    setTexture(palette, QPalette::Window, brush);
#else
    setTexture(palette, QPalette::Button, brush);
    setTexture(palette, QPalette::Mid, brush);
    setTexture(palette, QPalette::Window, brush);

    brush = palette.background();
	brush.setColor(brush.color().darker(110));

    //palette.setBrush(QPalette::Disabled, QPalette::WindowText, brush);
    //palette.setBrush(QPalette::Disabled, QPalette::Text, brush);
    //palette.setBrush(QPalette::Disabled, QPalette::ButtonText, brush);
    palette.setBrush(QPalette::Disabled, QPalette::Base, brush);
    palette.setBrush(QPalette::Disabled, QPalette::Button, brush);
    palette.setBrush(QPalette::Disabled, QPalette::Mid, brush);
#endif
}

int GDEMCustomStyle::styleHint(StyleHint hint, const QStyleOption *option, const QWidget *widget,
                               QStyleHintReturn *returnData) const
{
	int ret = 0;
    switch (hint) 
	{
		case SH_ItemView_ShowDecorationSelected:
			ret = 0;
			break;
		default:
#ifdef Q_WS_MAC
	        ret = QMacStyle::styleHint(hint, option, widget, returnData);
#else
			ret = QPlastiqueStyle::styleHint(hint, option, widget, returnData);
#endif
			break;
	}
	
	return ret;
}

void GDEMCustomStyle::drawPrimitive(PrimitiveElement element, const QStyleOption *option,
                                    QPainter *painter, const QWidget *widget) const
{
    Q_ASSERT(option);
    
#ifdef Q_WS_MAC
        QMacStyle::drawPrimitive(element, option, painter, widget);
        return;
#else
    QColor borderColor = option->palette.background().color().darker(178);
    QColor gradientStartColor = option->palette.button().color().lighter(104);
    QColor gradientStopColor = option->palette.button().color().darker(105);
    QColor baseGradientStartColor = option->palette.base().color().darker(101);
    QColor baseGradientStopColor = option->palette.base().color().darker(106);
    QColor highlightedGradientStartColor = option->palette.button().color().lighter(101);
    QColor highlightedGradientStopColor = mergedColors(option->palette.button().color(), option->palette.highlight().color(), 85);
    QColor highlightedBaseGradientStartColor = option->palette.base().color();
    QColor highlightedBaseGradientStopColor = mergedColors(option->palette.base().color().darker(105), option->palette.highlight().color(), 70);
    QColor highlightedDarkInnerBorderColor = mergedColors(option->palette.button().color(), option->palette.highlight().color(), 35);
    QColor highlightedLightInnerBorderColor = mergedColors(option->palette.button().color(), option->palette.highlight().color(), 58);
    QColor alphaCornerColor;
    if (widget) {
        // ### backgroundrole/foregroundrole should be part of the style option
        alphaCornerColor = mergedColors(option->palette.color(widget->backgroundRole()), borderColor);
    } else {
        alphaCornerColor = mergedColors(option->palette.background().color(), borderColor);
    }
    QColor alphaInnerColor = mergedColors(highlightedLightInnerBorderColor, gradientStartColor);
    QColor alphaInnerColorNoHover = mergedColors(borderColor, gradientStartColor);
    QColor alphaTextColor = mergedColors(option->palette.background().color(), option->palette.text().color());
    QColor alphaLightTextColor = mergedColors(option->palette.background().color().lighter(250), option->palette.text().color().lighter(250));
    QColor lightShadow = option->palette.button().color().lighter(105);
    QColor shadowGradientStartColor = option->palette.button().color().darker(115);
    QColor shadow = shadowGradientStartColor;

    switch (element) {
    case PE_IndicatorCheckBox:
        if (const QStyleOptionButton *button = qstyleoption_cast<const QStyleOptionButton *>(option)) {
            //BEGIN_STYLE_PIXMAPCACHE(QLatin1String("checkbox"))

			QRect rect = option->rect;
			QPixmap internalPixmapCache;
			QImage imageCache;
			QPainter *p = painter;
			//QString unique = uniqueName((QLatin1String("checkbox")), option, option->rect.size());
			//int txType = painter->deviceTransform().type() | painter->worldTransform().type();
			//bool doPixmapCache = UsePixmapCache && txType <= QTransform::TxTranslate;
			//if (doPixmapCache && QPixmapCache::find(unique, internalPixmapCache)) 
			//{
			//	painter->drawPixmap(option->rect.topLeft(), internalPixmapCache);
			//}
			//else 
			//{
			//	if (doPixmapCache)
			//	{
			//		rect.setRect(0, 0, option->rect.width(), option->rect.height());
			//		imageCache = QImage(option->rect.size(), QImage::Format_ARGB32_Premultiplied);
			//		imageCache.fill(0);
			//		p = new QPainter(&imageCache);
			//	}
			//}

            p->save();

            // Outline
            QBrush border = option->palette.shadow();
            qBrushSetAlphaF(&border, qreal(0.4));
            p->setPen(QPen(border, 0));
            const QLine lines[4] = {
                QLine(rect.left() + 1, rect.top(), rect.right() - 1, rect.top()),
                QLine(rect.left() + 1, rect.bottom(), rect.right() - 1, rect.bottom()),
                QLine(rect.left(), rect.top() + 1, rect.left(), rect.bottom() - 1),
                QLine(rect.right(), rect.top() + 1, rect.right(), rect.bottom() - 1) };
            p->drawLines(lines, 4);

            QBrush corner = option->palette.shadow();
            qBrushSetAlphaF(&corner, qreal(0.2));
            p->setPen(QPen(corner, 0));
            const QPoint points[4] = {
                rect.topLeft(), rect.topRight(),
                rect.bottomLeft(), rect.bottomRight() };
            p->drawPoints(points, 4);

            // Fill
            QBrush baseBrush = qMapBrushToRect(button->palette.base(), rect);
            if (!baseBrush.gradient() && baseBrush.texture().isNull()) {
                QLinearGradient gradient(rect.center().x(), rect.top(), rect.center().x(), rect.bottom());
                gradient.setColorAt(0, baseBrush.color());
                gradient.setColorAt(1, baseBrush.color().darker(105));
                baseBrush = gradient;
            }
            p->fillRect(rect.adjusted(1, 1, -1, -1), baseBrush);

            // Hover
            if ((button->state & State_Enabled) && (button->state & State_MouseOver)) {
                QBrush pen = qMapBrushToRect(button->palette.highlight(), rect);
                qBrushSetAlphaF(&pen, qreal(0.8));
                p->setPen(QPen(pen, 0));
                p->drawRect(rect.adjusted(1, 1, -2, -2));
                qBrushSetAlphaF(&pen, 0.5);
                p->setPen(QPen(pen, 0));
                p->drawRect(rect.adjusted(2, 2, -3, -3));

                qBrushSetAlphaF(&pen, qreal(0.2));
                p->setBrush(pen);
                p->drawRect(rect.adjusted(2, 2, -3, -3));
            }

            // Indicator
            bool on = button->state & State_On;
            bool sunken = button->state & State_Sunken;
            bool unchanged = button->state & State_NoChange;
            bool enabled = button->state & State_Enabled;
            if (on || (enabled && sunken) || unchanged) {
                p->setRenderHint(QPainter::Antialiasing);
                QBrush pointBrush = qMapBrushToRect(button->palette.text(), rect);
                if (sunken)
                    qBrushSetAlphaF(&pointBrush, qreal(0.5));
                else if (unchanged)
                    qBrushSetAlphaF(&pointBrush, qreal(0.3));
				//p->setPen(QPen(pointBrush, ));
                p->setPen(QPen(pointBrush, 2));
                const QLine lines[2] = {
					// x check mark
                    //QLine(rect.left() + 4, rect.top() + 4, rect.right() - 3, rect.bottom() - 3),
                    //QLine(rect.right() - 3, rect.top() + 4, rect.left() + 4, rect.bottom() - 3) };
					// standard type mark
					QLine(rect.left() + 4, rect.top() + 5, rect.left() + 5, rect.bottom() - 4),
                    QLine(rect.right() - 3, rect.top() + 4, rect.left() + 5, rect.bottom() - 4) };
                p->drawLines(lines, 2);
            }

            p->restore();
            
			//if (doPixmapCache)
			//{
			//	p->end();
			//	delete p;
			//	internalPixmapCache = QPixmap::fromImage(imageCache);
			//	painter->drawPixmap(option->rect.topLeft(), internalPixmapCache);
			//	QPixmapCache::insert(unique, internalPixmapCache);
			//}
        }
        break;
    default:
		QPlastiqueStyle::drawPrimitive(element, option, painter, widget);
        break;
    }
#endif
}

void GDEMCustomStyle::setTexture(QPalette &palette, QPalette::ColorRole role,
                                    const QBrush &brush)
{
    for (int i = 0; i < QPalette::NColorGroups; ++i) {
        QColor color = palette.brush(QPalette::ColorGroup(i), role).color();
        palette.setBrush(QPalette::ColorGroup(i), role, brush);
    }
}
