#ifndef TREEITEMDELEGATE_H
#define TREEITEMDELEGATE_H

#include <QItemDelegate>

class TreeItemDelegate : public QItemDelegate
{
	Q_OBJECT

public:
	TreeItemDelegate(QObject *parent = 0);
	QSize sizeHint(const QStyleOptionViewItem &option,
				   const QModelIndex &index) const;

	QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
						  const QModelIndex &index) const;
	void setEditorData(QWidget *editor, const QModelIndex &index) const;
	void setModelData(QWidget *editor, QAbstractItemModel *model,
					  const QModelIndex &index) const;
	void paint( QPainter * painter, const QStyleOptionViewItem & option,
				 const QModelIndex & index ) const;

protected:
	void drawDisplay ( QPainter * painter, const QStyleOptionViewItem & option,
							   const QRect & rect, const QString & text ) const;
	void drawFocus ( QPainter * painter, const QStyleOptionViewItem & option,
					 const QRect & rect ) const;

private:
	void drawText(QPainter *painter, const QStyleOptionViewItem & option,
								const QRect &rect, QString &name, QString &desc) const;
	QSize AddSize(QSize size, QString description) const;
};


#endif
