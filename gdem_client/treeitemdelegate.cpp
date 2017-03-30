#include <QtGui>
#include "treeitemdelegate.h"
#include "treeitem.h"
#include "treeitemedit.h"
#include "guiuty.h"
#include "gdemclient.h"

#define NAME_COLOR			Qt::black
#define DESCRIPTION_COLOR	Qt::darkGray //QColor(121,121,121)
#define HIGHLIGHT_COLOR		QColor(153,204,255)
#define ITEM_NAME_SIZE		9
#define ITEM_DESC_SIZE		8
#define LEFT_MARGIN			8
#define RIGHT_MARGIN		3
#define TOP_MARGIN			3
#define BOTTOM_MARGIN		7
#define LINE_SPACE			3
#define LEFT_POSITION_TEXT	43

int FindLine(const QString text)
{
	QString str = text;
	QChar *data = str.data();
	int pos = 0;
	 while (!data->isNull())
	{
		 quint16 code = data->unicode();
		 if (code == 8232)
			 break;
		 ++data;
		 ++pos;
	 }
	 return pos;
}


/*
 * public
 */
TreeItemDelegate::TreeItemDelegate(QObject *parent)
	: QItemDelegate(parent)
{
}

QSize TreeItemDelegate::sizeHint(const QStyleOptionViewItem &option,
				   const QModelIndex &index) const
{

	QSize itemSize =  QItemDelegate::sizeHint(option, index);
	PLACEMARK_DATA data = qVariantValue<PLACEMARK_DATA>(index.data(TI_DATA_ROLE));
	QString des = QString::fromUtf16(data.description);
	return AddSize(itemSize, des);
}

QWidget *TreeItemDelegate::createEditor(QWidget *parent,
									 const QStyleOptionViewItem & /* option */,
									 const QModelIndex &index) const
{
	//PLACEMARK_DATA data = qVariantValue<PLACEMARK_DATA>(index.data(DATA_ROLE));
	TreeItemEdit *edit = new TreeItemEdit(parent);
	edit->setReadOnly(true);

	QString text = qVariantValue<QString>(index.data(TI_PAGE_ROLE));
	edit->setHtml(text);

#ifdef DEBUG
	qDebug("createEditor\n");
#endif
	return edit;
}

void TreeItemDelegate::setEditorData(QWidget *editor,
								  const QModelIndex &index) const
{
	QLineEdit *edit = qobject_cast<QLineEdit *>(editor);
	if (!edit)
		return;

	PLACEMARK_DATA data = qVariantValue<PLACEMARK_DATA>(index.data(TI_DATA_ROLE));
	QString str = QString::fromUtf16(data.name);
	edit->setText(str);
#ifdef DEBUG
	qDebug("setEditorData\n");
#endif

}

void TreeItemDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
								 const QModelIndex &index) const
{
	PLACEMARK_DATA data = qVariantValue<PLACEMARK_DATA>(index.data(TI_DATA_ROLE));
	QLineEdit *edit = qobject_cast<QLineEdit *>(editor);
	if (!edit)
		return;
	/*
	int length = edit->text().toWCharArray(data.name);
	quint16 code = 0;
	data.name[length] = code;*/

	QString str = edit->text();
	gu_qstring2ushort(str, data.name, PMD_STR_MAX_SIZE);

	TreeItem * item = static_cast<TreeItem*>(((QStandardItemModel *)model)->itemFromIndex(index));

	item->SetItemData(data);
	item->setEditable(false);
#ifdef DEBUG
	qDebug("setModelData\n");
#endif
}

void TreeItemDelegate::paint( QPainter * painter, const QStyleOptionViewItem & option,
				 const QModelIndex & index ) const
{
	QStyleOptionViewItem newoption = option;
	QColor backgroundColor = option.palette.color(QPalette::Window).darker(110);
	if ((option.state & QStyle::State_HasFocus))
		backgroundColor = HIGHLIGHT_COLOR;

	newoption.palette.setColor(QPalette::Highlight, backgroundColor);

	QItemDelegate::paint(painter, newoption, index);
}


/*
 * protected
 */

void TreeItemDelegate::drawDisplay ( QPainter * painter, const QStyleOptionViewItem & option,
							   const QRect & rect, const QString & text ) const
{
	QString name("");
	QString desc("");

	//int n = text.indexOf(QChar(0x0a));
	int n = FindLine(text);

	if (n == -1)
	{
		name = text;
	}
	else
	{
		name = text.left(n);
		desc = text.right(text.count() - n -1);
	}

	drawText(painter,option,rect,name,desc);
}

void TreeItemDelegate::drawFocus ( QPainter * painter, const QStyleOptionViewItem & option,
								   const QRect & rect ) const
{
	return;
}

/*
 * private
 */
void TreeItemDelegate::drawText(QPainter *painter, const QStyleOptionViewItem & option,
								const QRect &rect, QString &name, QString &desc) const
{
	QRect nameRect = rect;
	QRect desRect = rect;

	painter->save();
	QFont font = painter->font();
	font.setPointSize(g_app->GetDrawFontSize(ITEM_NAME_SIZE));
	QFontMetrics fm(font);
	int pixelsNameHigh = fm.height();
	painter->setFont(font);
	painter->setPen(NAME_COLOR);
	nameRect.setLeft(nameRect.left() + LEFT_MARGIN);
	nameRect.setTop(nameRect.top() + TOP_MARGIN);
	nameRect.setRight(nameRect.right() - RIGHT_MARGIN);
	nameRect.setBottom(nameRect.top() + pixelsNameHigh);

	painter->drawText(nameRect, name);

	if (desc.isEmpty())
	{
		painter->restore();
		return;
	}

	font.setPointSize(g_app->GetDrawFontSize(ITEM_DESC_SIZE));
	painter->setFont(font);
	painter->setPen(DESCRIPTION_COLOR);
	fm = QFontMetrics(font);
	int pixelsDesHigh = fm.height();
	desRect.setTop(nameRect.bottom() + LINE_SPACE);
	desRect.setLeft(nameRect.left());
	desRect.setRight(nameRect.right());
	int btMargin = g_app->GetDrawFontMargin(BOTTOM_MARGIN);
	desRect.setBottom(desRect.top() + 2 * pixelsDesHigh + btMargin);
	painter->drawText(desRect, Qt::TextWordWrap, desc);

	painter->restore();
}

QSize TreeItemDelegate::AddSize(QSize size, QString description) const
{
	QSize itemSize = size;
	QFont font;
	font.setPointSize(g_app->GetDrawFontSize(ITEM_NAME_SIZE));
	QFontMetrics fm(font);
	itemSize.setHeight(fm.height()+ TOP_MARGIN );
	if (description.isEmpty())
		return itemSize;

	font.setPointSize(g_app->GetDrawFontSize(ITEM_DESC_SIZE));
	fm = QFontMetrics(font);
	int btMargin = g_app->GetDrawFontMargin(BOTTOM_MARGIN);
	itemSize.setHeight(itemSize.height() + 1 * fm.height() + LINE_SPACE + btMargin);

	return itemSize;
}
