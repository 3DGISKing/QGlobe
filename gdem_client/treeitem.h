#ifndef TREEITEM_H
#define TREEITEM_H

#include <QStandardItem>
#include <QMetaType>
#include "gdemclientbase.h"

#define TI_DATA_ROLE			30
#define TI_PAGE_ROLE			(Qt::UserRole + 1)
#define TI_MAX_SEARCH_COUNT		20

Q_DECLARE_METATYPE(PLACEMARK_DATA)

typedef struct _PLACEMARK_DATA_LIST
{
	unsigned int    total_count;
	unsigned int    start_index;
	unsigned int    end_index;
	PLACEMARK_DATA	data_list[TI_MAX_SEARCH_COUNT];
} PLACEMARK_DATA_LIST;

class TreeItem : public QStandardItem
{
public:
	TreeItem(const PLACEMARK_DATA &data, const QString& pagelist = QString(""));
	~TreeItem ();
	PLACEMARK_DATA GetItemData();
	void SetItemData(const PLACEMARK_DATA &data, const QString& pagelist = QString(""));

private:
	QString pagelist;
};

QDataStream & operator<< (QDataStream &out, const PLACEMARK_DATA &item);
QDataStream & operator>> (QDataStream &in, PLACEMARK_DATA &item);
QDataStream & operator<< (QDataStream &out, const ushort *text);
QDataStream & operator>> (QDataStream &in, ushort *text);
QDataStream & operator<< (QDataStream &out, const void *item);
QDataStream & operator>> (QDataStream &in, void *item);

#endif // TREEITEM_H
