#ifndef TREEITEMMODEL_H
#define TREEITEMMODEL_H

#include <QStandardItemModel>

class TreeItemModel : public QStandardItemModel
{
	 Q_OBJECT

public:
    TreeItemModel();

	QStringList mimeTypes() const;
};

#endif // TREEITEMMODEL_H
