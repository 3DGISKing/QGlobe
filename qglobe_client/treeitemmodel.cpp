#include "treeitemmodel.h"
#include "treeitem.h"
#include "qglobeclientbase.h"

TreeItemModel::TreeItemModel()
{
}

QStringList TreeItemModel::mimeTypes() const
{
   return QStandardItemModel::mimeTypes() <<  QLatin1String("text/uri-list");
}
