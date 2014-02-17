#pragma once

#ifdef _WIN32
#include <Windows.h>
#endif

#ifdef linux
#include <sys/time.h>
#endif

long GetElapsedMS(void)
{
	long elapsedTime = 0;
#ifdef _WIN32
	elapsedTime = GetTickCount64();
#endif

#ifdef linux
	struct timeval capturedTime;
	gettimeofday(&captureTime, NULL);
	elapsedTime = captureTime.tv_sec * 1000000 + captureTime.tv_usec;
	elapsedTime /= 1000;
#endif
	return elapsedTime;
}