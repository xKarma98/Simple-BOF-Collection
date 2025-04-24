#include "beacon.h" // For cobalt strike data/printing.
#include <windows.h> // Windows api include.
#include <lm.h> // Required for NET_API

// Manually importing these.
DECLSPEC_IMPORT DWORD WINAPI NETAPI32$NetApiBufferFree(LPVOID buf);

DECLSPEC_IMPORT NET_API_STATUS WINAPI NETAPI32$NetShareEnum(LMSTR servername, DWORD level, LPBYTE* bufptr,DWORD prefmaxlen, LPDWORD entriesread, LPDWORD totalentries, LPDWORD resume_handle);

void myfunction() {
  // Setting values, we don't need it to be 0 but I personally feel better having it like that.
	DWORD entriesread = 0;
	DWORD totalentries = 0;
	DWORD resume_handle = 0;
	LPBYTE buf = NULL; // Null pointer.

  // Defining our function.
	NET_API_STATUS cuh = NETAPI32$NetShareEnum(NULL, 2, &buf, MAX_PREFERRED_LENGTH, &entriesread, &totalentries, &resume_handle);
	if (cuh == NERR_Success) {
		SHARE_INFO_2* shareinfo = (SHARE_INFO_2*)buf; // Making usage of this struct in order to use a member.
		for (DWORD i = 0; i < entriesread; ++i) { // We loop through entries.
      // Moment of truth we print our output.
			BeaconPrintf(CALLBACK_ERROR, "Shares are: %s\n", shareinfo[i].shi2_netname);
		}
	}
	else {
		BeaconPrintf(CALLBACK_ERROR, "Likely no shares on this host error code: %lu", cuh);
	}
  // Clear out the memory.
	NETAPI32$NetApiBufferFree(buf);
}


void go() {
	myfunction();
}
