#ifndef PLISTTABLEMODEL_H
#define PLISTTABLEMODEL_H

#include <QObject>
#include <QAbstractTableModel>
#include <QList>
#include "GDM_Vector3D.h"

class PointData
{
public:
	PointData(QString lati = 0, QString longi = 0, QString ht = 0);
	~PointData();

	QString m_latitude;
	QString m_longitude;
	QString m_height;
};

inline bool operator==(const PointData &pd1, const PointData &pd2)
{
	return pd1.m_latitude == pd2.m_latitude
		&& pd1.m_longitude == pd2.m_longitude
		&& pd1.m_height == pd2.m_height;
}

inline bool operator!=(const PointData &pd1, const PointData &pd2)
{
	return pd1.m_latitude != pd2.m_latitude
		|| pd1.m_longitude != pd2.m_longitude
		|| pd1.m_height != pd2.m_height;
}

class PListTableModel : public QAbstractTableModel
{
    Q_OBJECT
    
public:
    PListTableModel(QObject *parent=0);
    PListTableModel(QList< PointData > pointList, QObject *parent=0);
	~PListTableModel();
  
	int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role=Qt::EditRole);
    bool insertRows(int position, int rows, const QModelIndex &index=QModelIndex());
    bool removeRows(int position, int rows, const QModelIndex &index=QModelIndex());
    QList< PointData > getList();
	void setData(CGDM_LocationArr &cPtArr);
	void getData(CGDM_LocationArr &cPtArr);

private:
    QList< PointData > m_pointList;
};


class CPointListTableModel : public QAbstractTableModel
{
    Q_OBJECT
    
public:
    CPointListTableModel(QObject *parent=0);
	~CPointListTableModel();
  
	int				rowCount(const QModelIndex &parent) const;
    int				columnCount(const QModelIndex &parent) const;
    QVariant		data(const QModelIndex &index, int role) const;
    QVariant		headerData(int section, Qt::Orientation orientation, int role) const;
    Qt::ItemFlags	flags(const QModelIndex &index) const;
    bool			setData(const QModelIndex &index, const QVariant &value, int role=Qt::EditRole);
    bool			insertRows(int position, int rows, const QModelIndex &index=QModelIndex());
    bool			removeRows(int position, int rows, const QModelIndex &index=QModelIndex());
	void			setDataPoint(CGDM_LocationArr *pPtArr) {m_pPtArr = pPtArr;}

private:
	CGDM_LocationArr	*m_pPtArr;
};


#endif // PLISTTABLEMODEL_H