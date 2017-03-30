#include "colorXmlParser.h"


ColorXmlParser::ColorXmlParser()
{
	m_shpAttrList.clear();
}

bool ColorXmlParser::startElement(const QString &, const QString &,
								   const QString &qName,
								   const QXmlAttributes &attribs)
{
	QString strVal;
	bool ok;
	int color;
	float width;
	if (qName == "Layer")
	{	
		ShapeAttribute element;

		strVal = attribs.value("color");
		element.color = strVal.toUInt(&ok, 10);
		if (!ok) element.color = 0;	
		
		strVal = attribs.value("width");
		element.width = strVal.toFloat(&ok);
		if (!ok) element.width = 1.0f;

		strVal = attribs.value("id");
		element.id  = strVal.toInt(&ok);
		if (!ok) element.id = 1000;

		m_shpAttrList.push_back(element);
	}
	return true;

}
void ColorXmlParser::GetAttributeList(QVector<ShapeAttribute> *outList)
{
	if (!outList) return;
	if (outList == NULL) return;

	for (int i=0; i<m_shpAttrList.size(); i++)
		outList->push_back(m_shpAttrList.at(i));
	
}
void ColorXmlParser::SetAttributeList(ShapeAttribute *outList)
{
	if (!outList) return;
	if (outList == NULL) return;

	for (int i=0; i<m_shpAttrList.size(); i++)
	{
		outList[i].color = m_shpAttrList.at(i).color;
		outList[i].id    = m_shpAttrList.at(i).id;
		outList[i].width = m_shpAttrList.at(i).width;
	}
}

bool ColorXmlParser::characters(const QString &str)
{
	return true;
}

bool ColorXmlParser::endElement(const QString &, const QString &,
								 const QString &qName)
{
	return true;
}

bool ColorXmlParser::fatalError(const QXmlParseException &exception)
{
	return false;
}