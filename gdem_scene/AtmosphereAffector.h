#ifndef __ATMOSPHEREAFFECTOR_H__
#define __ATMOSPHEREAFFECTOR_H__

#include "AtmospherePixelBuffer.h"


namespace geo
{

using namespace video;

namespace scene
{

class AtmosphereAffector
{
public:
	AtmosphereAffector(f32 radius, f32 depth);
	virtual ~AtmosphereAffector(void);

	void ApplyColor(vector3df& CameraPos, vector3df& LightDirection, S3DVertex* vertexBuffer, s32 nVertexCount);
private:
	void Init();
	void ApplyOneVertextColor(vector3df& CameraPos, vector3df& LightDirection, S3DVertex& Vertex);

private:
	s16 m_nSamples;
	u16 m_nPolygonMode;
	f32 m_Kr, m_Kr4PI;
	f32 m_Km, m_Km4PI;
	f32 m_ESun;
	f32 m_g;

	f32 m_fInnerRadius;
	f32 m_fOuterRadius;
	f32 m_fScale;
	f32 m_fWavelength[3];
	f32 m_fWavelength4[3];
	f32 m_fRayleighScaleDepth;
	f32 m_fMieScaleDepth;
	AtmospherePixelBuffer PixelBuffer;
};


}
}
#endif //__ATMOSPHEREAFFECTOR_H__
