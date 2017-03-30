#ifndef GIS_DATASET_H
#define GIS_DATASET_H

#include <QtCore/QList>

#include "GIS_Area.h"
#include "GIS_Path.h"

class GISLayer
{
public:
	enum LayerType
	{
		GISPoygon,
		GISPath
	};

	virtual ~GISLayer();

	virtual void AddFeature(CGIS_Node* f)=0;
	inline  int  FeatureCount() {return _featureList.size();}
	bool         ExistFeature(int id);
	CGIS_Node*   GetFeature(int i) 
	{
		Q_ASSERT(i>=0 && i<FeatureCount());

		return  _featureList[i];
	}

	inline LayerType GetType() {return _layerType;}

protected:
	LayerType _layerType;
	QList<CGIS_Node*> _featureList;
};

class PolygonLayer :public GISLayer
{
public:
	PolygonLayer()
	{
		_layerType=GISPoygon;
	}

	~PolygonLayer();

	virtual void AddFeature(CGIS_Node* f)
	{
		Q_ASSERT(f->Get_GisType()==E_GISNODE_TYPE_AREA);
		_featureList.push_back(f);
	}
private:
};

class PathLayer: public GISLayer
{
public:
	PathLayer()
	{
		_layerType=GISPath;
	}

	~PathLayer();
	
	virtual void AddFeature(CGIS_Node* f)
	{
		Q_ASSERT(f->Get_GisType()==E_GISNODE_TYPE_PATH);
		_featureList.push_back(f);
	}

private:
};

#endif
