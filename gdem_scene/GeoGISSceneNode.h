// Copyright (C) 2002-2008 Nikolaus Gebhardt
// This file is part of the "Geo Engine".
// For conditions of distribution and use, see copyright notice in geoEngine.h

#ifndef _GEOPOLYGONSCENNODE_H_
#define _GEOPOLYGONSCENNODE_H_

#include "GDM_Vector3D.h"
#include "GDM_Location.h"
#include "ISceneManager.h"
#include "ISceneNode.h"
#include "GIS_Polygon.h"

namespace geo
{
namespace scene
{

//! Scene node which is a dynamic light. You can switch the light on and off by 
//! making it visible or not, and let it be animated by ordinary scene node animators.
class CGeoGISSceneNode : public ISceneNode
{
public:
	//! constructor
	CGeoGISSceneNode(ISceneNode* parent, ISceneManager* mgr, s32 id);
	virtual ~CGeoGISSceneNode() { };

	//! Returns type of the scene node
	virtual ESCENE_NODE_TYPE getType() const { return ESNT_MEASURE; }

	//! renders the node.
	virtual void render();
	virtual void OnRegisterSceneNode();

	//! returns the axis aligned bounding box of this node
	virtual const core::aabbox3d<f32>& getBoundingBox() const;

	//kimyi. modify param, implement, access property
	void	DrawTrackPts();

private:

	core::aabbox3d<f32>				 m_Box;
	video::SMaterial				 mat;
	double							 m_fOffsetRadius;

private:
	void	DrawPolyNode(CGIS_Polygon *pPolyNode);
	void	DrawPolySideSurface(core::array<core::vector3df> arrPt, core::vector3df &centerPt, video::SColor clr);
	void	DrawCoverPolygon(CGDM_MeshArr &meshs, video::SColor clr);
	double	GetOffsetRadius(CGIS_Node *pNode);
};


} // end namespace scene
} // end namespace geo

#endif //_GEOPOLYGONSCENNODE_H_