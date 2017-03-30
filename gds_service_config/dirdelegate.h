#ifndef DIRDELEGATE_H
#define DIRDELEGATE_H

#include <QItemDelegate>
#include <QModelIndex>

class DirDelegate : public QItemDelegate
{
	Q_OBJECT

public:
	DirDelegate(QObject *parent = 0);

	QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
						  const QModelIndex &index) const;

	void setEditorData(QWidget *editor, const QModelIndex &index) const;
	void setModelData(QWidget *editor, QAbstractItemModel *model,
					  const QModelIndex &index) const;

	void updateEditorGeometry(QWidget *editor,
		const QStyleOptionViewItem &option, const QModelIndex &index) const;
private slots:
	void commitEditor();
};

#endif // DIRDELEGATE_H
