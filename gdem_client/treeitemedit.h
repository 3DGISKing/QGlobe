#ifndef TREEITEMEDIT_H
#define TREEITEMEDIT_H

#include <QTextEdit>

class TreeItemEdit : public QTextEdit
{
public:
	TreeItemEdit( QWidget * parent = 0);
	QSize sizeHint () const;

protected:
	void mousePressEvent ( QMouseEvent * e );
	void mouseMoveEvent ( QMouseEvent * e );
	void contextMenuEvent ( QContextMenuEvent * event );

private:
	void Init();
	bool GetSearchParam(const QString &url, QString &key, int &index, int &itemkey);

};

#endif // TREEITEMEDIT_H
