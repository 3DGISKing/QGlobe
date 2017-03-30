//added by RSH 2013.3.14
///*
#include "shapecolorpick.h"
#include <QColor>
#include <QColorDialog>
#include <QStringList>
#include <QMessageBox>
#include <QDomDocument>
#include <QDomNode>
#include "msgdata.h"
#include "gdemclient.h"
#include "guiuty.h"
#include "../gdem_libmain/GDM_DataMgr.h"
//#include "../gds_common/GDSConfigMgr.h"

shapeColorPick::shapeColorPick(QWidget *parent)
	: QDialog(parent)
{
	ui_shpColorPicker.setupUi(this);
	connect(ui_shpColorPicker.shpColorTypeCmb, SIGNAL(currentIndexChanged(int)), this, SLOT(SelectCurrentShape(int)));
	connect(ui_shpColorPicker.shpColorDialogBtn, SIGNAL(clicked()), this, SLOT(OpenColorDialog()));
	connect(ui_shpColorPicker.shpColorSelectBtn, SIGNAL(clicked()), this, SLOT(ConfirmShapeAttribute()));

	Init();
}

void shapeColorPick::Init()
{
	m_fileAttrList = new QVector<ShapeAttribute>;
	
	m_fileName = gdm_GetWorkPath() + "/ShapeAttribute.xml";

	QFile file(m_fileName);
	QXmlSimpleReader reader;

	ColorXmlParser handler;
	reader.setContentHandler(&handler);
	reader.setErrorHandler(&handler);
	reader.parse(&file);
	handler.GetAttributeList(m_fileAttrList);

	for (int i=0; i<m_fileAttrList->size(); i++)
	{
		oldColorList._attr[i].color = m_fileAttrList->at(i).color;
		oldColorList._attr[i].width = m_fileAttrList->at(i).width;
		oldColorList._attr[i].id    = m_fileAttrList->at(i).id;
	}

	QStringList items;
	items << tr("Country Border")        << tr("1st Level Admin Borders")           <<tr("2nd Level Admin Borders") 
		  << tr("3nd Level Admin Borders")			  << tr( "HighWay")               << tr( "1st Road") 
		  << tr( "2st Road")               <<  tr( "3st Road")             <<  tr( "4st Road")
		  <<  tr( "5st Road")               << tr( "Wide Railway")          << tr( "Narrow Railway")
		  << tr( "Wide River")			  << tr("River")                 << tr("Lake, Reservoir")
		  << tr("Farmland")              << tr("NonFarmland")           << tr("Forest Land")   ;
	
	m_curInx = 0;
	m_first = true;
	ui_shpColorPicker.shpColorTypeCmb->addItems(items);
	ui_shpColorPicker.shpColorTypeCmb->setCurrentIndex(m_curInx);
}
void shapeColorPick::SelectCurrentShape(int index)
{
	UpdateData(index);
}

void shapeColorPick::UpdateData(int index)
{		
	if (!m_first)
	{
		QString val = ui_shpColorPicker.shpColorWidthTxt->toPlainText();
		bool ok;
		float width = val.toFloat(&ok);
		if (!ok)
		{
			ui_shpColorPicker.shpColorWidthTxt->clear();
			ui_shpColorPicker.shpColorWidthTxt->setFocus();
		}
		oldColorList._attr[m_curInx].width = width;
	}
	m_first = false;

	m_curInx = index;
	if ((index + 1) > GEO_SHAPE_COUNT) m_curInx = GEO_SHAPE_COUNT - 1;
	if (index < 0) m_curInx = 0;

	ui_shpColorPicker.shpColorWidthTxt->setText(QString().setNum(oldColorList._attr[m_curInx].width));
	
	QColor col;
	col.setRgb(QRgb(oldColorList._attr[m_curInx].color));
	QString sheet = QString("background-color: %1").arg(col.name());
	ui_shpColorPicker.shpColorDialogBtn->setStyleSheet(sheet);
	ui_shpColorPicker.shpColorWidthTxt->setText(QString().setNum(oldColorList._attr[m_curInx].width));

}
void shapeColorPick::OpenColorDialog()
{
	QColor col;
	col.setRgb(QRgb(oldColorList._attr[m_curInx].color));
	col = QColorDialog::getColor(col);
	QString sheet = QString("background-color: %1").arg(col.name());
	ui_shpColorPicker.shpColorDialogBtn->setStyleSheet(sheet);
	oldColorList._attr[m_curInx].color = (unsigned int)(col.rgb());
}
void shapeColorPick::ConfirmShapeAttribute()
{
	QString val = ui_shpColorPicker.shpColorWidthTxt->toPlainText();
	bool ok;
	float width = val.toFloat(&ok);
	if (!ok)
	{
		ui_shpColorPicker.shpColorWidthTxt->clear();
		ui_shpColorPicker.shpColorWidthTxt->setFocus();
	}
	oldColorList._attr[m_curInx].width = width;

	QFile file(m_fileName);
	if (file.isOpen()) file.close();
	if (!file.open(QIODevice::WriteOnly))
	{
		gu_MessageBox(this, tr("Save File"), tr("File save error."));
		return;
	}
	QTextStream out(&file);
	const int Indent = 4;
	QDomDocument XMLFile;

	QDomNode xmlNode = XMLFile.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\"");
	XMLFile.insertBefore(xmlNode, XMLFile.firstChild());
	QDomElement root = XMLFile.createElement("ShapeLayerGroup");

	for (unsigned int i=0; i<GEO_SHAPE_COUNT; i++)
	{
		QDomElement layerNode = XMLFile.createElement("Layer");
		root.appendChild(layerNode);
		QString widthStr  = QString().setNum(oldColorList._attr[i].width);
		QString colStr = QString().setNum(oldColorList._attr[i].color);
		QString idStr = QString().setNum(1000 + i);
		root.appendChild(layerNode);
		layerNode.setAttribute("id", idStr);
		layerNode.setAttribute("width", widthStr);
		layerNode.setAttribute("color", colStr);
		
		g_pGDMDataMgr->m_sOption.m_geoShapeAttr[i].color = oldColorList._attr[i].color;
		g_pGDMDataMgr->m_sOption.m_geoShapeAttr[i].id    = oldColorList._attr[i].id;
		g_pGDMDataMgr->m_sOption.m_geoShapeAttr[i].width = oldColorList._attr[i].width;
	}
	XMLFile.appendChild(root);

	XMLFile.save(out, Indent);
	file.close();
	QDialog::reject();

}
shapeColorPick::~shapeColorPick()
{
	delete m_fileAttrList;
}
//*/
//end addition by RSH