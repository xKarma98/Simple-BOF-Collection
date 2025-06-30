#include "beacon.h"
#include <windows.h>

// Flag me pls.
DECLSPEC_IMPORT void WINAPI KERNEL32$GetSystemTime(LPSYSTEMTIME);
DECLSPEC_IMPORT void WINAPI KERNEL32$GetLocalTime(LPSYSTEMTIME);

void go() {
    // Pointers.
	SYSTEMTIME  systime, locsystemtime;
	// defining sys and loc time with our pointer references.
	KERNEL32$GetSystemTime(&systime);
	KERNEL32$GetLocalTime(&locsystemtime);

    // Print our local time & using our pointer with member access operators to get our hour and minute.
	BeaconPrintf(CALLBACK_OUTPUT, "Local time is: %02d:%02d\n", locsystemtime.wHour, locsystemtime.wMinute);
	BeaconPrintf(CALLBACK_OUTPUT, "system time is: %02d:%02d\n", systime.wHour, systime.wMinute);
}
