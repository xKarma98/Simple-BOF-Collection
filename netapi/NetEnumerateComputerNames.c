#include <windows.h>
#include <lm.h>
#include "beacon.h"

// Need's admin permissions and workstation service to be enabled.
DECLSPEC_IMPORT NET_API_STATUS NETAPI32$NetEnumerateComputerNames(LPCWSTR, NET_COMPUTER_NAME_TYPE, ULONG, PDWORD, LPSTR*);

void go() {
	// NULL
	// NetAllComputerNames
	ULONG Reserved = 0; // Had to or compiler cry.
	DWORD entrycount = 0; // Loop me.
	LPSTR* Computer = NULL; // Initialize this pointer to null and we changed it to this to point ot a string and not deal with wide char aka wchar_t.
	NET_API_STATUS com = NETAPI32$NetEnumerateComputerNames(NULL, NetAllComputerNames, Reserved, &entrycount, (LPSTR*)&Computer);
	if (com == NERR_Success) {
		for (DWORD i = 0; i < entrycount; i++) {
			BeaconPrintf(CALLBACK_OUTPUT, "Computers: %s\n", Computer);
		}
	}
	else {
		BeaconPrintf(CALLBACK_ERROR, "Failed to gather computernames: %lu", com);
	}
}
