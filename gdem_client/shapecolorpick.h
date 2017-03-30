#ifndef SHAPECOLORPICK_H
#define SHAPECOLORPICK_H
//added by RSH 2013.3.14
///*
#include <QDialog>
#include "ui_shapecolorpick.h"
#include <QFile>
#include "colorXmlParser.h"

struct ShapeWidthColor
{
	ShapeAttribute _attr[18];
};

class shapeColorPick : public QDialog
{
	Q_OBJECT

public:
	shapeColorPick(QWidget *parent = 0);
	~shapeColorPick();

	ShapeWidthColor oldColorList;

private:
	Ui::shapeColorPickClass ui_shpColorPicker;

	void UpdateData(int index);

private slots:
		void SelectCurrentShape(int index);
		void OpenColorDialog();
		void ConfirmShapeAttribute();

private:
	void Init();
	QVector<ShapeAttribute> * m_fileAttrList;
	int m_curInx;
	QString m_fileName;
	bool m_first;
};
//end addition by RSH
//*/
#endif // SHAPECOLORPICK_H
