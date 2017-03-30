#include "plisttablemodel.h"
#include "gdemclient.h"
#include "config.h"
#include "guiuty.h"

PointData::PointData(QString lati, QString longi, QString ht)
{
	m_latitude = lati;
	m_longitude = longi;
	m_height = ht;
}

PointData::~PointData()
{
}

PListTableModel::PListTableModel( QObject *parent)
	:QAbstractTableModel(parent)
{
}

PListTableModel::PListTableModel(QList<PointData> pointList, QObject *parent)
{
	m_pointList = pointList;
}

PListTableModel::~PListTableModel()
{
}

int PListTableModel::rowCount(const QModelIndex &parent) const
{
	return m_pointList.size();
}

int PListTableModel::columnCount(const QModelIndex &parent) const
{
    return 2;
}


QVariant PListTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();
    
    if (index.row() >= m_pointList.size() || index.row() < 0)
        return QVariant();
    
	if (role == Qt::DisplayRole || role == Qt::EditRole) 
	{
		PointData pi = m_pointList.at(index.row());
        
        if (index.column() == 0)
			return pi.m_latitude;
        else if (index.column() == 1)
			return pi.m_longitude;
		else if (index.column() == 2)
			return pi.m_height;
    }
    return QVariant();
}

QVariant PListTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();
    
    if (orientation == Qt::Horizontal) 
	{
        switch (section) {
            case 0:
                return tr("Latitude");
                
            case 1:
                return tr("Longitude");

			case 2:
				return tr("Height");
                
            default:
                return QVariant();
        }
    }
	
	if (orientation == Qt::Vertical) 
	{
		return QString("%1").arg(section+1);
    }

    return QVariant();
}

bool PListTableModel::insertRows(int position, int rows, const QModelIndex &index)
{
    Q_UNUSED(index);
    beginInsertRows(QModelIndex(), position, position+rows-1);
    
    for (int row=0; row < rows; row++) 
	{
		PointData pi("0", "0", "0");
		m_pointList.insert(position, pi);
    }

    endInsertRows();
    return true;
}

bool PListTableModel::removeRows(int position, int rows, const QModelIndex &index)
{
    Q_UNUSED(index);    
    beginRemoveRows(QModelIndex(), position, position+rows-1);
    
    for (int row=0; row < rows; ++row) 
	{
        m_pointList.removeAt(position);
    }

    endRemoveRows();
    return true;
}

QString ValueToString(double value) //asd 2014.6.4
{
	QString str;
	switch(g_app->GetConfig()->m_llShowMode)
	{
	case OPTION_LL_10NOTATION:
		str = QString("%1%2").arg(value, 10, 'f', 6).arg(QChar(0x00B0));
		break;
	case OPTION_LL_DMS:
		str = gu_Degree2DMS(value, GEO_TYPE_LATITUDE);
		break;
	case OPTION_LL_10NOTATION_DM:
		str = gu_Degree2DM(value, GEO_TYPE_LATITUDE);
		break;
	default:
		break;
	}
	return str;
}

double StringToValue(QString &str) //asd 2014.6.4
{
	double value;
	switch(g_app->GetConfig()->m_llShowMode)
	{
	case OPTION_LL_10NOTATION:
		value = gu_Degree2Degree(str);
		break;
	case OPTION_LL_DMS:
		value = gu_DMS2Degree(str, GEO_TYPE_LATITUDE);
		break;
	case OPTION_LL_10NOTATION_DM:
		value = gu_DM2Degree(str, GEO_TYPE_LATITUDE);
		break;
	default:
		break;
	}
	return value;
}

void PListTableModel::setData(CGDM_LocationArr &cPtArr)
{
	m_pointList.clear();

	for(int i = 0; i < cPtArr.size(); i++)
	{
		CGDM_Location3D &pt = cPtArr[i];
		PointData p;
		p.m_longitude = ValueToString(radian2degree(pt.m_tX));
		p.m_latitude  = ValueToString(radian2degree(pt.m_tY));
		p.m_height	  = 0.0;
		m_pointList.append(p);
	}

	reset();
}

void PListTableModel::getData(CGDM_LocationArr &cPtArr)
{
	cPtArr.clear();

	for(int i = 0; i < m_pointList.count(); i++)
	{
		PointData p = m_pointList.value(i);
		CGDM_Location3D pt;

		pt.m_tX = degree2radian(StringToValue(p.m_longitude));
		pt.m_tY = degree2radian(StringToValue(p.m_latitude));
		pt.m_tZ = 0;
		cPtArr.push_back(pt);
	}
}

bool PListTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
	bool ok = false;
	double pvalue = value.toDouble(&ok);

	if (!ok || !value.isValid())
		return false;

	if (index.isValid() && role == Qt::EditRole) 
	{
		int row = index.row();
		PointData p = m_pointList.value(row);
		
		if (index.column() == 0)
		{
			if (pvalue < -90 || pvalue > 90)
				return false;
			p.m_latitude = ValueToString(pvalue);
		}
		else if (index.column() == 1)
		{
			if (pvalue < -180 || pvalue > 180)
				return false;
			p.m_longitude = ValueToString(pvalue);
		}
		else if (index.column() == 2)
		{
			p.m_height = QString("%1").arg(pvalue, 0, 'f', 6);
		}
        else
            return false;
            
		m_pointList.replace(row, p);
		emit(dataChanged(index, index));
		
        return true;
	}

	return false;
}

Qt::ItemFlags PListTableModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;
    
    return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
}

QList< PointData > PListTableModel::getList()
{
    return m_pointList;
}



/*************************************************************************/

CPointListTableModel::CPointListTableModel( QObject *parent)
	:QAbstractTableModel(parent)
{
	m_pPtArr = NULL;
}

CPointListTableModel::~CPointListTableModel()
{
}

int CPointListTableModel::rowCount(const QModelIndex &parent) const
{
	if(m_pPtArr)
		return m_pPtArr->size();
	return 0;
}

int CPointListTableModel::columnCount(const QModelIndex &parent) const
{
    return 3;
}


QVariant CPointListTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

	if(m_pPtArr == NULL)
		return QVariant();
  
	if (index.row() >= m_pPtArr->size() || index.row() < 0)
        return QVariant();
    
	if (role == Qt::DisplayRole || role == Qt::EditRole) 
	{
		CGDM_Location3D pt = (*m_pPtArr)[index.row()];
        
        if (index.column() == 0)
			return pt.m_tX;
        else if (index.column() == 1)
			return pt.m_tY;
		else if (index.column() == 2)
			return pt.m_tZ;
    }
    return QVariant();
}

QVariant CPointListTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();
    
    if (orientation == Qt::Horizontal) 
	{
        switch (section) {
            case 0:
                return tr("Latitude");
                
            case 1:
                return tr("Longitude");

			case 2:
				return tr("Height");
                
            default:
                return QVariant();
        }
    }
	
	if (orientation == Qt::Vertical) 
	{
		return QString("%1").arg(section+1);
    }

    return QVariant();
}

bool CPointListTableModel::insertRows(int position, int rows, const QModelIndex &index)
{
 	if(!m_pPtArr)
		return false;
    Q_UNUSED(index);
    beginInsertRows(QModelIndex(), position, position+rows-1);
    
    for (int row=0; row < rows; row++) 
	{
		CGDM_Location3D pt;
		m_pPtArr->push_back(pt);
    }

    endInsertRows();
    return true;
}

bool CPointListTableModel::removeRows(int position, int rows, const QModelIndex &index)
{
	if(!m_pPtArr)
		return false;
    Q_UNUSED(index);    
    beginRemoveRows(QModelIndex(), position, position+rows-1);

    
    for (int row=0; row < rows; ++row) 
	{
		m_pPtArr->erase(position);
    }

    endRemoveRows();
    return true;
}

bool CPointListTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
	if(!m_pPtArr)
		return false;

	if (index.isValid() && role == Qt::EditRole) 
	{
		int row = index.row();

		CGDM_Location3D &pt = (*m_pPtArr)[row];
				
		if (index.column() == 0)
			pt.m_tX	= value.toDouble();
		else if (index.column() == 1)
			pt.m_tY	= value.toDouble();
		else if (index.column() == 2)
			pt.m_tZ	= value.toDouble();
        else
            return false;
            
		emit(dataChanged(index, index));
		
        return true;
	}

	return false;
}

Qt::ItemFlags CPointListTableModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;
    
    return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
}
