#ifndef COLORXMLPARSER_H
#define	COLORXMLPARSER_H

#include <QtXml/QXmlDefaultHandler>
#include <QString>
#include <QVector>

#define GEO_SHAPE_COUNT 18

struct ShapeAttribute
{
	int            id;
	unsigned int   color;
	float          width;
};
class ColorXmlParser : public QXmlDefaultHandler
{
public:
	ColorXmlParser();

	bool startElement(const QString &namespaceURI,
						const QString &localName,
						const QString &qName,
						const QXmlAttributes &attribs);
	bool endElement(const QString &namespaceURI,
					const QString &localName,
					const QString &qName);
	bool characters(const QString &str);
	bool fatalError(const QXmlParseException &exception);
	void GetAttributeList(QVector<ShapeAttribute> *outList);
	void SetAttributeList(ShapeAttribute *outList);

private:
	QVector<ShapeAttribute> m_shpAttrList;

};
#endif
