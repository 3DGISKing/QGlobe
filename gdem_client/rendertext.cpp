#include "gdemclient.h"
#include "gdemclientbase.h"
#include "config.h"
#include "proxy.h"
#include "guiuty.h"
#include "rendertext.h"

inline int MakeKey(unsigned short code, int fontSize, int thick)
{
	return ((unsigned int)code*10000 + fontSize*100 + thick);
}

inline bool IsSpace(unsigned short code)
{
	return (code==0x20) ? true : false;
}

/*
 * public
 */
RenderText::RenderText()
{
	m_view = NULL;
	m_bodyOpacity = 1.0;
	m_outOpacity = 0.6;
	m_fontFamily.clear();
	m_fontBold = false;
	m_fontItalic = false;
	m_fontImg = NULL;
}

bool RenderText::Init()
{
	CreateFontImage(60, 40);
	return true;
}

void RenderText::Release()
{
	ReleaseRenderChar();

	if (m_fontImg)
		delete m_fontImg;
	m_fontImg = NULL;
}

void RenderText::ReleaseRenderChar()
{
	RenderCharMap::iterator i = m_renderChars.begin();

	while (i != m_renderChars.end())
	{
		RenderChar rchar = i.value();
		if (rchar.body)
			delete rchar.body;
		if (rchar.outline)
			delete rchar.outline;
		++i;
	}

	m_renderChars.clear();
}

bool RenderText::DrawText(QPainter *painter, const QRect &rect, int flags, const QString &text,
						  int thick, const QPen &bodyPen, const QPen &outPen)
{
	QRect textRect;
	if (!GetBoundingRect(painter, rect, flags, text, thick, &textRect))
		return false;

	int x = textRect.left();
	int y = textRect.top();

	return DrawText(painter, x, y, text, thick, bodyPen, outPen);
}

bool RenderText::DrawText(QPainter *painter, int x, int y, const QString &text,
						  int thick, const QPen &bodyPen, const QPen &outPen)
{
	const QChar *data = text.data();
	RenderChar rchar;
	const QFont &font = painter->font();
	
	int i;
	for (i = 0; i < text.length() != 0; i ++)
	{
		ushort code = data[i].unicode();
		if (!GetRenderChar(font, code, thick, &rchar))
			break;
		DrawRenderChar(painter, x, y, &rchar, bodyPen, outPen);
		x += rchar.pro.cx;
		if (i < text.length()-1)
			x -= 2*rchar.pro.thick;
	}

	return (i == text.length()) ? true : false;
}

bool RenderText::GetBoundingRect(QPainter *painter, const QRect &rect, const int flags,
							 const QString &text, const int thick, QRect *textRect)
{
	const QFont &font = painter->font();
	RenderChar rchar;
	const QChar *data = text.data();

	int i;
	int cx = 0;
	int cy = 0;

	for (i = 0; i < text.length(); i ++)
	{
		ushort code = data[i].unicode();

		if (!GetRenderChar(font, code, thick, &rchar))
			break;
		if (rchar.pro.cy > cy)
			cy = rchar.pro.cy;
		cx += rchar.pro.cx;
		if (i < text.length()-1)
			cx -= 2*rchar.pro.thick;
	}

	if (i < text.length())
		return false;

	QRect trect(0, 0, cx, cy);
	trect.moveTo(rect.left(), rect.top());

	if (flags & Qt::AlignRight)
		trect.moveRight(rect.right());
	else if (flags & Qt::AlignHCenter)
		trect.moveLeft(rect.left()+(rect.width()-trect.width())/2);
	if (flags & Qt::AlignBottom)
		trect.moveBottom(rect.bottom());
	else if (flags & Qt::AlignVCenter)
		trect.moveTop(rect.top()+(rect.height()-trect.height())/2);

	if (textRect)
		*textRect = trect;
	return true;
}

/*
 * protected
 */
void RenderText::DrawRenderChar(QPainter *painter, int x, int y, RenderChar *rchar,
								const QPen &bodyPen, const QPen &outPen)
{
	if (!painter)
		return;

	if (rchar->outline)
	{
		painter->setOpacity(m_outOpacity);
		painter->setPen(outPen);
		DrawCharSpan(painter, x - rchar->pro.x, y, rchar->outline);
	}
	if (rchar->body)
	{
		painter->setOpacity(m_bodyOpacity);
		painter->setPen(bodyPen);
		DrawCharSpan(painter, x - rchar->pro.x + rchar->pro.thick, y + rchar->pro.thick, rchar->body);
	}
}

void RenderText::DrawCharSpan(QPainter *painter, int x, int y, SpanVector *vector)
{
	if (painter && vector)
	{
		for (int i = 0; i < vector->size(); ++i)
		{
			const SpanData &s = vector->at(i);
			painter->drawLine(x+s.x, y+s.y, x+s.x+s.cx-1, y+s.y);
		}
	}
}

bool RenderText::CheckFont(const QFont &font)
{
	if (m_fontFamily != font.family())
		return false;
	if (m_fontBold != font.bold())
		return false;
	if (m_fontItalic != font.italic())
		return false;

	return true;
}

void RenderText::SetFont(const QFont &font)
{
	m_fontFamily = font.family();
	m_fontBold = font.bold();
	m_fontItalic = font.italic();
}

bool RenderText::CreateFontImage(int width, int height)
{
	int w = width;
	int h = height;

	if (m_fontImg)
	{
		if (m_fontImg->width() >= w && m_fontImg->height() >= h)
			return false;

		w = qMax(m_fontImg->width(), width);
		h = qMax(m_fontImg->height(), height);

		delete m_fontImg;
	}

	m_fontImg = new QImage(w, h, QImage::Format_RGB888);

	return true;
}

bool RenderText::GetRenderChar(const QFont &font, unsigned short code, int thick, RenderChar *rchar)
{
	if (!CheckFont(font))
	{
		ReleaseRenderChar();
		SetFont(font);
	}

	int key = MakeKey(code, font.pointSize(), thick);
	RenderCharMap::iterator i = m_renderChars.find(key);
	if (i != m_renderChars.end())
	{
		*rchar = i.value();
		return true;
	}

	MakeRenderChar(font, code, thick, rchar);
	return true;
}

#define PATT_COLOR		qRgb(255,255,255)

static void AddSpan(int x, int y, int cx, SpanVector *vector, RenderCharProperty *pro)
{
	SpanData s;
	memset(&s, 0, sizeof(s));
	s.x = x;
	s.y = y;
	s.cx = cx;
	vector->append(s);

	if (pro->cy < y + 1)
		pro->cy = y + 1;
}

static void CheckSpan(const QImage *img, const QRect *rect, SpanVector *vector, RenderCharProperty *pro)
{
	int r = qRed(PATT_COLOR) / 2;
	int g = qGreen(PATT_COLOR) / 2;
	int b = qBlue(PATT_COLOR) / 2;

	for (int y = rect->top(); y <= rect->bottom(); y ++)
	{
		int sx = -1;
		int cx = 0;
		for (int x = rect->left(); x <= rect->right(); x ++)
		{
			QRgb color = img->pixel(x, y);

			//if (PATT_COLOR == img->pixel(x,y))
			if (r < qRed(color) && g < qGreen(color) && b < qBlue(color))
			{
				if (sx == -1)
					sx = x;
				cx ++;
			}
			else
			{
				if (sx >= 0 && cx > 0)
					AddSpan(sx, y, cx, vector, pro);

				sx = -1;
				cx = 0;
			}
		}
		if (sx >= 0 && cx > 0)
			AddSpan(sx, y, cx, vector, pro);
	}
}

static void SetProperty(const QFont &font, int width, int height, unsigned short code,
						int thick, RenderCharProperty *pro)
{
	pro->height = font.pointSize();
	pro->code = code;
	pro->thick = thick;
	pro->x = 0;
	pro->cx = width + 2*thick;
	pro->cy = height + 2*thick;
}

void RenderText::MakeRenderChar(const QFont &font, unsigned short code, int thick, RenderChar *rchar1)
{
	QString text = QString::fromUtf16(&code, 1);

	//calc bound rect
	QPainter p(m_fontImg);
	QRect rect(0, 0, m_fontImg->width(), m_fontImg->height());
	QColor foreColor(PATT_COLOR);
	QPen pen(foreColor);

	p.setFont(font);
	p.setPen(pen);
	QRect boundRect = p.boundingRect(rect, Qt::AlignLeft | Qt::AlignTop, text);
	p.end();

	//set property of render char
	RenderChar rchar;
	memset(&rchar, 0, sizeof(rchar));
	SetProperty(font, boundRect.width(), boundRect.height(), code, thick, &rchar.pro);
	rchar.pro.cy = 0;

	if (!IsSpace(code))
	{
		boundRect.setWidth(boundRect.width()*15/10+2*thick);
		boundRect.setHeight(boundRect.height()+2*thick);

		CreateFontImage(boundRect.width(), boundRect.height());
		MakeCharSpan(font, boundRect, text, thick, &rchar);
	}

	//check fount
	int key = MakeKey(code, font.pointSize(), thick);
	m_renderChars.insert(key, rchar);

	if (rchar1)
		*rchar1 = rchar;
}

void RenderText::MakeCharSpan(const QFont &font, const QRect &boundRect, const QString &text, int thick, RenderChar *rchar)
{
	QRect rect(0, 0, m_fontImg->width(), m_fontImg->height());
	QPainter p(m_fontImg);
	QPainter *painter = &p;
	int flags = Qt::AlignLeft | Qt::AlignTop;
	QColor bkColor(qRgb(0,0,0));
	QColor foreColor(PATT_COLOR);
	QPen pen(foreColor);

	painter->setFont(font);
	painter->setPen(pen);

	rchar->body = new SpanVector;
	rchar->outline = new SpanVector;

	painter->setOpacity(1.0);
	painter->setPen(Qt::NoPen);
	painter->fillRect(rect, bkColor);
	painter->setBackgroundMode(Qt::TransparentMode);
	painter->setPen(pen);
	painter->drawText(rect, flags, text);

	CheckSpan(m_fontImg, &boundRect, rchar->body, &rchar->pro);

	for (int row = 0; row <= 2*thick; row ++)
	{
		for (int col = 0; col <= 2*thick; col ++)
		{
			if (row != thick || col != thick)
				DrawCharSpan(painter, col, row, rchar->body);
		}
	}

	CheckSpan(m_fontImg, &boundRect, rchar->outline, &rchar->pro);
}
