#ifndef COLORDIALOG_H
#define COLORDIALOG_H

#include <QColorDialog>

class ColorDialog : public QColorDialog
{
public:
	ColorDialog (QWidget *parent = 0);
	ColorDialog (const QColor &initial, QWidget *parent = 0);

	static QColor GetColor(const QColor &initial, QWidget *parent=0);

protected:
	virtual void showEvent(QShowEvent *event);
	void TransObjectText(QObject *parent, int level);
};

#endif // COLORDIALOG_H
