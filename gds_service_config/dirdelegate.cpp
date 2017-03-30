#include "dirdelegate.h"
#include "directorybox.h"
#include <QtGui>

DirDelegate::DirDelegate(QObject *parent)
	: QItemDelegate(parent)
{
}

QWidget *DirDelegate::createEditor(QWidget *parent,
	const QStyleOptionViewItem & option ,
	const QModelIndex & index ) const
{
	if (!index.isValid())
		return 0;
	if (index.column() != 2)
		return QItemDelegate::createEditor(parent,option,index);

	DirectoryBox *editor = new DirectoryBox(parent);
	connect(editor, SIGNAL(setDirectory()),
				this, SLOT(commitEditor()));
	return editor;
}

void DirDelegate::setEditorData(QWidget *editor,
									const QModelIndex &index) const
{
	if (index.column() != 2)
		return QItemDelegate::setEditorData(editor,index);
	QString value = index.model()->data(index, Qt::EditRole).toString();
	DirectoryBox *dirBox = static_cast<DirectoryBox*>(editor);
	dirBox->SetValue(value);
}

void DirDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
								   const QModelIndex &index) const
{
	if (index.column() != 2)
		return QItemDelegate::setModelData(editor, model, index);
	DirectoryBox *dirBox = static_cast<DirectoryBox*>(editor);
	QString value = dirBox->GetValue();
	model->setData(index, value, Qt::EditRole);
}

void DirDelegate::updateEditorGeometry(QWidget *editor,
	const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
{
	editor->setGeometry(option.rect);
}

void DirDelegate::commitEditor()
{
	DirectoryBox *dirBox = qobject_cast<DirectoryBox *>(sender());
	emit commitData(dirBox);
}
