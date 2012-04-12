#pragma once

#include <Windows.h>

inline void msec_sleep(unsigned int msec)
{
#if defined(WIN32)
	Sleep(msec);
#else
	// use nanosleep
#endif
}
