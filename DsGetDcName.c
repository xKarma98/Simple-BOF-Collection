#include "beacon.h"
#include <windows.h>
#include <dsgetdc.h>

// https://learn.microsoft.com/en-us/windows/win32/api/dsgetdc/nf-dsgetdc-dsgetdcnamea
// Before blindly copying check out elastic protections artifact placed on this specific BOF.
DECLSPEC_IMPORT DWORD WINAPI NETAPI32$DsGetDcNameA(LPCSTR ComputerName, LPCSTR DomainName, GUID* DomainGuid, LPCSTR  SiteName, ULONG Flags, LPVOID DomainCotrolerInfo);
DECLSPEC_IMPORT DWORD WINAPI NETAPI32$NetApiBufferFree(LPVOID);


void exec() {
	DOMAIN_CONTROLLER_INFOA* DomainControllerInfo = NULL; // Pointer to this struct.
	DWORD mando = NETAPI32$DsGetDcNameA(NULL, NULL, NULL, NULL, 0, &DomainControllerInfo);
	if (mando == ERROR_SUCCESS) {
    // We make use of our pointer to point to this struct and call this api.
		BeaconPrintf(CALLBACK_OUTPUT, "Domain controller name: %s", DomainControllerInfo->DomainName);
	}
	else { // Error handling improvement over last time :3
		BeaconPrintf(CALLBACK_ERROR, "Error getting info code: %lu", mando);
	}
  // Free the buffer.
	NETAPI32$NetApiBufferFree(DomainControllerInfo);
}

void go() {
	exec();
}
