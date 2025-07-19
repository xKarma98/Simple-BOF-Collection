#include <windows.h>
#include "beacon.h"

DECLSPEC_IMPORT DWORD KERNEL32$GetCurrentProcessId();

void go() {
	DWORD c = KERNEL32$GetCurrentProcessId();
	BeaconPrintf(CALLBACK_OUTPUT, "Current Pid %d", c);
}
