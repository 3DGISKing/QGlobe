#ifndef SHAPEDRAW_H
#define SHAPEDRAW_H

#include <QtGui/QMainWindow>
#include "ui_shapedraw.h"
#include "renderarea.h"
#include <QDomDocument>
#include <QDomNode>
#include <QDomText>
#include <QtGui>

class ShapeDraw : public QMainWindow
{
	Q_OBJECT

public:
	ShapeDraw(QWidget *parent = 0, Qt::WFlags flags = 0);
	~ShapeDraw();

private:
	Ui::ShapeDrawClass ui;
	QString m_lastVisitedFolder;

private slots:
	void onAddNewTheme();
	void onConvertToKML();
	void onExit();
	void onClearThemes();
private:
	void InitWindow();
	void MakeStyleMap(int style, QDomDocument* xmlFile, QDomElement * root);
	void MakeStyleNodes(int style, QDomDocument* xmlFile, QDomElement * root);
	void WritePointFolder(QDomDocument *xmlFile, QDomElement*root);
	void WritePolygonFolder(QDomDocument *xmlFile, QDomElement*root);
	void WritePolylineFolder(QDomDocument *xmlFile, QDomElement*root);
	void AddEmptyPairsForLine(QDomDocument *xmlFile, QDomElement*root, int nCount);

private:
	RenderArea *m_Renderer;

protected:
	virtual void paintEvent(QPaintEvent*event);
};

#endif // SHAPEDRAW_H
