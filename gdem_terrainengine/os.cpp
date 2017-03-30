// Copyright (C) 2002-2008 Nikolaus Gebhardt
// This file is part of the "Geo Engine".
// For conditions of distribution and use, see copyright notice in geoEngine.h

#include "os.h"
#include "geoString.h"
#include "geoCompileConfig.h"
#include "geoMath.h"

#if defined(_GEO_USE_SDL_DEVICE_)
	#include <SDL/SDL_endian.h>
	#define bswap_16(X) SDL_Swap16(X)
	#define bswap_32(X) SDL_Swap32(X)
#elif defined(_GEO_WINDOWS_API_)
	#if (defined(_MSC_VER) && (_MSC_VER > 1298))
		#include <stdlib.h>
		#define bswap_16(X) _byteswap_ushort(X)
		#define bswap_32(X) _byteswap_ulong(X)
	#else
		#define bswap_16(X) ((((X)&0xFF) << 8) | (((X)&=0xFF00) >> 8))
		#define bswap_32(X) ( (((X)&0x000000FF)<<24) | (((X)&0xFF000000) >> 24) | (((X)&0x0000FF00) << 8) | (((X) &0x00FF0000) >> 8))
	#endif
#else
	#if defined(_GEO_OSX_PLATFORM_)
		#include <libkern/OSByteOrder.h>
		#define bswap_16(X) OSReadSwapInt16(&X,0)
		#define bswap_32(X) OSReadSwapInt32(&X,0)
	#elif defined(__FreeBSD__)
		#include <sys/endian.h>
		#define bswap_16(X) bswap16(X)
		#define bswap_32(X) bswap32(X)
	#elif !defined(_GEO_SOLARIS_PLATFORM_) && !defined(__PPC__)
		#include <byteswap.h>
	#else
		#define bswap_16(X) ((((X)&0xFF) << 8) | (((X)&=0xFF00) >> 8))
		#define bswap_32(X) ( (((X)&0x000000FF)<<24) | (((X)&0xFF000000) >> 24) | (((X)&0x0000FF00) << 8) | (((X) &0x00FF0000) >> 8))
	#endif
#endif

namespace geo
{
namespace os
{
	u16 Byteswap::byteswap(u16 num) {return bswap_16(num);}
	s16 Byteswap::byteswap(s16 num) {return bswap_16(num);}
	u32 Byteswap::byteswap(u32 num) {return bswap_32(num);}
	s32 Byteswap::byteswap(s32 num) {return bswap_32(num);}
	f32 Byteswap::byteswap(f32 num) {u32 tmp=bswap_32(*((u32*)&num)); return *((f32*)&tmp);}
}
}

#if defined(_GEO_WINDOWS_API_)
// ----------------------------------------------------------------
// Windows specific functions
// ----------------------------------------------------------------

#ifdef _GEO_XBOX_PLATFORM_
#include <xtl.h>
#else
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

namespace geo
{
namespace os
{
	//! prints a debuginfo string
	void Printer::print(const c8* message)
	{
#if !defined (_WIN32_WCE )
		c8* tmp = new c8[strlen(message) + 2];
		sprintf(tmp, "%s\n", message);
		//kimyi
		//OutputDebugString(tmp);
		printf(tmp);
		delete [] tmp;
#endif
	}

	static LARGE_INTEGER HighPerformanceFreq;
	static BOOL HighPerformanceTimerSupport = FALSE;
	static BOOL MultiCore = FALSE;

	void Timer::initTimer()
	{
#if !defined(_WIN32_WCE)
		// disable hires timer on multiple core systems, bios bugs result in bad hires timers.
		SYSTEM_INFO sysinfo;
		GetSystemInfo(&sysinfo);
		MultiCore = (sysinfo.dwNumberOfProcessors > 1);	
#endif
		HighPerformanceTimerSupport = QueryPerformanceFrequency(&HighPerformanceFreq);
		initVirtualTimer();
	}

	u32 Timer::getRealTime()
	{
		if (HighPerformanceTimerSupport)
		{
#if !defined(_WIN32_WCE)
			// Avoid potential timing inaccuracies across multiple cores by 
			// temporarily setting the affinity of this process to one core.
			DWORD_PTR affinityMask;
			if(MultiCore)
				affinityMask = SetThreadAffinityMask(GetCurrentThread(), 1); 
#endif
			LARGE_INTEGER nTime;
			BOOL queriedOK = QueryPerformanceCounter(&nTime);

#if !defined(_WIN32_WCE)
			// Restore the true affinity.
			if(MultiCore)
				(void)SetThreadAffinityMask(GetCurrentThread(), affinityMask);
#endif
			if(queriedOK)
				return u32((nTime.QuadPart) * 1000 / HighPerformanceFreq.QuadPart);

		}

		return GetTickCount();
	}

} // end namespace os


#else

// ----------------------------------------------------------------
// linux/ansi version
// ----------------------------------------------------------------

#include <stdio.h>
#include <time.h>
#include <sys/time.h>

namespace geo
{
namespace os
{

	//! prints a debuginfo string
	void Printer::print(const c8* message)
	{
#ifdef DEBUG
		printf("%s\n", message);
#endif
	}

	void Timer::initTimer()
	{
		initVirtualTimer();
	}

	u32 Timer::getRealTime()
	{
		timeval tv;
		gettimeofday(&tv, 0);
		return (u32)(tv.tv_sec * 1000) + (tv.tv_usec / 1000);
	}

} // end namespace os

#endif // end linux / windows

namespace os
{
	// The platform independent implementation of the printer
	ILogger* Printer::Logger = 0;

	void Printer::log(const c8* message, ELOG_LEVEL ll)
	{
		if (Logger)
			Logger->log(message, ll);
	}

	void Printer::log(const wchar_t* message, ELOG_LEVEL ll)
	{
		if (Logger)
			Logger->log(message, ll);
	}

	void Printer::log(const c8* message, const c8* hint, ELOG_LEVEL ll)
	{
		if (Logger)
			Logger->log(message, hint, ll);
	}

	void Printer::log(const c8* message, const io::path& hint, ELOG_LEVEL ll)
	{
		if (Logger)
			Logger->log(message, hint.c_str(), ll);
	}


	// our Randomizer is not really os specific, so we
	// code one for all, which should work on every platform the same,
	// which is desireable.

	s32 Randomizer::seed = 0x0f0f0f0f;

	//! generates a pseudo random number
	s32 Randomizer::rand()
	{
		const s32 m = 2147483399;	// a non-Mersenne prime
		const s32 a = 40692;		// another spectral success story
		const s32 q = m/a;
		const s32 r = m%a;		// again less than q

		seed = a * (seed%q) - r* (seed/q);
		if (seed<0) seed += m;

		return seed;
	}

	//! resets the randomizer
	void Randomizer::reset()
	{
		seed = 0x0f0f0f0f;
	}


	// ------------------------------------------------------
	// virtual timer implementation

	f32 Timer::VirtualTimerSpeed = 1.0f;
	s32 Timer::VirtualTimerStopCounter = 0;
	u32 Timer::LastVirtualTime = 0;
	u32 Timer::StartRealTime = 0;
	u32 Timer::StaticTime = 0;

	//! returns current virtual time
	u32 Timer::getTime()
	{
		if (isStopped())
			return LastVirtualTime;

		return LastVirtualTime + (u32)((StaticTime - StartRealTime) * VirtualTimerSpeed);
	}

	//! ticks, advances the virtual timer
	void Timer::tick()
	{
		StaticTime = getRealTime();
	}

	//! sets the current virtual time
	void Timer::setTime(u32 time)
	{
		StaticTime = getRealTime();
		LastVirtualTime = time;
		StartRealTime = StaticTime;
	}

	//! stops the virtual timer
	void Timer::stopTimer()
	{
		if (!isStopped())
		{
			// stop the virtual timer
			LastVirtualTime = getTime();
		}

		--VirtualTimerStopCounter;
	}

	//! starts the virtual timer
	void Timer::startTimer()
	{
		++VirtualTimerStopCounter;

		if (!isStopped())
		{
			// restart virtual timer
			setTime(LastVirtualTime);
		}
	}

	//! sets the speed of the virtual timer
	void Timer::setSpeed(f32 speed)
	{
		setTime(getTime());

		VirtualTimerSpeed = speed;
		if (VirtualTimerSpeed < 0.0f)
			VirtualTimerSpeed = 0.0f;
	}

	//! gets the speed of the virtual timer
	f32 Timer::getSpeed()
	{
		return VirtualTimerSpeed;
	}

	//! returns if the timer currently is stopped
	bool Timer::isStopped()
	{
		return VirtualTimerStopCounter != 0;
	}

	void Timer::initVirtualTimer()
	{
		StaticTime = getRealTime();
		StartRealTime = StaticTime;
	}

} // end namespace os
} // end namespace geo


