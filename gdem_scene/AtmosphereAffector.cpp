#include "AtmosphereAffector.h"

namespace geo
{
namespace scene
{

	AtmosphereAffector::AtmosphereAffector(f32 radius, f32 depth)
	{
		m_fInnerRadius = radius;
		m_fOuterRadius = radius + depth;

		Init();
	}

	AtmosphereAffector::~AtmosphereAffector()
	{

	}

	void AtmosphereAffector::Init()
	{
		m_nPolygonMode = GL_FILL;

		m_nSamples = 4;		// Number of sample rays to use in integral equation
		m_Kr = 0.0025f;		// Rayleigh scattering constant
		m_Kr4PI = m_Kr*4.0f*PI;
		m_Km = 0.0025f;		// Mie scattering constant
		m_Km4PI = m_Km*4.0f*PI;
		m_ESun = 15.0f;		// Sun brightness constant
		m_g = -0.75f;		// The Mie phase asymmetry factor
#if ATMOSPHERE_BUG_FIX
		m_fScale = ATMOSPHERE_SCALE / (m_fOuterRadius - m_fInnerRadius);
#else
		m_fScale = 1 / (m_fOuterRadius - m_fInnerRadius);
#endif
		m_fWavelength[0] = 0.650f;		// 650 nm for red
		m_fWavelength[1] = 0.570f;		// 570 nm for green
		m_fWavelength[2] = 0.475f;		// 475 nm for blue
		m_fWavelength4[0] = powf(m_fWavelength[0], 4.0f);
		m_fWavelength4[1] = powf(m_fWavelength[1], 4.0f);
		m_fWavelength4[2] = powf(m_fWavelength[2], 4.0f);

		m_fRayleighScaleDepth = 0.25f;
		m_fMieScaleDepth = 0.1f;
		PixelBuffer.MakeOpticalDepthBuffer(m_fInnerRadius, m_fOuterRadius, m_fRayleighScaleDepth, m_fMieScaleDepth);
	}

	void AtmosphereAffector::ApplyColor(vector3df& CameraPos, vector3df& LightDirection, S3DVertex* vertexBuffer, s32 nVertexCount)
	{
		for(s32 index = 0 ; index < nVertexCount ; index++)
		{
			if(CameraPos.dotProduct(vertexBuffer[index].Pos) > 0)
				ApplyOneVertextColor(CameraPos, LightDirection, vertexBuffer[index]);
		}
	}

	void AtmosphereAffector::ApplyOneVertextColor(vector3df& CameraPos, vector3df& LightDirection, S3DVertex& Vertex)
	{
		vector3df vPos = Vertex.Pos;
		// Get the ray from the camera to the vertex, and its length (which is the far point of the ray passing through the atmosphere)
#if ATMOSPHERE_BUG_FIX
		float fInnerRadiusOrg = m_fInnerRadius;
		float fOuterRadiusOrg = m_fOuterRadius;
		vector3df vCameraPosOrg = CameraPos;
		vector3df vLightDirectionOrg = LightDirection;

		vPos /= ATMOSPHERE_SCALE;
		CameraPos /= ATMOSPHERE_SCALE;
		m_fOuterRadius /= ATMOSPHERE_SCALE;
		m_fInnerRadius /= ATMOSPHERE_SCALE;
		LightDirection = CameraPos;
		LightDirection.normalize();
#endif

		vector3df vRay = vPos - CameraPos;
		f32 fFar = vRay.getLength();
		vRay /= fFar;

		// Calculate the closest intersection of the ray with the outer atmosphere (which is the near point of the ray passing through the atmosphere)
		f32 B = 2.0f * (CameraPos.dotProduct(vRay));
		f32 C = (CameraPos.getLengthSQ()) - m_fOuterRadius*m_fOuterRadius;
		f32 fDet = max_<f32>(0.0f, B*B - 4.0f * C);
		f32 fNear = 0.5f * (-B - sqrtf(fDet));

		bool bCameraInAtmosphere = false;
		bool bCameraAbove = true;
		f32 fCameraDepth[4] = {0, 0, 0, 0};
		f32 fLightDepth[4];
		f32 fSampleDepth[4];
#if ATMOSPHERE_BUG_FIX
		f32 fAlpha=1.0f;
#endif
		if(fNear <= 0)
		{
			// If the near point is behind the camera, it means the camera is inside the atmosphere
			bCameraInAtmosphere = true;
			fNear = 0;
			f32 fCameraHeight = CameraPos.getLength();
			f32 fCameraAltitude = (fCameraHeight - m_fInnerRadius) * m_fScale;
			bCameraAbove = fCameraHeight >= vPos.getLength();
			f32 fCameraAngle = ((bCameraAbove ? -vRay : vRay).dotProduct(CameraPos)) / fCameraHeight;
			PixelBuffer.Interpolate(fCameraDepth, fCameraAltitude, 0.5f - fCameraAngle * 0.5f);

#if ATMOSPHERE_BUG_FIX
			f32 fDetVisible = B*B - 4.0f*(CameraPos.getLengthSQ()-m_fInnerRadius*m_fInnerRadius);
			bool bVisible = (fDetVisible < 0 || (0.5f * (-B - sqrtf(fDetVisible)) <= 0) && (0.5f * (-B + sqrtf(fDetVisible)) <= 0));
			if(bVisible)
			{
				f32 fDistTangentLine = sqrtf(m_fOuterRadius*m_fOuterRadius-m_fInnerRadius*m_fInnerRadius)+sqrtf(max_<f32>(0.0f, fCameraHeight*fCameraHeight-m_fInnerRadius*m_fInnerRadius)); 
				vector3df vDist = vPos - CameraPos;
				f32 fDist = vDist.getLength();
				fAlpha = 1.0f-expf((-fDist)/(fDistTangentLine*m_fRayleighScaleDepth));
			}
#endif
		}
		else
		{
#if ATMOSPHERE_BUG_FIX
			f32 fDetVisible = B*B - 4.0f*(CameraPos.getLengthSQ()-m_fInnerRadius*m_fInnerRadius);
			bool bVisible = (fDetVisible < 0 || (0.5f * (-B - sqrtf(fDetVisible)) <= 0) && (0.5f * (-B + sqrtf(fDetVisible)) <= 0));
			if(bVisible)
			{
				f32 fDistTangentLine = 2.0f*sqrtf(m_fOuterRadius*m_fOuterRadius-m_fInnerRadius*m_fInnerRadius);
				f32 fDist = sqrtf(max_<f32>(0.0f, B*B+4.0f*(m_fOuterRadius*m_fOuterRadius - CameraPos.getLengthSQ())));
				fAlpha = 1.0f-expf((-fDist)/(fDistTangentLine*m_fRayleighScaleDepth));
			}
#endif
			// Otherwise, move the camera up to the near intersection point
			CameraPos += vRay * fNear;
			fFar -= fNear;
			fNear = 0;
		}

		// If the distance between the points on the ray is negligible, don't bother to calculate anything
		if(fFar <= DELTA)
		{
			glColor4f(0, 0, 0, 1);
#if ATMOSPHERE_BUG_FIX
			m_fInnerRadius = fInnerRadiusOrg;
			m_fOuterRadius = fOuterRadiusOrg;
			CameraPos = vCameraPosOrg;
			LightDirection = vLightDirectionOrg;
#endif
			return;
		}

		// Initialize a few variables to use inside the loop
		f32 fRayleighSum[3] = {0, 0, 0};
		f32 fMieSum[3] = {0, 0, 0};
		f32 fSampleLength = fFar / m_nSamples;
		f32 fScaledLength = fSampleLength * m_fScale;
		vector3df vSampleRay = vRay * fSampleLength;

		// Start at the center of the first sample ray, and loop through each of the others
		vPos = CameraPos + vSampleRay * 0.5f;
		for(int i=0; i<m_nSamples; i++)
		{
			f32 fHeight = vPos.getLength();

			// Start by looking up the optical depth coming from the light source to this point
			f32 fLightAngle = (LightDirection.dotProduct(vPos)) / fHeight;
			f32 fAltitude = (fHeight - m_fInnerRadius) * m_fScale;
			PixelBuffer.Interpolate(fLightDepth, fAltitude, 0.5f - fLightAngle * 0.5f);

			// If no light light reaches this part of the atmosphere, no light is scattered in at this point
			if(fLightDepth[0] < DELTA)
				continue;

			// Get the density at this point, along with the optical depth from the light source to this point
			f32 fRayleighDensity = fScaledLength * fLightDepth[0];
			f32 fRayleighDepth = fLightDepth[1];
			f32 fMieDensity = fScaledLength * fLightDepth[2];
			f32 fMieDepth = fLightDepth[3];

			// If the camera is above the point we're shading, we calculate the optical depth from the sample point to the camera
			// Otherwise, we calculate the optical depth from the camera to the sample point
			if(bCameraAbove)
			{
				f32 fSampleAngle = (-vRay.dotProduct(vPos)) / fHeight;
				PixelBuffer.Interpolate(fSampleDepth, fAltitude, 0.5f - fSampleAngle * 0.5f);
				fRayleighDepth += fSampleDepth[1] - fCameraDepth[1];
				fMieDepth += fSampleDepth[3] - fCameraDepth[3];
			}
			else
			{
				f32 fSampleAngle = (vRay.dotProduct(vPos)) / fHeight;
				PixelBuffer.Interpolate(fSampleDepth, fAltitude, 0.5f - fSampleAngle * 0.5f);
				fRayleighDepth += fCameraDepth[1] - fSampleDepth[1];
				fMieDepth += fCameraDepth[3] - fSampleDepth[3];
			}

			// Now multiply the optical depth by the attenuation factor for the sample ray
			fRayleighDepth *= m_Kr4PI;
			fMieDepth *= m_Km4PI;

			// Calculate the attenuation factor for the sample ray
			f32 fAttenuation[3];
			fAttenuation[0] = expf(-fRayleighDepth / m_fWavelength4[0] - fMieDepth);
			fAttenuation[1] = expf(-fRayleighDepth / m_fWavelength4[1] - fMieDepth);
			fAttenuation[2] = expf(-fRayleighDepth / m_fWavelength4[2] - fMieDepth);

			fRayleighSum[0] += fRayleighDensity * fAttenuation[0];
			fRayleighSum[1] += fRayleighDensity * fAttenuation[1];
			fRayleighSum[2] += fRayleighDensity * fAttenuation[2];

			fMieSum[0] += fMieDensity * fAttenuation[0];
			fMieSum[1] += fMieDensity * fAttenuation[1];
			fMieSum[2] += fMieDensity * fAttenuation[2];

			// Move the position to the center of the next sample ray
			vPos += vSampleRay;
		}

		// Calculate the angle and phase values (this block of code could be handled by a small 1D lookup table, or a 1D texture lookup in a pixel shader)
		f32 fAngle = -vRay.dotProduct(LightDirection);
		f32 fPhase[2];
		f32 fAngle2 = fAngle*fAngle;
		f32 g2 = m_g*m_g;
		fPhase[0] = 0.75f * (1.0f + fAngle2);
		fPhase[1] = 1.5f * ((1 - g2) / (2 + g2)) * (1.0f + fAngle2) / powf(1 + g2 - 2*m_g*fAngle, 1.5f);
		fPhase[0] *= m_Kr * m_ESun;
		fPhase[1] *= m_Km * m_ESun;

		// Calculate the in-scattering color and clamp it to the max color value
		f32 fColor[3] = {0, 0, 0};
		fColor[0] = fRayleighSum[0] * fPhase[0] / m_fWavelength4[0] + fMieSum[0] * fPhase[1];
		fColor[1] = fRayleighSum[1] * fPhase[0] / m_fWavelength4[1] + fMieSum[1] * fPhase[1];
		fColor[2] = fRayleighSum[2] * fPhase[0] / m_fWavelength4[2] + fMieSum[2] * fPhase[1];
		fColor[0] = min_<f32>(fColor[0], 1.0f);
		fColor[1] = min_<f32>(fColor[1], 1.0f);
		fColor[2] = min_<f32>(fColor[2], 1.0f);

		// Last but not least, set the color
#if ATMOSPHERE_BUG_FIX
		Vertex.Color = video::SColorf(fColor[0], fColor[1], fColor[2], fAlpha).toSColor();
#else
		Vertex.Color = video::SColorf(fColor[0], fColor[1], fColor[2]).toSColor();
#endif

#if ATMOSPHERE_BUG_FIX
		m_fInnerRadius = fInnerRadiusOrg;
		m_fOuterRadius = fOuterRadiusOrg;
		CameraPos = vCameraPosOrg;
		LightDirection = vLightDirectionOrg;
#endif

	}

}
}
