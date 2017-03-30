#ifndef FONTDIALOG_H
#define FONTDIALOG_H

#include <QFontDialog>

class FontDialog : public QFontDialog
{
public:
	FontDialog(QWidget *parent = 0);
	FontDialog(const QFont &initial, QWidget *parent = 0);

	static QFont GetFont(const QFont &initial, QWidget *parent=0);

protected:
	virtual void showEvent(QShowEvent *event);
	void TransObjectText(QObject *parent, int level);
};

#endif // FONTDIALOG_H
