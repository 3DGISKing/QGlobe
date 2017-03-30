#include "shapedraw.h"
#include <QDirModel>
#include <QGridLayout>
#include "ImportManager.h"
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>

#define STYLE_ID				"msn_ylw-pushpin"
#define STYLE_URL_NORMAL		"#sn_ylw-pushpin"
#define STYLE_URL_HIGHLIGHT		"#sh_ylw-pushpin"
#define STYLE_URL_NORMAL_ID		"sn_ylw-pushpin"
#define STYLE_URL_HIGHLIGHT_ID	"sh_ylw-pushpin"
#define ALTITUDE_MODE_CLAMP		"clampToGround"

#define POINT_THEME				"1"
#define POLYLINE_THEME			"2"
#define POLYGON_THEME			"3"

ShapeDraw::ShapeDraw(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);

	connect(ui.actionAdd, SIGNAL(triggered()), this, SLOT(onAddNewTheme()));
	connect(ui.actionConvert, SIGNAL(triggered()), this, SLOT(onConvertToKML()));
	connect(ui.actionClear, SIGNAL(triggered()), this, SLOT(onClearThemes()));
	connect(ui.actionExit, SIGNAL(triggered()), this, SLOT(onExit()));

	InitWindow();

}
void ShapeDraw::InitWindow()
{
	m_Renderer = new RenderArea();

	QGridLayout *leftLayout = new QGridLayout;
	leftLayout->setColumnMinimumWidth(1, 600);
	leftLayout->setColumnStretch(0, 1);
	leftLayout->setColumnStretch(1, 1);

	leftLayout->addWidget(ui.treeTOC, 0, 0);
	leftLayout->addWidget(m_Renderer, 0, 1);
	QWidget *centralWidget = new QWidget();
	centralWidget->setLayout(leftLayout);

	this->setCentralWidget(centralWidget);
}
void ShapeDraw::paintEvent(QPaintEvent*event)
{
	this->m_Renderer->UpdateDisplay();
}
void ShapeDraw::onConvertToKML()
{
	QMessageBox msgBox;
	QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),	"/home",   tr("KML file(*.kml)"));
	//QString fileName = "d:/1.kml";
	QString title;
	title = fileName.mid(fileName.lastIndexOf("/")+1);
	title = title.left(title.length()-4);

	if (fileName.isEmpty())	return;
	QFile data(fileName);
	if (!data.open(QFile::WriteOnly | QFile::Truncate)) 
	{
		msgBox.setText("Cannot write file! File may be unwritable.");
		msgBox.exec();
		return;
	}	

	QTextStream out(&data);
	
	const int Indent = 4;

	QDomDocument XMLFile;

	//Header Text
	QDomNode xmlNode = XMLFile.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\"");
	XMLFile.insertBefore(xmlNode, XMLFile.firstChild());
	//Upper Node
	QDomElement root = XMLFile.createElement("kml");
	QDomElement subroot = XMLFile.createElement("Document");

	QDomElement groupname = XMLFile.createElement("name");
	subroot.appendChild(groupname);
	QDomText grNameTxt = XMLFile.createTextNode(title);
	groupname.appendChild(grNameTxt);

	for (int i=1; i<4; i++)
	{
	   MakeStyleMap(i, &XMLFile, &subroot);
	   MakeStyleNodes(i, &XMLFile, &subroot);

	}

	XMLFile.appendChild(root);
	root.appendChild(subroot);
	
	//QDomElement datagroup = XMLFile.createElement("Document");
	//subroot.appendChild(datagroup);


	//QDomElement address = XMLFile.createElement("address");
	//datagroup.appendChild(address);
	//QDomElement description = XMLFile.createElement("description");
	//datagroup.appendChild(description);
	//QDomText dsTxt = XMLFile.createTextNode("");
	//description.appendChild(dsTxt);
	
	WritePointFolder(&XMLFile, &subroot);
	WritePolylineFolder(&XMLFile, &subroot);
	WritePolygonFolder(&XMLFile, &subroot);

	XMLFile.save(out, Indent);

	msgBox.setText("Conversion Completed!");
	msgBox.exec();
}

void ShapeDraw::AddEmptyPairsForLine(QDomDocument *xmlFile, QDomElement*root, int nCount)
{
	QDomElement style = xmlFile->createElement("StyleMap");
	root->appendChild(style);
	for (int i=0; i<2*nCount-1; i++)
	{
		QDomElement pairElem = xmlFile->createElement("Pair");
		style.appendChild(pairElem);
		QDomElement keyElem = xmlFile->createElement("key");
		QDomElement urlElem = xmlFile->createElement("styleUrl");
		QDomText keyTxt = xmlFile->createTextNode("0");
		pairElem.appendChild(keyElem);
		pairElem.appendChild(urlElem);
		keyElem.appendChild(keyTxt);
	}
}
void ShapeDraw::WritePolylineFolder(QDomDocument *xmlFile, QDomElement*root)
{
	QMessageBox msgBox;
	QVector<PolygonTheme> *ptThms = m_Renderer->GetPolylineData();
	if (ptThms->size() < 1)	  return;
	for (int i=0; i<ptThms->size(); i++)
	{  	
		PolygonTheme ptTheme = ptThms->at(i);
		DBaseFile *basedata = NULL;
		basedata = ptTheme.m_dBaseData;
		int numField = basedata->_NumberOfFiled;
		QVector<QPolygonF>* ptData = NULL;
		ptData = ptTheme.GetData();

		DBaseFieldDescriptor*field = NULL;
		field = basedata->_Fields;
		int record_count = basedata->_RecordSet.count();

		QDomElement folder = xmlFile->createElement("Folder");
		root->appendChild(folder);

		QDomElement fldname = xmlFile->createElement("name");
		folder.appendChild(fldname);											 
		QDomText grNameTxt = xmlFile->createTextNode(ptTheme.m_themeName);
		fldname.appendChild(grNameTxt);

		QDomElement address = xmlFile->createElement("address");
		folder.appendChild(address);

		QDomElement description = xmlFile->createElement("description");
		folder.appendChild(description);
		QDomText dsTxt = xmlFile->createTextNode("");
		description.appendChild(dsTxt);

		for (int k=0; k<record_count; k++)
		{
			if (record_count != ptData->size()) 
			{
				msgBox.setText("Shape file corrupted!  " + ptTheme.m_themeName);
				msgBox.exec();
				return;
			}
			QPolygonF ptCoord = ptData->at(k);

			Record *rec = basedata->_RecordSet.at(k);

			QString nameStr, descriptionStr="\n";
			if (numField < 1) nameStr = "No Name";
			else 
			{
				nameStr = 	rec->GetFieldValue(0);
				for (int j = 1; j<numField; j++)
				{
					QString fieldName;
					const char* fldName =   NULL;
					fldName = (const char*) basedata->_Fields[j]._Name;
					fieldName = fieldName.fromLocal8Bit(fldName);
					descriptionStr += fieldName + ":" +	rec->GetFieldValue(j) + "\n";
				}	
			}
			double polyWidth = ptCoord.boundingRect().width();
			double look_alti = polyWidth*140000.0;
			QString lookaltStr = QString().setNum(look_alti, 'g', 12);

			double x, y;
			QString xStr, yStr;
			x =	ptCoord.boundingRect().left() + polyWidth/2.0;
			y = ptCoord.boundingRect().bottom() - ptCoord.boundingRect().height()/2.0;
			xStr = QString().setNum(x, 'g', 12);
			yStr = QString().setNum(-y,'g', 12);

			QDomElement placemark = xmlFile->createElement("Placemark");
			folder.appendChild(placemark);

			QDomElement ptname = xmlFile->createElement("name");
			QDomElement ptadr = xmlFile->createElement("address");
			QDomElement ptdscr = xmlFile->createElement("description");

			placemark.appendChild(ptname);
			placemark.appendChild(ptadr);
			placemark.appendChild(ptdscr);

			QDomText ptnametxt = xmlFile->createTextNode(nameStr);
			QDomText ptdesctxt = xmlFile->createTextNode(descriptionStr);
			ptname.appendChild(ptnametxt);
			ptdscr.appendChild(ptdesctxt);

			QDomElement ptlookat = xmlFile->createElement("LookAt");
			placemark.appendChild(ptlookat);
			QDomElement looklongi = xmlFile->createElement("longitude");
			QDomElement looklati = xmlFile->createElement("latitude");
			QDomElement lookalt = xmlFile->createElement("altitude");
			QDomElement lookhead = xmlFile->createElement("heading");
			QDomElement looktilt = xmlFile->createElement("tilt");
			QDomElement lookrange = xmlFile->createElement("range");
			QDomElement lookstatus = xmlFile->createElement("altitudeMode");
			ptlookat.appendChild(looklongi);
			ptlookat.appendChild(looklati);
			ptlookat.appendChild(lookalt);
			ptlookat.appendChild(lookhead);
			ptlookat.appendChild(looktilt);
			ptlookat.appendChild(lookrange);
			ptlookat.appendChild(lookstatus);
			
			QDomText longTxt = xmlFile->createTextNode(xStr);
			QDomText latiTxt = xmlFile->createTextNode(yStr);
			QDomText rangeTxt = xmlFile->createTextNode(lookaltStr);
			looklongi.appendChild(longTxt);
			looklati.appendChild(latiTxt);
			lookrange.appendChild(rangeTxt);

			QDomText altMode = xmlFile->createTextNode(ALTITUDE_MODE_CLAMP);
			lookstatus.appendChild(altMode);

			QDomElement stylemap = xmlFile->createElement("styleUrl");
			placemark.appendChild(stylemap);
			QDomText styleText = xmlFile->createTextNode("#" + QString(STYLE_ID) + QString(POLYLINE_THEME));
			stylemap.appendChild(styleText);

			QDomElement poly = xmlFile->createElement("LineString");
			QDomElement tessel = xmlFile->createElement("tessellate");
			QDomElement coords = xmlFile->createElement("coordinates");

			int nPtCount = ptCoord.size();
			placemark.appendChild(poly);
			poly.appendChild(tessel);

			AddEmptyPairsForLine(xmlFile, &placemark, nPtCount);
			
			poly.appendChild(coords);
			QDomText tesselTxt = xmlFile->createTextNode("1");
			tessel.appendChild(tesselTxt);


			for (int j=0; j<nPtCount; j++)
			{   
				if (j == ptCoord.size()) j = 0;
				x =	 ptCoord.at(j).x(); y = ptCoord.at(j).y();
				xStr = QString().setNum(x, 'g', 12);
				yStr = QString().setNum(-y,'g', 12);
				QDomText coordsTex = xmlFile->createTextNode(xStr + ", " + yStr+",  0 \n");
				coords.appendChild(coordsTex);
			}
		}
	}
}

void ShapeDraw::WritePolygonFolder(QDomDocument *xmlFile, QDomElement*root)
{

	QVector<PolygonTheme> *ptThms = m_Renderer->GetPolygonData();
	if (ptThms->size() < 1) return;
	for (int i=0; i<ptThms->size(); i++)
	{  	
		PolygonTheme ptTheme = ptThms->at(i);
		DBaseFile *basedata = NULL;
		basedata = ptTheme.m_dBaseData;
		int numField = basedata->_NumberOfFiled;
		QVector<QPolygonF>* ptData = NULL;
		ptData = ptTheme.GetData();

		DBaseFieldDescriptor*field = NULL;
		field = basedata->_Fields;
		int record_count = basedata->_RecordSet.count();

		QDomElement folder = xmlFile->createElement("Folder");
		root->appendChild(folder);

		QDomElement fldname = xmlFile->createElement("name");
		folder.appendChild(fldname);											 
		QDomText grNameTxt = xmlFile->createTextNode(ptTheme.m_themeName);
		fldname.appendChild(grNameTxt);

		QDomElement address = xmlFile->createElement("address");
		folder.appendChild(address);

		QDomElement description = xmlFile->createElement("description");
		folder.appendChild(description);
		QDomText dsTxt = xmlFile->createTextNode("");
		description.appendChild(dsTxt);

		for (int k=0; k<record_count; k++)
		{
			if (record_count != ptData->size())
			{
				QMessageBox msgBox;
				msgBox.setText("Theme has different number of records with features! " + ptTheme.m_themeName);
				msgBox.exec();
				return;
			}
			QPolygonF ptCoord = ptData->at(k);

			Record *rec = basedata->_RecordSet.at(k);

			QString nameStr, descriptionStr="";
			if (numField < 1) nameStr = "No Name";
			else 
			{
				nameStr = 	rec->GetFieldValue(0);
				for (int j = 1; j<numField; j++)
				{
					QString fieldName;
					const char* fldName =   NULL;
					fldName = (const char*) basedata->_Fields[j]._Name;
					fieldName = fieldName.fromLocal8Bit(fldName);
					descriptionStr += fieldName + ":" +	rec->GetFieldValue(j) + "\n";
				}	
			}
			double polyWidth = ptCoord.boundingRect().width();
			double look_alti = polyWidth*140000.0;
			QString lookaltStr = QString().setNum(look_alti, 'g', 12);
			
			double x, y;
			QString xStr, yStr;
			x =	ptCoord.boundingRect().left() + polyWidth/2.0;
			y = ptCoord.boundingRect().bottom() - ptCoord.boundingRect().height()/2.0;
			xStr = QString().setNum(x, 'g', 12);
			yStr = QString().setNum(-y,'g', 12);

			QDomElement placemark = xmlFile->createElement("Placemark");
			folder.appendChild(placemark);

			QDomElement ptname = xmlFile->createElement("name");
			QDomElement ptadr = xmlFile->createElement("address");
			QDomElement ptdscr = xmlFile->createElement("description");

			placemark.appendChild(ptname);
			placemark.appendChild(ptadr);
			placemark.appendChild(ptdscr);

			QDomText ptnametxt = xmlFile->createTextNode(nameStr);
			QDomText ptdesctxt = xmlFile->createTextNode(descriptionStr);
			ptname.appendChild(ptnametxt);
			ptdscr.appendChild(ptdesctxt);

			QDomElement ptlookat = xmlFile->createElement("LookAt");
			placemark.appendChild(ptlookat);
			QDomElement looklongi = xmlFile->createElement("longitude");
			QDomElement looklati = xmlFile->createElement("latitude");
			QDomElement lookalt = xmlFile->createElement("altitude");
			QDomElement lookhead = xmlFile->createElement("heading");
			QDomElement looktilt = xmlFile->createElement("tilt");
			QDomElement lookrange = xmlFile->createElement("range");
			QDomElement lookstatus = xmlFile->createElement("altitudeMode");
			ptlookat.appendChild(looklongi);
			ptlookat.appendChild(looklati);
			ptlookat.appendChild(lookalt);
			ptlookat.appendChild(lookhead);
			ptlookat.appendChild(looktilt);
			ptlookat.appendChild(lookrange);
			ptlookat.appendChild(lookstatus);
			
			QDomText longTxt = xmlFile->createTextNode(xStr);
			QDomText latiTxt = xmlFile->createTextNode(yStr);
			QDomText rangeTxt = xmlFile->createTextNode(lookaltStr);
			looklongi.appendChild(longTxt);
			looklati.appendChild(latiTxt);
			lookrange.appendChild(rangeTxt);

			QDomText altMode = xmlFile->createTextNode(ALTITUDE_MODE_CLAMP);
			lookstatus.appendChild(altMode);

			QDomElement stylemap = xmlFile->createElement("styleUrl");
			placemark.appendChild(stylemap);
			QDomText styleText = xmlFile->createTextNode("#" + QString(STYLE_ID) + QString(POLYGON_THEME));
			stylemap.appendChild(styleText);

			QDomElement poly = xmlFile->createElement("Polygon");
			QDomElement tessel = xmlFile->createElement("tessellate");
			QDomElement outer = xmlFile->createElement("outerBoundaryIs");
			QDomElement linearRing = xmlFile->createElement("LinearRing");
			QDomElement coords = xmlFile->createElement("coordinates");

			placemark.appendChild(poly);
			poly.appendChild(tessel);
			poly.appendChild(outer);
			outer.appendChild(linearRing);
			linearRing.appendChild(coords);
			QDomText tesselTxt = xmlFile->createTextNode("1");
			tessel.appendChild(tesselTxt);
			
			int nPtCount = ptCoord.size();
			for (int j=0; j<(nPtCount + 1); j++)
			{   
				int inx = j;
				if (j == ptCoord.size()) inx = 0;
				x =	 ptCoord.at(inx).x(); y = ptCoord.at(inx).y();
				xStr = QString().setNum(x, 'g', 12);
				yStr = QString().setNum(-y,'g', 12);
				QDomText coordsTex = xmlFile->createTextNode(xStr + ", " + yStr+",  0 \n");
				coords.appendChild(coordsTex);
			}
		}
	}
}
void ShapeDraw::WritePointFolder(QDomDocument *xmlFile, QDomElement*root)
{
	QVector<PointTheme> *ptThms = m_Renderer->GetPointData();
	if (ptThms->size() < 1) return;
	for (int i=0; i<ptThms->size(); i++)
	{   
		PointTheme ptTheme = ptThms->at(i);
		DBaseFile *basedata = NULL;
		basedata = ptTheme.m_dBaseData;
		int numField = basedata->_NumberOfFiled;
		QVector<QPointF>* ptData = NULL;
		ptData = ptTheme.GetData();

		DBaseFieldDescriptor*field = NULL;
		field = basedata->_Fields;
		int record_count = basedata->_RecordSet.count();
		
		
		QDomElement folder = xmlFile->createElement("Folder");
		root->appendChild(folder);

		QDomElement fldname = xmlFile->createElement("name");
		folder.appendChild(fldname);											 
		QDomText grNameTxt = xmlFile->createTextNode(ptTheme.m_themeName);
		fldname.appendChild(grNameTxt);

		QDomElement address = xmlFile->createElement("address");
		folder.appendChild(address);

		QDomElement description = xmlFile->createElement("description");
		folder.appendChild(description);
		QDomText dsTxt = xmlFile->createTextNode("");
		description.appendChild(dsTxt);

		for (int k=0; k<record_count; k++)
		{
			if (record_count != ptData->size())
			{
				QMessageBox msgBox;
				msgBox.setText("Theme has different number of records with features! " + ptTheme.m_themeName);
				msgBox.exec();
				return;
			}	

			double _x, _y;
			QPointF ptCoord = ptData->at(k);

			_x = ptCoord.x();
			_y = -ptCoord.y();

			QString xStr, yStr;
			xStr = QString().setNum(_x, 'g', 12);
			yStr = QString().setNum(_y, 'g', 12);

			Record *rec = basedata->_RecordSet.at(k);

			QString nameStr, descriptionStr="";
			if (numField < 1) nameStr = "No Name";
			else 
			{
				nameStr = 	rec->GetFieldValue(0);
				for (int j = 1; j<numField; j++)
				{
					QString fieldName;
					const char* fldName =   NULL;
					fldName = (const char*) basedata->_Fields[j]._Name;
					fieldName = fieldName.fromLocal8Bit(fldName);
					descriptionStr += fieldName + ":" +	rec->GetFieldValue(j) + "\n";
				}	
			}

			QDomElement placemark = xmlFile->createElement("Placemark");
			folder.appendChild(placemark);

			QDomElement ptname = xmlFile->createElement("name");
			QDomElement ptadr = xmlFile->createElement("address");
			QDomElement ptdscr = xmlFile->createElement("description");

			placemark.appendChild(ptname);
			placemark.appendChild(ptadr);
			placemark.appendChild(ptdscr);

			QDomText ptnametxt = xmlFile->createTextNode(nameStr);
			QDomText ptdesctxt = xmlFile->createTextNode(descriptionStr);
			ptname.appendChild(ptnametxt);
			ptdscr.appendChild(ptdesctxt);

			QDomElement ptlookat = xmlFile->createElement("LookAt");
			placemark.appendChild(ptlookat);
			QDomElement looklongi = xmlFile->createElement("longitude");
			QDomElement looklati = xmlFile->createElement("latitude");
			QDomElement lookalt = xmlFile->createElement("altitude");
			QDomElement lookhead = xmlFile->createElement("heading");
			QDomElement looktilt = xmlFile->createElement("tilt");
			QDomElement lookrange = xmlFile->createElement("range");
			QDomElement lookstatus = xmlFile->createElement("altitudeMode");
			
			
			
			ptlookat.appendChild(looklongi);
			ptlookat.appendChild(looklati);
			ptlookat.appendChild(lookalt);
			ptlookat.appendChild(lookhead);
			ptlookat.appendChild(looktilt);
			ptlookat.appendChild(lookrange);
			ptlookat.appendChild(lookstatus);

			QDomText longTxt = xmlFile->createTextNode(xStr);
			QDomText latiTxt = xmlFile->createTextNode(yStr);
			QDomText rangeTxt = xmlFile->createTextNode("3300.0");
			looklongi.appendChild(longTxt);
			looklati.appendChild(latiTxt);
			lookrange.appendChild(rangeTxt);			
			QDomText altMode = xmlFile->createTextNode(ALTITUDE_MODE_CLAMP);
			lookstatus.appendChild(altMode);
			
			QDomElement stylemap = xmlFile->createElement("styleUrl");
			placemark.appendChild(stylemap);
			QDomText styleText = xmlFile->createTextNode("#" + QString(STYLE_ID) + QString(POINT_THEME));
			stylemap.appendChild(styleText);

			QDomElement point = xmlFile->createElement("Point");
			QDomElement coords = xmlFile->createElement("coordinates");
			placemark.appendChild(point);
			point.appendChild(coords);
			QDomText xTex = xmlFile->createTextNode(xStr+",  ");
			QDomText yTex = xmlFile->createTextNode(yStr+",  0");
			coords.appendChild(xTex);
			coords.appendChild(yTex);

		}

	}
}
void ShapeDraw::MakeStyleMap(int style, QDomDocument* xmlFile, QDomElement * root)
{
	//StyleMap Node : 1-Point theme, 2-Polyline theme, 3-Polygon theme;
	QString strStyle = QString().setNum(style);
	QDomElement stylemap = xmlFile->createElement("StyleMap");
	QDomElement Pair1 = xmlFile->createElement("Pair");
	QDomElement key1 = xmlFile->createElement("key");
	QDomElement key2 = xmlFile->createElement("key");
	QDomElement styleUrl1 = xmlFile->createElement("styleUrl");
	QDomText keyTxt1 = xmlFile->createTextNode("0");
	QDomText keyTxt2 = xmlFile->createTextNode("1");
	QDomElement styleUrl2 = xmlFile->createElement("styleUrl");
	QDomText styleUrlTxt1 = xmlFile->createTextNode(QString(STYLE_URL_NORMAL) + strStyle);
	QDomText styleUrlTxt2 = xmlFile->createTextNode(QString(STYLE_URL_HIGHLIGHT) + strStyle);
	QDomElement Pair2 = xmlFile->createElement("Pair");

	root->appendChild(stylemap);
	stylemap.setAttribute("id", STYLE_ID+strStyle);
	stylemap.appendChild(Pair1);
	Pair1.appendChild(key1);
	Pair1.appendChild(styleUrl1);
	key1.appendChild(keyTxt1);
	styleUrl1.appendChild(styleUrlTxt1);

	stylemap.appendChild(Pair2);
	Pair2.appendChild(key2);
	Pair2.appendChild(styleUrl2);
	key2.appendChild(keyTxt2);
	styleUrl2.appendChild(styleUrlTxt2);


}
void ShapeDraw::MakeStyleNodes(int styleInt, QDomDocument* xmlFile, QDomElement * root)
{
	QString styleStr = QString().setNum(styleInt);
	if (styleInt < 2) //point theme
	{
		for (int i=0; i<2; i++)
		{
			QDomElement style = xmlFile->createElement("Style");
			QDomElement iconStyle = xmlFile->createElement("IconStyle");
			QDomElement scale = xmlFile->createElement("scale");
			QDomElement hotspot = xmlFile->createElement("hotSpot");
			QDomElement labelStyle = xmlFile->createElement("LabelStyle");
			QDomElement color = xmlFile->createElement("color");

			root->appendChild(style);
			style.appendChild(iconStyle);
			iconStyle.appendChild(scale);
			iconStyle.appendChild(hotspot);
			if (i == 0) 
			{
				QDomText sclTxt = xmlFile->createTextNode("1.1");
				scale.appendChild(sclTxt);
				style.setAttribute("id", STYLE_URL_NORMAL_ID + styleStr);
			}
			else
			{
				QDomText sclTxt = xmlFile->createTextNode("1.3");
				scale.appendChild(sclTxt);
				style.setAttribute("id", STYLE_URL_HIGHLIGHT_ID + styleStr);
			}
			hotspot.setAttribute("x", "20");
			hotspot.setAttribute("xunits", "pixels");
			hotspot.setAttribute("y", "2");
			hotspot.setAttribute("yunits", "pixels");
			style.appendChild(labelStyle);
			labelStyle.appendChild(color);
			QDomText csTxt = xmlFile->createTextNode("ff00ff00");
			color.appendChild(csTxt);
		}
	}
	else
	{
		QDomElement style = xmlFile->createElement("Style");
		QDomElement lineStyle1 = xmlFile->createElement("LineStyle");
		QDomElement linecolor1 = xmlFile->createElement("color");
		QDomElement width1 = xmlFile->createElement("width");
		QDomElement polyStyle1 = xmlFile->createElement("PolyStyle");
		QDomElement polycolor1 = xmlFile->createElement("color");
		QDomElement style2 = xmlFile->createElement("Style");
		QDomElement iconStyle = xmlFile->createElement("IconStyle");
		QDomElement scale = xmlFile->createElement("scale");
		QDomElement heading = xmlFile->createElement("heading");
		QDomElement icon = xmlFile->createElement("Icon");
		QDomElement href = xmlFile->createElement("href");
		QDomElement hotSpot = xmlFile->createElement("hotSpot");
		QDomElement lineStyle2 = xmlFile->createElement("LineStyle");
		QDomElement linecolor2 = xmlFile->createElement("color");
		QDomElement width2 = xmlFile->createElement("width");
		QDomElement polyStyle2 = xmlFile->createElement("PolyStyle");
		QDomElement polycolor2 = xmlFile->createElement("color");

		root->appendChild(style);
		style.appendChild(lineStyle1);
		lineStyle1.appendChild(linecolor1);
		lineStyle1.appendChild(width1);
		style.appendChild(polyStyle1);
		polyStyle1.appendChild(polycolor1);
		root->appendChild(style2);
		style2.appendChild(iconStyle);
		iconStyle.appendChild(scale);
		iconStyle.appendChild(heading);
		iconStyle.appendChild(icon);
		icon.appendChild(href);
		style2.appendChild(lineStyle2);
		lineStyle2.appendChild(linecolor2);
		lineStyle2.appendChild(width2);
		style2.appendChild(polyStyle2);
		polyStyle2.appendChild(polycolor2);

		style.setAttribute("id", STYLE_URL_NORMAL_ID + styleStr);
		QString colStyleStr = "ff00ff00";
		if (styleInt == 3) colStyleStr = "ffffff00";
		QDomText lcolorTxt = xmlFile->createTextNode(colStyleStr);
		linecolor1.appendChild(lcolorTxt);
		QDomText lwidthTxt = xmlFile->createTextNode("2");
		width1.appendChild(lwidthTxt);
		QDomText pctxt = xmlFile->createTextNode("77ff3333");
		polycolor1.appendChild(pctxt);
		QDomText sctxt = xmlFile->createTextNode("0");
		scale.appendChild(sctxt);
		style2.setAttribute("id", STYLE_URL_HIGHLIGHT_ID + styleStr);
		QDomText hdtxt = xmlFile->createTextNode("0");
		heading.appendChild(hdtxt);
		QDomText lctxt = xmlFile->createTextNode(colStyleStr);
		linecolor2.appendChild(lctxt);
		QDomText wd = xmlFile->createTextNode("2");
		width2.appendChild(wd);
		QDomText polyColTxt = xmlFile->createTextNode("77ff3333");
		polycolor2.appendChild(polyColTxt);
	}
}
void ShapeDraw::onAddNewTheme()
{	
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),m_lastVisitedFolder,tr("Shape file (*.shp)"));
	if (fileName.isEmpty()) return;
	m_lastVisitedFolder = fileName;

	ShapeFileImporter *importer = new ShapeFileImporter(fileName);
	QVector<QPolygonF> polys;
	ShapeTypeAndCount header = importer->ReadShapeFileHeader();
	DBaseFile *dbasedata=NULL;

	switch (header.ShapeType)
	{
		case POINT_SHAPE:
		{
			QVector<QPointF> pts;
			importer->ReadPointShape(header, &pts, dbasedata);
			m_Renderer->addPointTheme(&pts, dbasedata, fileName);
			break;
		}
		case POLYLINE_SHAPE:
		{
			QVector<QPolygonF> polys;
			importer->ReadPolygonShape(header, &polys, dbasedata);
			m_Renderer->addPolylineTheme(&polys, dbasedata, fileName);	
			break;
		}
		case POLYGON_SHAPE:
		{
			QVector<QPolygonF> polys;
			importer->ReadPolygonShape(header, &polys, dbasedata);
			m_Renderer->addPolygonTheme(&polys, dbasedata, fileName);
			break;
		}
		default:
			break;
	}
	delete importer;
	
	fileName = fileName.mid(fileName.lastIndexOf("/")+1);

	ui.treeTOC->addItem(fileName);

}
void ShapeDraw::onExit()
{
	QMainWindow::close();
}
void ShapeDraw::onClearThemes()
{	
	m_Renderer->clearAll();
	ui.treeTOC->clear();
	if (m_Renderer) delete m_Renderer;
	m_Renderer = new RenderArea();
	
}
ShapeDraw::~ShapeDraw()
{
	if (m_Renderer)
		delete m_Renderer;
	m_Renderer = NULL;
}
