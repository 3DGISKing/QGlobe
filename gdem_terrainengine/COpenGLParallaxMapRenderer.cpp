// Copyright (C) 2002-2008 Nikolaus Gebhardt
// This file is part of the "Geo Engine".
// For conditions of distribution and use, see copyright notice in geoEngine.h

#include "geoCompileConfig.h"
#ifdef _GEO_COMPILE_WITH_OPENGL_

#include "COpenGLParallaxMapRenderer.h"
#include "COpenGLDriver.h"
#include "IGPUProgrammingServices.h"
#include "IShaderConstantSetCallBack.h"
#include "IVideoDriver.h"
#include "os.h"

namespace geo
{
namespace video
{

// Geo Engine OpenGL render path parallax map vertex shader
// I guess it could be optimized a lot, because I wrote it in D3D ASM and
// transferred it 1:1 to OpenGL
const char OPENGL_PARALLAX_MAP_VSH[] =
	"!!ARBvp1.0\n"\
	"#input\n"\
	"# 0-3: transposed world matrix;\n"\
	"#;12: Light01 position \n"\
	"#;13: x,y,z: Light01 color; .w: 1/LightRadius^2 \n"\
	"#;14: Light02 position \n"\
	"#;15: x,y,z: Light02 color; .w: 1/LightRadius^2 \n"\
	"#;16: Eye position \n"\
	"\n"\
	"ATTRIB InPos = vertex.position;\n"\
	"ATTRIB InColor = vertex.color;\n"\
	"ATTRIB InNormal = vertex.normal;\n"\
	"ATTRIB InTexCoord = vertex.texcoord[0];\n"\
	"ATTRIB InTangent = vertex.texcoord[1];\n"\
	"ATTRIB InBinormal = vertex.texcoord[2];\n"\
	"\n"\
	"#output\n"\
	"OUTPUT OutPos = result.position;\n"\
	"OUTPUT OutLightColor1 = result.color.primary;\n"\
	"OUTPUT OutLightColor2 = result.color.secondary;\n"\
	"OUTPUT OutTexCoord = result.texcoord[0];\n"\
	"OUTPUT OutLightVector1 = result.texcoord[1];\n"\
	"OUTPUT OutLightVector2 = result.texcoord[2];\n"\
	"OUTPUT OutEyeVector = result.texcoord[3];\n"\
	"\n"\
	"PARAM MVP[4] = { state.matrix.mvp }; # modelViewProjection matrix.\n"\
	"TEMP Temp;\n"\
	"TEMP TempColor;\n"\
	"TEMP TempNormal;\n"\
	"TEMP TempTangent;\n"\
	"TEMP TempBinormal;\n"\
	"TEMP TempLightVector1;\n"\
	"TEMP TempLightVector2;\n"\
	"TEMP TempEyeVector;\n"\
	"TEMP TempTransLightV1;\n"\
	"TEMP TempTransLightV2;\n"\
	"\n"\
	"# transform position to clip space \n"\
	"DP4 OutPos.x, MVP[0], InPos;\n"\
	"DP4 OutPos.y, MVP[1], InPos;\n"\
	"DP4 OutPos.z, MVP[2], InPos;\n"\
	"DP4 OutPos.w, MVP[3], InPos;\n"\
	"\n"\
	"# transform normal \n"\
	"DP3 TempNormal.x, InNormal.x, program.local[0];\n"\
	"DP3 TempNormal.y, InNormal.y, program.local[1]; \n"\
	"DP3 TempNormal.z, InNormal.z, program.local[2];\n"\
	"\n"\
	"# transform tangent \n"\
	"DP3 TempTangent.x, InTangent.x, program.local[0];\n"\
	"DP3 TempTangent.y, InTangent.y, program.local[1]; \n"\
	"DP3 TempTangent.z, InTangent.z, program.local[2];\n"\
	"\n"\
	"# transform binormal \n"\
	"DP3 TempBinormal.x, InBinormal.x, program.local[0];\n"\
	"DP3 TempBinormal.y, InBinormal.y, program.local[1]; \n"\
	"DP3 TempBinormal.z, InBinormal.z, program.local[2];\n"\
	"\n"\
	"# vertex into world position \n"\
	"DP4 Temp.x, InPos, program.local[0];\n"\
	"DP4 Temp.y, InPos, program.local[1];\n"\
	"DP4 Temp.z, InPos, program.local[2];\n"\
	"DP4 Temp.w, InPos, program.local[3];\n"\
	"\n"\
	"# vertex - lightpositions \n"\
	"SUB TempLightVector1, program.local[12], Temp; \n"\
	"SUB TempLightVector2, program.local[14], Temp; \n"\
	"\n"\
	"# eye vector \n"\
	"SUB Temp, program.local[16], Temp; \n"\
	"\n"\
	"# transform the light vector 1 with U, V, W \n"\
	"DP3 TempTransLightV1.x, TempTangent, TempLightVector1; \n"\
	"DP3 TempTransLightV1.y, TempBinormal, TempLightVector1; \n"\
	"DP3 TempTransLightV1.z, TempNormal, TempLightVector1; \n"\
	"\n"\
	"# transform the light vector 2 with U, V, W \n"\
	"DP3 TempTransLightV2.x, TempTangent, TempLightVector2; \n"\
	"DP3 TempTransLightV2.y, TempBinormal, TempLightVector2; \n"\
	"DP3 TempTransLightV2.z, TempNormal, TempLightVector2; \n"\
	"\n"\
	"# transform the eye vector with U, V, W \n"\
	"DP3 TempEyeVector.x, TempTangent, Temp; \n"\
	"DP3 TempEyeVector.y, TempBinormal, Temp; \n"\
	"DP3 TempEyeVector.z, TempNormal, Temp; \n"\
	"\n"\
	"# normalize light vector 1 \n"\
	"DP3 TempTransLightV1.w, TempTransLightV1, TempTransLightV1; \n"\
	"RSQ TempTransLightV1.w, TempTransLightV1.w; \n"\
	"MUL TempTransLightV1, TempTransLightV1, TempTransLightV1.w;\n"\
	"\n"\
	"# normalize light vector 2 \n"\
	"DP3 TempTransLightV2.w, TempTransLightV2, TempTransLightV2; \n"\
	"RSQ TempTransLightV2.w, TempTransLightV2.w; \n"\
	"MUL TempTransLightV2, TempTransLightV2, TempTransLightV2.w;\n"\
	"\n"\
	"# normalize eye vector \n"\
	"DP3 TempEyeVector.w, TempEyeVector, TempEyeVector; \n"\
	"RSQ TempEyeVector.w, TempEyeVector.w; \n"\
	"MUL TempEyeVector, TempEyeVector, TempEyeVector.w;\n"\
	"MUL TempEyeVector, TempEyeVector, {1,-1,-1,1}; # flip x \n"\
	"\n"\
	"\n"\
	"# move light and eye vectors out\n"\
	"MAD OutLightVector1, TempTransLightV1, {0.5,0.5,0.5,0.5}, {0.5,0.5,0.5,0.5}; \n"\
	"MAD OutLightVector2, TempTransLightV2, {0.5,0.5,0.5,0.5}, {0.5,0.5,0.5,0.5}; \n"\
	"MAD OutEyeVector, TempEyeVector, {0.5,0.5,0.5,0.5}, {0.5,0.5,0.5,0.5}; \n"\
	"\n"\
	"# calculate attenuation of light 1\n"\
	"MOV TempLightVector1.w, {0,0,0,0}; \n"\
	"DP3 TempLightVector1.x, TempLightVector1, TempLightVector1; \n"\
	"MUL TempLightVector1.x, TempLightVector1.x, program.local[13].w;  \n"\
	"RSQ TempLightVector1, TempLightVector1.x; \n"\
	"MUL OutLightColor1, TempLightVector1, program.local[13]; # resulting light color = lightcolor * attenuation \n"\
	"\n"\
	"# calculate attenuation of light 2\n"\
	"MOV TempLightVector2.w, {0,0,0,0}; \n"\
	"DP3 TempLightVector2.x, TempLightVector2, TempLightVector2; \n"\
	"MUL TempLightVector2.x, TempLightVector2.x, program.local[15].w;  \n"\
	"RSQ TempLightVector2, TempLightVector2.x; \n"\
	"MUL OutLightColor2, TempLightVector2, program.local[15]; # resulting light color = lightcolor * attenuation \n"\
	"\n"\
	"# move out texture coordinates and original alpha value\n"\
	"MOV OutTexCoord, InTexCoord; \n"\
	"MOV OutLightColor1.w, InColor.w; \n"\
	"\n"\
	"END\n";

// Geo Engine OpenGL render path parallax map pixel shader
// I guess it could be optimized a bit, because I wrote it in D3D ASM and
// transfered it 1:1 to OpenGL
const char OPENGL_PARALLAX_MAP_PSH[] =
	"!!ARBfp1.0\n"\
	"\n"\
	"#Input\n"\
	"ATTRIB inTexCoord = fragment.texcoord[0];   \n"\
	"ATTRIB light1Vector = fragment.texcoord[1]; \n"\
	"ATTRIB light2Vector = fragment.texcoord[2];    \n"\
	"ATTRIB eyeVector = fragment.texcoord[3];    \n"\
	"ATTRIB light1Color = fragment.color.primary;   \n"\
	"ATTRIB light2Color = fragment.color.secondary; \n"\
	"\n"\
	"#Output\n"\
	"OUTPUT outColor = result.color;\n"\
	"TEMP temp;\n"\
	"TEMP temp2;\n"\
	"TEMP colorMapColor;\n"\
	"TEMP normalMapColor;\n"\
	"\n"\
	"PARAM height_scale = program.local[0]; \n"\
	"# fetch color and normal map; \n"\
	"TXP normalMapColor, inTexCoord, texture[1], 2D; \n"\
	"MAD normalMapColor, normalMapColor, {2,2,2,2}, {-1,-1,-1,-1}; \n"\
	"\n"\
	"\n"\
	"# extract eye vector (so substract 0.5f and multiply by 2)\n"\
	"MAD temp, eyeVector, {2,2,2,2}, {-1,-1,-1,-1};\n"\
	"\n"\
	"# height = height * scale \n"\
	"MUL normalMapColor, normalMapColor, height_scale;\n"\
	"\n"\
	"# calculate new texture coord: height * eye + oldTexCoord\n"\
	"MAD temp, temp, normalMapColor.wwww, inTexCoord;\n"\
	"\n"\
	"# fetch new textures \n"\
	"TXP colorMapColor, temp, texture[0], 2D; \n"\
	"TXP normalMapColor, temp, texture[1], 2D; \n"\
	"\n"\
	"# calculate color of light1; \n"\
	"MAD normalMapColor, normalMapColor, {2,2,2,2}, {-1,-1,-1,-1}; \n"\
	"MAD temp, light1Vector, {2,2,2,2}, {-1,-1,-1,-1}; \n"\
	"DP3_SAT temp, normalMapColor, temp; \n"\
	"MUL temp, light1Color, temp; \n"\
	"\n"\
	"# calculate color of light2; \n"\
	"MAD temp2, light2Vector, {2,2,2,2}, {-1,-1,-1,-1}; \n"\
	"DP3_SAT temp2, normalMapColor, temp2; \n"\
	"MAD temp, light2Color, temp2, temp; \n"\
	"\n"\
	"# luminance * base color; \n"\
	"MUL outColor, temp, colorMapColor; \n"\
	"MOV outColor.a, light1Color.a; #write interpolated vertex alpha value\n"\
	"\n"\
	"END\n";

//! Constructor
COpenGLParallaxMapRenderer::COpenGLParallaxMapRenderer(video::COpenGLDriver* driver,
	s32& outMaterialTypeNr, IMaterialRenderer* baseMaterial)
	: COpenGLShaderMaterialRenderer(driver, 0, baseMaterial), CompiledShaders(true)
{

	#ifdef _DEBUG
	setDebugName("COpenGLParallaxMapRenderer");
	#endif

	// set this as callback. We could have done this in
	// the initialization list, but some compilers don't like it.

	CallBack = this;

	// basically, this simply compiles the hard coded shaders if the
	// hardware is able to do them, otherwise it maps to the base material

	if (!driver->queryFeature(video::EVDF_ARB_FRAGMENT_PROGRAM_1) ||
		!driver->queryFeature(video::EVDF_ARB_VERTEX_PROGRAM_1))
	{
		// this hardware is not able to do shaders. Fall back to
		// base material.
		outMaterialTypeNr = driver->addMaterialRenderer(this);
		return;
	}

	// check if already compiled normal map shaders are there.

	video::IMaterialRenderer* renderer = driver->getMaterialRenderer(EMT_PARALLAX_MAP_SOLID);

	if (renderer)
	{
		// use the already compiled shaders
		video::COpenGLParallaxMapRenderer* nmr = reinterpret_cast<video::COpenGLParallaxMapRenderer*>(renderer);
		CompiledShaders = false;

		VertexShader = nmr->VertexShader;
		PixelShader = nmr->PixelShader;

		outMaterialTypeNr = driver->addMaterialRenderer(this);
	}
	else
	{
		// compile shaders on our own
		init(outMaterialTypeNr, OPENGL_PARALLAX_MAP_VSH, OPENGL_PARALLAX_MAP_PSH, EVT_TANGENTS);
	}

	// fallback if compilation has failed
	if (-1==outMaterialTypeNr)
		outMaterialTypeNr = driver->addMaterialRenderer(this);
}


//! Destructor
COpenGLParallaxMapRenderer::~COpenGLParallaxMapRenderer()
{
	if (CallBack == this)
		CallBack = 0;

	if (!CompiledShaders)
	{
		// prevent this from deleting shaders we did not create
		VertexShader = 0;
		PixelShader = 0;
	}
}


void COpenGLParallaxMapRenderer::OnSetMaterial(const video::SMaterial& material,
	const video::SMaterial& lastMaterial,
	bool resetAllRenderstates, video::IMaterialRendererServices* services)
{
	COpenGLShaderMaterialRenderer::OnSetMaterial(material, lastMaterial,
			resetAllRenderstates, services);

	CurrentScale = material.MaterialTypeParam;
}



//! Returns the render capability of the material.
s32 COpenGLParallaxMapRenderer::getRenderCapability() const
{
	if (Driver->queryFeature(video::EVDF_ARB_FRAGMENT_PROGRAM_1) &&
		Driver->queryFeature(video::EVDF_ARB_VERTEX_PROGRAM_1))
		return 0;

	return 1;
}


//! Called by the engine when the vertex and/or pixel shader constants for an
//! material renderer should be set.
void COpenGLParallaxMapRenderer::OnSetConstants(IMaterialRendererServices* services, s32 userData)
{
	video::IVideoDriver* driver = services->getVideoDriver();

	// set transposed world matrix
	const core::matrix4& tWorld = driver->getTransform(video::ETS_WORLD).getTransposed();
	services->setVertexShaderConstant(tWorld.pointer(), 0, 4);

	// The  viewpoint is at (0., 0., 0.) in eye space.
	// Turning this into a vector [0 0 0 1] and multiply it by
	// the inverse of the view matrix, the resulting vector is the
	// object space location of the camera.

	f32 floats[4] = {0.0f,0.0f,0.0f,1.0f};
	core::matrix4 minv(driver->getTransform(video::ETS_VIEW));
	minv.makeInverse();
	minv.multiplyWith1x4Matrix(floats);
	services->setVertexShaderConstant(floats, 16, 1);

	// set transposed worldViewProj matrix
	core::matrix4 worldViewProj(driver->getTransform(video::ETS_PROJECTION));
	worldViewProj *= driver->getTransform(video::ETS_VIEW);
	worldViewProj *= driver->getTransform(video::ETS_WORLD);
	core::matrix4 tr(worldViewProj.getTransposed());
	services->setVertexShaderConstant(tr.pointer(), 8, 4);

	// here we fetch the fixed function lights from the driver
	// and set them as constants

	u32 cnt = driver->getDynamicLightCount();

	for (u32 i=0; i<2; ++i)
	{
		video::SLight light;

		if (i<cnt)
			light = driver->getDynamicLight(i);
		else
		{
			light.DiffuseColor.set(0,0,0); // make light dark
			light.Radius = 1.0f;
		}

		light.DiffuseColor.a = 1.0f/(light.Radius*light.Radius); // set attenuation

		services->setVertexShaderConstant(
			reinterpret_cast<const f32*>(&light.Position), 12+(i*2), 1);

		services->setVertexShaderConstant(
			reinterpret_cast<const f32*>(&light.DiffuseColor), 13+(i*2), 1);
	}

	// set scale factor
	f32 factor = 0.02f; // default value
	if (CurrentScale != 0.0f)
		factor = CurrentScale;

	f32 c6[] = {factor, factor, factor, factor};
	services->setPixelShaderConstant(c6, 0, 1);
}


} // end namespace video
} // end namespace geo


#endif

