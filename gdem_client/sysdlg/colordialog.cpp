#include <qglobal.h>
#include <qdebug.h>
#include <qlabel.h>
#include <qgroupbox.h>
#include <qpushbutton.h>
#include <qdialogbuttonbox.h>
#include <qmetaobject.h>
#include <qlineedit.h>

#include "colordialog.h"

#ifdef GDEM_CLIENT
#include "gdemclient.h"
#define TransText(t)	g_app->Trans(t)
#else
#define TransText(t)	(t)
#endif

#define COLORDLG_DEBUG		0
#define COLORDLG_DEBUGTEXT	0

#define	MESSAGE0	tr("Select Color")
#define	MESSAGE1	tr("&Basic colors")
#define	MESSAGE2	tr("&Custom colors")
#define	MESSAGE3	tr("&Add to Custom Colors")
#define	MESSAGE4	tr("Hu&e:")
#define	MESSAGE5	tr("&Sat:")
#define	MESSAGE6	tr("&Val:")
#define	MESSAGE7	tr("&Red:")
#define	MESSAGE8	tr("&Green:")
#define	MESSAGE9	tr("Bl&ue:")
#define	MESSAGE10	tr("A&lpha channel:")
#define	MESSAGE11	tr("OK")
#define	MESSAGE12	tr("Cancel")

ColorDialog::ColorDialog(QWidget *parent)
		: QColorDialog(parent)
{
}

ColorDialog::ColorDialog(const QColor &initial, QWidget *parent)
		: QColorDialog(initial, parent)
{
}

QColor ColorDialog::GetColor(const QColor &initial, QWidget *parent)
{
	QColor color = initial;

	ColorDialog dlg(color);

	dlg.exec();
	if (QDialog::Accepted == dlg.result())
		color = dlg.selectedColor();

	return color;
}

void ColorDialog::showEvent(QShowEvent *event)
{
	setWindowTitle(TransText("Select Color"));
	TransObjectText(this, 0);
}

void ColorDialog::TransObjectText(QObject *parent, int level)
{
	QObjectList childs = parent->children();
	for (int i = 0; i < childs.count(); ++i)
	{
		QObject *obj = childs.at(i);
		QString text;
		QString transText;
		const QMetaObject *metaObj = obj->metaObject();
		QString className = QString(metaObj->className());

		if (obj->children().count() > 0)
		{
#if COLORDLG_DEBUG
			QString dbstr(8*level, ' ');
			dbstr += QString::number(i) + QString(":") + className;
			qDebug(dbstr.toAscii().data());
#endif
			TransObjectText(obj, level+1);
			continue;
		}

		if (obj->inherits("QLabel"))
		{
			QLabel *label = (QLabel *)obj;
			text = label->text();
			transText = TransText(text);
			label->setText(transText);
		}
		else if (obj->inherits("QAbstractButton"))
		{
			QAbstractButton *button = (QAbstractButton *)obj;
			text = button->text();
			transText = TransText(text);
			button->setText(transText);
		}
#if COLORDLG_DEBUG
		QString dbstr(8*level, ' ');
		dbstr += QString::number(i) + QString(":") + className + QString(":") + text
				 + QString(":") + transText;
		qDebug(dbstr.toAscii().data());
#endif
#if COLORDLG_DEBUGTEXT
		if (text.length() > 0 && text == transText)
			qDebug(text.toAscii().data());
#endif
	}
}
