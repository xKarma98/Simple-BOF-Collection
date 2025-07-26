#include <windows.h>
#include <lm.h>
#include "beacon.h"
// Need's to be a admin context to use.

// Flag me
DECLSPEC_IMPORT NET_API_STATUS NETAPI32$NetServerDiskEnum(LMSTR, DWORD, LPBYTE*, DWORD, LPDWORD, LPDWORD, LPDWORD);

// 2114 = no smb
// error 5 permissions aren't administrative.
void go() {
	// servername will be null enumerate the local pc.
	DWORD level = 0;
	LPBYTE buf = NULL;
	DWORD prefmaxlen = MAX_PREFERRED_LENGTH;
	DWORD entriesread = 0;
	DWORD totalentries = 0;
	// NULL for resume

	NET_API_STATUS yesnowhy = NETAPI32$NetServerDiskEnum(NULL, level, (LPBYTE*)&buf, prefmaxlen, &entriesread, &totalentries, NULL);
	if (yesnowhy == NERR_Success) {
		for (DWORD i = 0; i < entriesread; i++) {
			BeaconPrintf(CALLBACK_OUTPUT, "Drives avaliable: %s\n", buf);
		}
	} else {
		BeaconPrintf(CALLBACK_ERROR, "Failed to enumerate drives: %lu", yesnowhy);
	}
}
