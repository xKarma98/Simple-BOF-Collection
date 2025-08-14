#include <windows.h>
#include <lm.h>
#include "beacon.h"

// Flag me and beaconprintf.
DECLSPEC_IMPORT NET_API_STATUS NETAPI32$NetRemoteComputerSupports(LPCWSTR, DWORD, LPDWORD);

void go() {
  // Set these values in dword, or just put them in the parameter.
	DWORD OptionWanted = SUPPORTS_REMOTE_ADMIN_PROTOCOL;
	DWORD OptionsSupported = 0; // Holder of the value.
	NET_API_STATUS ye = NETAPI32$NetRemoteComputerSupports(NULL, OptionWanted, &OptionsSupported);

  // If success print or fail and return a error code.
	if (ye == NERR_Success) {
		BeaconPrintf(CALLBACK_OUTPUT, "Remote admin protocol supported\n", ye);
	}
	else {
		BeaconPrintf(CALLBACK_ERROR, "This host doesn't support remote admin: %lu", ye);
	}
}
