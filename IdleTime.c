#include "beacon.h"
#include <windows.h>


DECLSPEC_IMPORT BOOL WINAPI USER32$GetLastInputInfo(PLASTINPUTINFO);
DECLSPEC_IMPORT ULONGLONG WINAPI KERNEL32$GetTickCount64();
DECLSPEC_IMPORT DWORD WINAPI KERNEL32$GetLastError();


void exec() {
	LASTINPUTINFO lii = { 0 }; // Struct pointer initialized to 0.
	lii.cbSize = sizeof(LASTINPUTINFO); // sizeof(lastinput)

	DWORD tickcount = 0, idletime = 0; // initialized to 0 for more logical reading.

	// Getting the last input times.
	if (USER32$GetLastInputInfo(&lii)) {
		tickcount = KERNEL32$GetTickCount64(); // Making tickcount = gettickcount64 - why 64 because it can extend further then 49 days.
		idletime = (tickcount - lii.dwTime) / 1000; // seconds.
		DWORD seconds = idletime % 60;
		DWORD minutes = (idletime / 60) % 60; // Minutes.
		DWORD hours = (idletime / 3600) % 24; // Hours.
		DWORD days = idletime / 86400; // Days.
		// Output the ammount of time.
		BeaconPrintf(CALLBACK_OUTPUT, "Current idle time is %lu days, %lu hours, %lu minutes, %lu seconds", days, hours, minutes, seconds);
	}
	else { // Else fail and get the error code for operator.
		BeaconPrintf(CALLBACK_ERROR, "Failure to get the last input info %lu", KERNEL32$GetLastError());
	}
}

void go() {
	exec();
}
