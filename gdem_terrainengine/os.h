// Copyright (C) 2002-2008 Nikolaus Gebhardt
// This file is part of the "Geo Engine".
// For conditions of distribution and use, see copyright notice in geoEngine.h

#ifndef __GEO_OS_H_INCLUDED__
#define __GEO_OS_H_INCLUDED__

#include "geoCompileConfig.h" // for endian check
#include "geoTypes.h"
#include "path.h"
#include "ILogger.h"
#include "gdem_terrainengine_global.h"

namespace geo
{

namespace os
{
	class Byteswap
	{
	public:
		static u16 byteswap(u16 num);
		static s16 byteswap(s16 num);
		static u32 byteswap(u32 num);
		static s32 byteswap(s32 num);
		static f32 byteswap(f32 num);
	};

	class Printer
	{
	public:
		// prints out a string to the console out stdout or debug log or whatever
		static void print(const c8* message);
		static void log(const c8* message, ELOG_LEVEL ll = ELL_INFORMATION);
		static void log(const wchar_t* message, ELOG_LEVEL ll = ELL_INFORMATION);
		static void log(const c8* message, const c8* hint, ELOG_LEVEL ll = ELL_INFORMATION);
		static void log(const c8* message, const io::path& hint, ELOG_LEVEL ll = ELL_INFORMATION);
		static ILogger* Logger;
	};


	class Randomizer
	{
	public:

		//! resets the randomizer
		static void reset();

		//! generates a pseudo random number
		static s32 rand();

	private:

		static s32 seed;
	};




	class GDEM_TERRAINENGINE_EXPORT Timer
	{
	public:

		//! returns the current time in milliseconds
		static u32 getTime();

		//! initializes the real timer
		static void initTimer();

		//! sets the current virtual (game) time
		static void setTime(u32 time);

		//! stops the virtual (game) timer
		static void stopTimer();

		//! starts the game timer
		static void startTimer();

		//! sets the speed of the virtual timer
		static void setSpeed(f32 speed);

		//! gets the speed of the virtual timer
		static f32 getSpeed();

		//! returns if the timer currently is stopped
		static bool isStopped();

		//! makes the virtual timer update the time value based on the real time
		static void tick();

		//! returns the current real time in milliseconds
		static u32 getRealTime();

	private:

		static void initVirtualTimer();

		static f32 VirtualTimerSpeed;
		static s32 VirtualTimerStopCounter;
		static u32 StartRealTime;
		static u32 LastVirtualTime;
		static u32 StaticTime;
	};

} // end namespace os
} // end namespace geo


#endif

