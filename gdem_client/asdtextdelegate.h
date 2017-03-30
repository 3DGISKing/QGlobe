#ifndef ASDTEXTDELEGATE_H
#define ASDTEXTDELEGATE_H

#include <QItemDelegate>

class asdTextDelegate : public QItemDelegate
{
	Q_OBJECT

public:
	asdTextDelegate(QObject *parent = 0);
	~asdTextDelegate();

	QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;

	void setEditorData(QWidget *editor, const QModelIndex &index) const;
	void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;

	void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;

private:
	QString ValueToString(double value) const; //asd 2014.6.4
	double StringToValue(QString &str) const; //asd 2014.6.4
};

#endif // ASDTEXTDELEGATE_H
