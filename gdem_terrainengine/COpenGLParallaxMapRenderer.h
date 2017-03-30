// Copyright (C) 2002-2008 Nikolaus Gebhardt
// This file is part of the "Geo Engine".
// For conditions of distribution and use, see copyright notice in geoEngine.h

#ifndef __C_OPENGL_PARALLAX_MAP_RENDERER_H_INCLUDED__
#define __C_OPENGL_PARALLAX_MAP_RENDERER_H_INCLUDED__

#include "geoCompileConfig.h"
#ifdef _GEO_COMPILE_WITH_OPENGL_

#include "COpenGLShaderMaterialRenderer.h"
#include "IShaderConstantSetCallBack.h"

namespace geo
{
namespace video
{

//! Class for rendering normal maps with OpenGL
class COpenGLParallaxMapRenderer : public COpenGLShaderMaterialRenderer, public IShaderConstantSetCallBack
{
public:

	//! Constructor
	COpenGLParallaxMapRenderer(video::COpenGLDriver* driver,
		s32& outMaterialTypeNr, IMaterialRenderer* baseMaterial);

	//! Destructor
	~COpenGLParallaxMapRenderer();

	//! Called by the engine when the vertex and/or pixel shader constants for an
	//! material renderer should be set.
	virtual void OnSetConstants(IMaterialRendererServices* services, s32 userData);

	//! Returns the render capability of the material.
	virtual s32 getRenderCapability() const;

	virtual void OnSetMaterial(const video::SMaterial& material,
		const video::SMaterial& lastMaterial,
		bool resetAllRenderstates, video::IMaterialRendererServices* services);

protected:

	bool CompiledShaders;
	f32 CurrentScale;
};


} // end namespace video
} // end namespace geo

#endif
#endif

