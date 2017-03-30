#ifndef RENDERTEXT_H
#define RENDERTEXT_H

typedef struct _SpanData
{
	int x;
	int y;
	int cx;
} SpanData;

typedef QVector<SpanData> SpanVector;

typedef struct _RenderCharProperty
{
	int height;
	unsigned short code;
	int thick;
	int x;
	int cx;
	int cy;
} RenderCharProperty;

typedef struct _RenderChar
{
	RenderCharProperty	pro;
	SpanVector			*body;
	SpanVector			*outline;
} RenderChar;

typedef QMap<int, RenderChar> RenderCharMap;

class RenderText
{
public:
    RenderText();

	void SetView(QWidget *view)
	{   m_view = view; }

	void SetOpacity(double body, double outline)
	{
		m_bodyOpacity = body;
		m_outOpacity = outline;
	}

	bool Init();
	void Release();
	void ReleaseRenderChar();

	bool DrawText(QPainter *painter, const QRect &rect, int flags, const QString &text,
				  int thick, const QPen &bodyPen, const QPen &outPen);
	bool DrawText(QPainter *painter, int x, int y, const QString &text,
				  int thick, const QPen &bodyPen, const QPen &outPen);
	bool GetBoundingRect(QPainter *painter, const QRect &rect, const int flags,
					 const QString &text, const int thick, QRect *textRect);
protected:
	QWidget *m_view;
	double m_bodyOpacity, m_outOpacity;

	QString m_fontFamily;
	bool m_fontBold;
	bool m_fontItalic;
	QImage *m_fontImg;

	RenderCharMap m_renderChars;

	void DrawRenderChar(QPainter *painter, int x, int y, RenderChar *rchar,
						const QPen &bodyPen, const QPen &outPen);
	void DrawCharSpan(QPainter *painter, int x, int y, SpanVector *vector);
	bool CheckFont(const QFont &font);
	void SetFont(const QFont &font);
	bool CreateFontImage(int width, int height);
	bool GetRenderChar(const QFont &font, unsigned short code, int thick, RenderChar *rchar);
	void MakeRenderChar(const QFont &font, unsigned short code, int thick, RenderChar *rchar=NULL);
	void MakeCharSpan(const QFont &font, const QRect &boundRect, const QString &text, int thick, RenderChar *rchar);
};

#endif // RENDERTEXT_H
