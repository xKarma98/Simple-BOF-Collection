#include <windows.h>
#include <lm.h>
#include "beacon.h"


DECLSPEC_IMPORT NET_API_STATUS NETAPI32$NetUserChangePassword(LPCSTR, LPCSTR, LPCSTR, LPCSTR);

// Hardcoded refer to MSDN if you want one that takes input.
void go() {
	// NULL for current domain.
	LPCSTR username = "Broman";
	LPCSTR password = "Keys45";
	LPCSTR newpass = "Johncena";
	NET_API_STATUS fu = NETAPI32$NetUserChangePassword(NULL, username, password, newpass);
	if (fu == NERR_Success) {
		BeaconPrintf(CALLBACK_OUTPUT, "Successfully changed the password\n");
	}
	else {
		BeaconPrintf(CALLBACK_ERROR, "Failed to change it with code: %lu", fu);
	}
}
