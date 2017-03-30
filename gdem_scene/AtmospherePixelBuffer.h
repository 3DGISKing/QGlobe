#pragma once
#include "COpenGLDriver.h"

#define ATMOSPHERE_BUG_FIX 1
#if ATMOSPHERE_BUG_FIX
#define ATMOSPHERE_SCALE (6378137.0f/10.0f)
#endif

namespace geo
{
using namespace core;
namespace scene
{

#define ALIGN_SIZE		64
#define ALIGN_MASK		(ALIGN_SIZE-1)
#define ALIGN(x)		(((u32)x+ALIGN_MASK) & ~ALIGN_MASK)
#define SWAP(a, b, t)		{ t = a; a = b; b = t; }
#define DELTA			1e-6f				// Small number for comparing floating point numbers

typedef enum
{
	UnsignedByteType = GL_UNSIGNED_BYTE,
	SignedByteType = GL_BYTE,
	UnsignedShortType = GL_UNSIGNED_SHORT,
	SignedShortType = GL_SHORT,
	UnsignedIntType = GL_UNSIGNED_INT,
	SignedIntType = GL_INT,
	FloatType = GL_FLOAT,
	DoubleType = GL_DOUBLE
} BufferDataType;

inline const s32 _GetGLDataTypeSize(const s32 nDataType)
{
	s32 nSize;
	switch(nDataType)
	{
		case UnsignedByteType:
		case SignedByteType:
			nSize = 1;
			break;
		case UnsignedShortType:
		case SignedShortType:
			nSize = 2;
			break;
		case UnsignedIntType:
		case SignedIntType:
		case FloatType:
			nSize = 4;
			break;
		case DoubleType:
			nSize = 8;
			break;
		default:
			nSize = 0;
			break;
	}
	return nSize;
}


class Base3DBuffer
{
protected:
	s32 m_nWidth;				// The width of the buffer (x axis)
	s32 m_nHeight;				// The height of the buffer (y axis)
	s32 m_nDepth;				// The depth of the buffer (z axis)
	s32 m_nDataType;			// The data type stored in the buffer (i.e. GL_UNSIGNED_BYTE, GL_FLOAT)
	s32 m_nChannels;			// The number of channels of data stored in the buffer
	s32 m_nElementSize;			// The size of one element in the buffer
	void *m_pAlloc;				// The pointer to the pixel buffer
	void *m_pBuffer;			// A byte-aligned pointer (for faster memory access)

public:
	Base3DBuffer()						{ m_pAlloc = m_pBuffer = NULL; }
	Base3DBuffer(const Base3DBuffer &buf)	{ *this = buf; }
	Base3DBuffer(const s32 nWidth, const s32 nHeight, const s32 nDepth, const s32 nDataType, const s32 nChannels=1, void *pBuffer=NULL)
	{
		m_pAlloc = m_pBuffer = NULL;
		Init(nWidth, nHeight, nDepth, nDataType, nChannels, pBuffer);
	}
	~Base3DBuffer()					{ Cleanup(); }

	void operator=(const Base3DBuffer &buf)
	{
		Init(buf.m_nWidth, buf.m_nHeight, buf.m_nDepth, buf.m_nDataType, buf.m_nChannels);
		memcpy(m_pBuffer, buf.m_pBuffer, GetBufferSize());
	}
	bool operator==(const Base3DBuffer &buf)
	{
		return (m_nWidth == buf.m_nWidth && m_nHeight == buf.m_nHeight && m_nDepth == buf.m_nDepth && m_nDataType == buf.m_nDataType && m_nChannels == buf.m_nChannels);
	}

	void *operator[](const s32 n)
	{
		return (void *)((u32)m_pBuffer + n * m_nElementSize);
	}
	void *operator()(const s32 x, const s32 y, const s32 z)
	{
		return (void *)((u32)m_pBuffer + m_nElementSize * (m_nWidth * (m_nHeight * z + y) + x));
	}

	void *operator()(const f32 x)
	{
		s32 nX = min_<s32>(m_nWidth-1, max_<s32>(0, (s32)(x*(m_nWidth-1)+0.5f)));
		return (void *)((unsigned long)m_pBuffer + m_nElementSize * nX);
	}
	void *operator()(const f32 x, const f32 y)
	{
		s32 nX = min_<s32>(m_nWidth-1, max_<s32>(0, (s32)(x*(m_nWidth-1)+0.5f)));
		s32 nY = min_<s32>(m_nHeight-1, max_<s32>(0, (s32)(y*(m_nHeight-1)+0.5f)));
		return (void *)((unsigned long)m_pBuffer + m_nElementSize * (m_nWidth * nY + nX));
	}
	void *operator()(const f32 x, const f32 y, const f32 z)
	{
		s32 nX = min_<s32>(m_nWidth-1, max_<s32>(0, (s32)(x*(m_nWidth-1)+0.5f)));
		s32 nY = min_<s32>(m_nHeight-1, max_<s32>(0, (s32)(y*(m_nHeight-1)+0.5f)));
		s32 nZ = min_<s32>(m_nDepth-1, max_<s32>(0, (s32)(z*(m_nDepth-1)+0.5f)));
		return (void *)((unsigned long)m_pBuffer + m_nElementSize * (m_nWidth * (m_nHeight * nZ + nY) + nX));
	}

	void Interpolate(f32 *p, const f32 x)
	{
		f32 fX = x*(m_nWidth-1);
		s32 nX = min_<s32>(m_nWidth-2, max_<s32>(0, (s32)fX));
		f32 fRatioX = fX - nX;
		f32 *pValue = (f32 *)((unsigned long)m_pBuffer + m_nElementSize * nX);
		for(s32 i=0; i<m_nChannels; i++)
		{
			p[i] =	pValue[0] * (1-fRatioX) + pValue[m_nChannels] * (fRatioX);
			pValue++;
		}
	}
	void Interpolate(f32 *p, const f32 x, const f32 y)
	{
		f32 fX = x*(m_nWidth-1);
		f32 fY = y*(m_nHeight-1);
		s32 nX = min_<s32>(m_nWidth-2, max_<s32>(0, (s32)fX));
		s32 nY = min_<s32>(m_nHeight-2, max_<s32>(0, (s32)fY));
		f32 fRatioX = fX - nX;
		f32 fRatioY = fY - nY;
		f32 *pValue = (f32 *)((unsigned long)m_pBuffer + m_nElementSize * (m_nWidth * nY + nX));
		for(s32 i=0; i<m_nChannels; i++)
		{
			p[i] =	pValue[0] * (1-fRatioX) * (1-fRatioY) +
					pValue[m_nChannels*1] * (fRatioX) * (1-fRatioY) +
					pValue[m_nChannels*m_nWidth] * (1-fRatioX) * (fRatioY) +
					pValue[m_nChannels*(m_nWidth+1)] * (fRatioX) * (fRatioY);
			pValue++;
		}
	}

	void Interpolate(f32 *p, const f32 x, const f32 y, const f32 z)
	{
		f32 fX = x*(m_nWidth-1);
		f32 fY = y*(m_nHeight-1);
		f32 fZ = z*(m_nDepth-1);
		s32 nX = min_<s32>(m_nWidth-2, max_<s32>(0, (s32)fX));
		s32 nY = min_<s32>(m_nHeight-2, max_<s32>(0, (s32)fY));
		s32 nZ = min_<s32>(m_nDepth-2, max_<s32>(0, (s32)fZ));
		f32 fRatioX = fX - nX;
		f32 fRatioY = fY - nY;
		f32 fRatioZ = fZ - nZ;
		f32 *pValue = (f32 *)((unsigned long)m_pBuffer + m_nElementSize * (m_nWidth * (m_nHeight * nZ + nY) + nX));
		f32 *pValue2 = (f32 *)((unsigned long)m_pBuffer + m_nElementSize * (m_nWidth * (m_nHeight * (nZ+1) + nY) + nX));
		for(s32 i=0; i<m_nChannels; i++)
		{
			p[i] =	pValue[0] * (1-fRatioX) * (1-fRatioY) * (1-fRatioZ) +
					pValue[m_nChannels*1] * (fRatioX) * (1-fRatioY) * (1-fRatioZ) +
					pValue[m_nChannels*m_nWidth] * (1-fRatioX) * (fRatioY) * (1-fRatioZ) +
					pValue[m_nChannels*(m_nWidth+1)] * (fRatioX) * (fRatioY) * (1-fRatioZ) +
					pValue2[0] * (1-fRatioX) * (1-fRatioY) * (fRatioZ) +
					pValue2[m_nChannels*1] * (fRatioX) * (1-fRatioY) * (fRatioZ) +
					pValue2[m_nChannels*m_nWidth] * (1-fRatioX) * (fRatioY) * (fRatioZ) +
					pValue2[m_nChannels*(m_nWidth+1)] * (fRatioX) * (fRatioY) * (fRatioZ);
			pValue++;
			pValue2++;
		}
	}

	void Init(const s32 nWidth, const s32 nHeight, const s32 nDepth, const s32 nDataType, const s32 nChannels=1, void *pBuffer=NULL)
	{
		// If the buffer is already initialized to the specified settings, then nothing needs to be done
		if(m_pAlloc && m_nWidth == nWidth && m_nHeight == nHeight && m_nDataType == nDataType && m_nChannels == nChannels)
			return;

		Cleanup();
		m_nWidth = nWidth;
		m_nHeight = nHeight;
		m_nDepth = nDepth;
		m_nDataType = nDataType;
		m_nChannels = nChannels;
		m_nElementSize = m_nChannels * _GetGLDataTypeSize(m_nDataType);
		if(pBuffer)
			m_pBuffer = pBuffer;
		else
		{
			m_pAlloc = new u8[GetBufferSize() + ALIGN_MASK];
			m_pBuffer = (void *)ALIGN(m_pAlloc);
		}
	}

	void Cleanup()
	{
		if(m_pAlloc)
		{
			delete m_pAlloc;
			m_pAlloc = m_pBuffer = NULL;
		}
	}

	s32 GetWidth() const 		{ return m_nWidth; }
	s32 GetHeight() const		{ return m_nHeight; }
	s32 GetDepth() const		{ return m_nDepth; }
	s32 GetDataType() const		{ return m_nDataType; }
	s32 GetChannels() const		{ return m_nChannels; }
	s32 GetBufferSize() const	{ return m_nWidth * m_nHeight * m_nDepth * m_nElementSize; }
	void *GetBuffer() const		{ return m_pBuffer; }

	void ClearBuffer()			{ memset(m_pBuffer, 0, GetBufferSize()); }
	void SwapBuffers(Base3DBuffer &buf)
	{
		void *pTemp;
		//ASSERT(*this == buf);
		SWAP(m_pAlloc, buf.m_pAlloc, pTemp);
		SWAP(m_pBuffer, buf.m_pBuffer, pTemp);
	}

	f32 LinearSample2D(s32 nChannel, f32 x, f32 y)
	{
		x = min_<f32>(max_<f32>(x, 0.0001f), 0.9999f);
		y = min_<f32>(max_<f32>(y, 0.0001f), 0.9999f);
		x *= m_nWidth;
		y *= m_nHeight;
		s32 n[2] = {(s32)x, (s32)y};
		f32 fRatio[2] = {x - n[0], y - n[1]};
		f32 *pBase = (f32 *)((u32)m_pBuffer + (m_nWidth * n[1] + n[0]) * m_nChannels * sizeof(f32));
		//if(n[0] == m_nWidth-1 || n[1] == m_nHeight-1)
			return pBase[nChannel];
		f32 *p[4] = {
			&pBase[0],
			&pBase[m_nChannels],
			&pBase[m_nWidth*m_nChannels],
			&pBase[(m_nWidth+1)*m_nChannels]
		};
		return p[0][nChannel] * (1-fRatio[0]) * (1-fRatio[1]) +
			p[1][nChannel] * (fRatio[0]) * (1-fRatio[1]) +
			p[2][nChannel] * (1-fRatio[0]) * (fRatio[1]) +
			p[3][nChannel] * (fRatio[0]) * (fRatio[1]);
	}
};

/*******************************************************************************
* Class: AtmospherePixelBuffer
********************************************************************************
* This class implements a general-purpose pixel buffer to be used for anything.
* It is often used by CTexture to set up OpenGL textures, so many of the
* parameters you use to initialize it look like the parameters you would pass
* to glTexImage1D or glTexImage2D. Some of the standard pixel buffer routines
* call fast MMX functions implemented in PixelBuffer.asm.
*******************************************************************************/
class AtmospherePixelBuffer : public Base3DBuffer
{
protected:
	s32 m_nFormat;				// The format of the pixel data (i.e. GL_LUMINANCE, GL_RGBA)

public:
	AtmospherePixelBuffer() : Base3DBuffer() {}
	AtmospherePixelBuffer(s32 nWidth, s32 nHeight, s32 nDepth, s32 nChannels=3, s32 nFormat=GL_RGB, s32 nDataType=UnsignedByteType) : Base3DBuffer(nWidth, nHeight, nDepth, nDataType, nChannels)
	{
		m_nFormat = nFormat;
	}

	s32 GetFormat()				{ return m_nFormat; }

	void Init(s32 nWidth, s32 nHeight, s32 nDepth, s32 nChannels=3, s32 nFormat=GL_RGB, s32 nDataType=GL_UNSIGNED_BYTE, void *pBuffer=NULL)
	{
		Base3DBuffer::Init(nWidth, nHeight, nDepth, nDataType, nChannels, pBuffer);
		m_nFormat = nFormat;
	}

	// Miscellaneous initalization routines
	void MakeCloudCell(f32 fExpose, f32 fSizeDisc);
	void Make3DNoise(s32 nSeed);
	void MakeGlow1D();
	void MakeOpticalDepthBuffer(f32 fInnerRadius, f32 fOuterRadius, f32 fRayleighScaleHeight, f32 fMieScaleHeight);
	void MakePhaseBuffer(f32 ESun, f32 Kr, f32 Km, f32 g);
};

}
}
