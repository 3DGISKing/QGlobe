#include <qglobal.h>
#include <qdebug.h>
#include <qlabel.h>
#include <qgroupbox.h>
#include <qpushbutton.h>
#include "fontdialog.h"

#ifdef GDEM_CLIENT
#include "gdemclient.h"
#define TransText(t)	g_app->Trans(t)
#else
#define TransText(t)	(t)
#endif

#define FONTDLG_DEBUG		0
#define FONTDLG_DEBUGTEXT	0

#define	MESSAGE0	tr("Select Font")
#define	MESSAGE1	tr("&Font")
#define	MESSAGE2	tr("Font st&yle")
#define	MESSAGE3	tr("&Size")
#define	MESSAGE4	tr("Wr&iting System")
#define	MESSAGE5	tr("Stri&keout")
#define	MESSAGE6	tr("&Underline")
#define	MESSAGE7	tr("Effects")
#define	MESSAGE8	tr("Sample")

FontDialog::FontDialog(QWidget *parent)
		:QFontDialog(parent)
{
}

FontDialog::FontDialog(const QFont &initial, QWidget *parent)
		:QFontDialog(initial, parent)
{
}

QFont FontDialog::GetFont(const QFont &initial, QWidget *parent)
{
	QFont font = initial;

	FontDialog dlg(font);

	dlg.exec();
	if (QDialog::Accepted == dlg.result())
		font = dlg.selectedFont();

	return font;
}

void FontDialog::showEvent(QShowEvent *event)
{
	setWindowTitle(TransText("Select Font"));
	TransObjectText(this, 0);
}

void FontDialog::TransObjectText(QObject *parent, int level)
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
#if FONTDLG_DEBUG
			QString dbstr(8*level, ' ');
			dbstr += QString::number(i) + QString(":") + className;
			qDebug(dbstr.toAscii().data());
#endif
			TransObjectText(obj, level+1);

			if (!obj->inherits("QGroupBox"))
				continue;
		}

		if (obj->inherits("QLabel"))
		{
			QLabel *label = (QLabel *)obj;
			text = label->text();
			transText = TransText(text);
			label->setText(transText);
		}
		else if (obj->inherits("QGroupBox"))
		{
			QGroupBox *group = (QGroupBox *)obj;
			text = group->title();
			transText = TransText(text);
			group->setTitle(transText);
		}
		else if (obj->inherits("QCheckBox"))
		{
			QCheckBox *checkBox = (QCheckBox *)obj;
			text = checkBox->text();
			transText = TransText(text);
			checkBox->setText(transText);
		}
		else if (obj->inherits("QAbstractButton"))
		{
			QAbstractButton *button = (QAbstractButton *)obj;
			text = button->text();
			transText = TransText(text);
			button->setText(transText);
		}
#if FONTDLG_DEBUG
		QString dbstr(8*level, ' ');
		dbstr += QString::number(i) + QString(":") + className + QString(":") + text
				 + QString(":") + transText;
		qDebug(dbstr.toAscii().data());
#endif
#if FONTDLG_DEBUGTEXT
		if (text.length() > 0 && text == transText)
			qDebug(text.toAscii().data());
#endif
	}
}
