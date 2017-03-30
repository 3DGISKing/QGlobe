// Copyright (C) 2002-2008 Nikolaus Gebhardt
// This file is part of the "Geo Engine".
// For conditions of distribution and use, see copyright notice in geoEngine.h

#include "geoCompileConfig.h"

static const char* const copyright = "Geo Engine (c) 2002-2008 Nikolaus Gebhardt";

#ifdef _GEO_WINDOWS_
	#include <windows.h>
	#if defined(_DEBUG) && !defined(__GNUWIN32__) && !defined(_WIN32_WCE)
		#include <crtdbg.h>
	#endif // _DEBUG
#endif

#ifdef _GEO_XBOX_PLATFORM_
	#include <xtl.h>
#endif

#include "geoEngine.h"

namespace geo
{
	//! stub for calling createDeviceEx
	GEOENGINE_API GeoDevice* GEOCALLCONV createDevice(video::E_DRIVER_TYPE driverType,
			const core::dimension2d<s32>& windowSize,
			u32 bits, bool fullscreen,
			bool stencilbuffer, bool vsync, IEventReceiver* res)
	{
		SGeoCreationParameters p;
		p.DriverType = driverType;
		p.WindowSize = windowSize;
		p.Bits = (u8)bits;
		p.Fullscreen = fullscreen;
		p.Stencilbuffer = stencilbuffer;
		p.Vsync = vsync;
		p.EventReceiver = res;

		return createDeviceEx(p);
	}

namespace core
{
	const matrix4 IdentityMatrix(matrix4::EM4CONST_IDENTITY);
}
} // end namespace geo


#if defined(_GEO_WINDOWS_API_)

BOOL APIENTRY DllMain( HANDLE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved )
{
	// _crtBreakAlloc = 139;

    switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
			#if defined(_DEBUG) && !defined(__GNUWIN32__) && !defined(__BORLANDC__) && !defined (_WIN32_WCE)
				_CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_ALLOC_MEM_DF);
			#endif
			break;
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
    }
    return TRUE;
}

#endif // defined(_GEO_WINDOWS_)

