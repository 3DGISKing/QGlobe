#include "GIS_Dataset.h"
#include "../qglobe_common/qglobe_protocol.h"

GISLayer::~GISLayer()
{
	for(int i=0;i<FeatureCount();i++)
	{
		CGIS_Node* feature=GetFeature(i);

		delete feature;
	}

	_featureList.clear();
}

bool GISLayer::ExistFeature(int id)
{
	unsigned int key=id;

	if(FeatureCount()==0) return false;

	for(int i=0;i<FeatureCount();i++)
	{
		CGIS_Node* feature=GetFeature(i);

		if (feature->GetKey()==key)
		{
			return true;
		}
	}
	return false;
}

PolygonLayer::~PolygonLayer()
{

}

PathLayer::~PathLayer()
{

}