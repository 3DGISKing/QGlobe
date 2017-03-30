#include "treeitemmodel.h"
#include "treeitem.h"
#include "gdemclientbase.h"

TreeItemModel::TreeItemModel()
{
}

QStringList TreeItemModel::mimeTypes() const
{
   return QStandardItemModel::mimeTypes() <<  QLatin1String("text/uri-list");
}
