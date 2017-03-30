#include "AtmospherePixelBuffer.h"

namespace geo
{
namespace scene
{

void AtmospherePixelBuffer::MakeCloudCell(f32 fExpose, f32 fSizeDisc)
{
	s32 i;
	s32 n = 0;
	u8 nIntensity;
	for(s32 y=0; y<m_nHeight; y++)
	{
		f32 fDy = (y+0.5f)/m_nHeight - 0.5f;
		for(s32 x=0; x<m_nWidth; x++)
		{
			f32 fDx = (x+0.5f)/m_nWidth - 0.5f;
			f32 fDist = sqrtf(fDx*fDx + fDy*fDy);
			f32 fIntensity = 2.0f - min_<f32>(2.0f, powf(2.0f, max_<f32>(fDist-fSizeDisc,0.0f)*fExpose));
			switch(m_nDataType)
			{
				case GL_UNSIGNED_BYTE:
					nIntensity = (u8)(fIntensity*255 + 0.5f);
					for(i=0; i<m_nChannels; i++)
						((u8 *)m_pBuffer)[n++] = nIntensity;
					break;
				case GL_FLOAT:
					for(i=0; i<m_nChannels; i++)
						((f32 *)m_pBuffer)[n++] = fIntensity;
					break;
			}
		}
	}
}

void AtmospherePixelBuffer::Make3DNoise(s32 nSeed)
{
	//CFractal noise(3, nSeed, 0.5f, 2.0f);
	//s32 n = 0;
	//f32 fValues[3];
	//for(s32 z=0; z<m_nDepth; z++)
	//{
	//	fValues[2] = (f32)z * 0.0625f;
	//	for(s32 y=0; y<m_nHeight; y++)
	//	{
	//		fValues[1] = (f32)y * 0.0625f;
	//		for(s32 x=0; x<m_nWidth; x++)
	//		{
	//			fValues[0] = (f32)x * 0.0625f;
	//			f32 fIntensity = abs_<f32>(noise.fBm(fValues, 4.0f)) - 0.5f;
	//			if(fIntensity < 0.0)
	//				fIntensity = 0.0f;
	//			fIntensity = 1.0f - powf(0.9f, fIntensity*255);
	//			u8 nIntensity = (u8)(fIntensity*255 + 0.5f);
	//			((u8 *)m_pBuffer)[n++] = 255;
	//			((u8 *)m_pBuffer)[n++] = nIntensity;
	//		}
	//	}
	//}
}

void AtmospherePixelBuffer::MakeGlow1D()
{
	s32 nIndex=0;
	for(s32 x=0; x<m_nWidth; x++)
	{
		f32 fIntensity = powf((f32)x / m_nWidth, 0.75f);
		for(s32 i=0; i<m_nChannels-1; i++)
			((u8 *)m_pBuffer)[nIndex++] = (u8)255;
		((u8 *)m_pBuffer)[nIndex++] = (u8)(fIntensity*255 + 0.5f);
	}
}

void AtmospherePixelBuffer::MakeOpticalDepthBuffer(f32 fInnerRadius, f32 fOuterRadius, f32 fRayleighScaleHeight, f32 fMieScaleHeight)
{
	const s32 nSize = 128;
	const s32 nSamples = 10;
#if ATMOSPHERE_BUG_FIX
	fInnerRadius /= ATMOSPHERE_SCALE;
	fOuterRadius /= ATMOSPHERE_SCALE;
#endif
	const f32 fScale = 1.0f / (fOuterRadius - fInnerRadius);
	Init(nSize, nSize, 1, 4, GL_RGBA, GL_FLOAT);
	s32 nIndex = 0;
	for(s32 nAngle=0; nAngle<nSize; nAngle++)
	{
		// As the y tex coord goes from 0 to 1, the angle goes from 0 to 180 degrees
		f32 fCos = 1.0f - (nAngle+nAngle) / (f32)nSize;
		f32 fAngle = acosf(fCos);
		vector3df vRay(sinf(fAngle), cosf(fAngle), 0);	// Ray pointing to the viewpoint
		for(s32 nHeight=0; nHeight<nSize; nHeight++)
		{
			// As the x tex coord goes from 0 to 1, the height goes from the bottom of the atmosphere to the top
			f32 fHeight = DELTA + fInnerRadius + ((fOuterRadius - fInnerRadius) * nHeight) / nSize;
			vector3df vPos(0, fHeight, 0);				// The position of the camera

			// If the ray from vPos heading in the vRay direction intersects the inner radius (i.e. the planet), then this spot is not visible from the viewpoint
			f32 B = 2.0f * (vPos.dotProduct(vRay));
			f32 Bsq = B * B;
			f32 Cpart = (vPos.dotProduct(vPos));
			f32 C = Cpart - fInnerRadius*fInnerRadius;
			f32 fDet = Bsq - 4.0f * C;
			bool bVisible = (fDet < 0 || (0.5f * (-B - sqrtf(fDet)) <= 0) && (0.5f * (-B + sqrtf(fDet)) <= 0));
			f32 fRayleighDensityRatio;
			f32 fMieDensityRatio;
			if(bVisible)
			{
				fRayleighDensityRatio = expf(-(fHeight - fInnerRadius) * fScale / fRayleighScaleHeight);
				fMieDensityRatio = expf(-(fHeight - fInnerRadius) * fScale / fMieScaleHeight);
			}
			else
			{
				// Smooth the transition from light to shadow (it is a soft shadow after all)
				fRayleighDensityRatio = ((f32 *)m_pBuffer)[nIndex - nSize*m_nChannels] * 0.75f;
				fMieDensityRatio = ((f32 *)m_pBuffer)[nIndex+2 - nSize*m_nChannels] * 0.75f;
			}

			// Determine where the ray intersects the outer radius (the top of the atmosphere)
			// This is the end of our ray for determining the optical depth (vPos is the start)
			C = Cpart - fOuterRadius*fOuterRadius;
			fDet = Bsq - 4.0f * C;
			f32 fFar = 0.5f * (-B + sqrtf(fDet));

			// Next determine the length of each sample, scale the sample ray, and make sure position checks are at the center of a sample ray
			f32 fSampleLength = fFar / nSamples;
			f32 fScaledLength = fSampleLength * fScale;
			vector3df vSampleRay = vRay * fSampleLength;
			vPos += vSampleRay * 0.5f;

			// Iterate through the samples to sum up the optical depth for the distance the ray travels through the atmosphere
			f32 fRayleighDepth = 0;
			f32 fMieDepth = 0;
			for(s32 i=0; i<nSamples; i++)
			{
				f32 fHeight = vPos.getLength();
				f32 fAltitude = (fHeight - fInnerRadius) * fScale;
				fAltitude = max_<f32>(fAltitude, 0.0f);
				fRayleighDepth += expf(-fAltitude / fRayleighScaleHeight);
				fMieDepth += expf(-fAltitude / fMieScaleHeight);
				vPos += vSampleRay;
			}
			// Multiply the sums by the length the ray traveled
			fRayleighDepth *= fScaledLength;
			fMieDepth *= fScaledLength;

			// Store the results for Rayleigh to the light source, Rayleigh to the camera, Mie to the light source, and Mie to the camera
			((f32 *)m_pBuffer)[nIndex++] = fRayleighDensityRatio;
			((f32 *)m_pBuffer)[nIndex++] = fRayleighDepth;
			((f32 *)m_pBuffer)[nIndex++] = fMieDensityRatio;
			((f32 *)m_pBuffer)[nIndex++] = fMieDepth;
		}
	}
}

void AtmospherePixelBuffer::MakePhaseBuffer(f32 ESun, f32 Kr, f32 Km, f32 g)
{
	Km *= ESun;
	Kr *= ESun;
	f32 g2 = g*g;
	f32 fMiePart = 1.5f * (1.0f - g2) / (2.0f + g2);

	s32 nIndex = 0;
	for(s32 nAngle=0; nAngle<m_nWidth; nAngle++)
	{
		f32 fCos = 1.0f - (nAngle+nAngle) / (f32)m_nWidth;
		f32 fCos2 = fCos*fCos;
		f32 fRayleighPhase = 0.75f * (1.0f + fCos2);
		f32 fMiePhase = fMiePart * (1.0f + fCos2) / powf(1.0f + g2 - 2.0f*g*fCos, 1.5f);
		((f32 *)m_pBuffer)[nIndex++] = fRayleighPhase * Kr;
		((f32 *)m_pBuffer)[nIndex++] = fMiePhase * Km;
	}
}

}
}