#include "treeitemedit.h"
#include "gdemclient.h"
#include "mainwindow.h"
#include "searchdlg.h"

// http://pic.com/maps?q=china&start=10^itemkey=100

#define SEARCH_VARIABLE		"q="
#define INDEX_VARIABLE		"&start="    
#define ITEMKEY_VARIABLE	"^itemkey="

/*
 *   public
 */

TreeItemEdit::TreeItemEdit( QWidget * parent)
: QTextEdit(parent)
{
	Init();
}

QSize TreeItemEdit::sizeHint () const
{
	QSize size(150,18);
	return size;
}

/*
 *	protected
 */

void TreeItemEdit::mousePressEvent ( QMouseEvent * e )
{
	QPoint pos = e->pos();
	QString url = anchorAt(pos);
	if (url.isEmpty())
		return;

	QString key("");
	int startindex = 0;
	int itemkey = 0;
	if (GetSearchParam(url, key, startindex, itemkey))
		g_app->GetMainWindow()->Search(key, startindex, itemkey);
}

void TreeItemEdit::mouseMoveEvent ( QMouseEvent * e )
{
	QPoint pos = e->pos();
	QString anchor = anchorAt(pos);
	if (anchor.isEmpty())
	{
		viewport()->setProperty("cursor", QVariant(QCursor(Qt::ArrowCursor)));
	}
	else
	{
		viewport()->setProperty("cursor", QVariant(QCursor(Qt::PointingHandCursor)));
	}
}

void TreeItemEdit::contextMenuEvent ( QContextMenuEvent * event )
{
	event->ignore();
}

/*
 *	private
 */

void TreeItemEdit::Init()
{
	// change property
	setMouseTracking(true);
	setFrameShape(QFrame::NoFrame);
	setFrameShadow(QFrame::Plain);
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

bool TreeItemEdit::GetSearchParam(const QString &url, QString &key, int &index, int &itemkey)
{
	int keypos = url.indexOf(SEARCH_VARIABLE);
	int indexpos = url.indexOf(INDEX_VARIABLE);
	int itemkeypos = url.indexOf(ITEMKEY_VARIABLE);

	if (keypos < 0 || indexpos < 0 || itemkeypos < 0)
		return false;

	int pos1 = keypos + QString(SEARCH_VARIABLE).count();
	key = url.mid(pos1 , indexpos - pos1);
	pos1 = indexpos + QString(INDEX_VARIABLE).count();
	QString indextext = url.mid(pos1, itemkeypos - pos1);

	bool ok;
	index = indextext.toInt(&ok);
	if (indextext.isEmpty() || ok == false)
		return false;


	pos1 = itemkeypos + QString(ITEMKEY_VARIABLE).count();
	indextext = url.right(url.count() - pos1);
	itemkey	= indextext.toInt(&ok);
	if ( !indextext.isEmpty() && ok == true)
		return true;
	return false;
}
