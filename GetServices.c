#include <windows.h>
#include "beacon.h"

// Manual imports, flag me threat intel.
DECLSPEC_IMPORT SC_HANDLE ADVAPI32$OpenSCManagerA(LPCSTR, LPCSTR, DWORD);
DECLSPEC_IMPORT BOOL ADVAPI32$CloseServiceHandle(SC_HANDLE);
DECLSPEC_IMPORT BOOL ADVAPI32$EnumServicesStatusExA(SC_HANDLE, SC_ENUM_TYPE, DWORD, DWORD, LPBYTE, DWORD, LPDWORD, LPDWORD, LPDWORD, LPCSTR);

// Memory allocations.
// Using heap over malloc because we have nothing but problems apparently.
DECLSPEC_IMPORT BOOL WINAPI KERNEL32$HeapFree(HANDLE, DWORD, LPVOID);
DECLSPEC_IMPORT HANDLE WINAPI KERNEL32$GetProcessHeap();
DECLSPEC_IMPORT LPVOID WINAPI KERNEL32$HeapAlloc(HANDLE, DWORD, SIZE_T);

void go() {
    // Variables initialized to 0 to keep it functional.
	DWORD pcbBytesNeeded = 0;
	DWORD lpServicesReturned = 0;
	BOOL result = FALSE; // Set this before using these or you get double statements.


	// Pointer to struct initialize to 0.
	ENUM_SERVICE_STATUS_PROCESSA* pServiceprocess = NULL;

	// Service handle to open the service manager and get granted to enumerate services.
	SC_HANDLE cuh = ADVAPI32$OpenSCManagerA(NULL, NULL, SC_MANAGER_ENUMERATE_SERVICE);
	if (cuh == NULL) { // Optimal to add a else to make sure, but it appears as blank for now since this is a KISS poc.
		BeaconPrintf(CALLBACK_ERROR, "Failed to open the service handle %lu", cuh);
	}

	// Passage one without a buffer.
	result = ADVAPI32$EnumServicesStatusExA(cuh, SC_ENUM_PROCESS_INFO, SERVICE_WIN32, SERVICE_STATE_ALL, NULL, 0, &pcbBytesNeeded, &lpServicesReturned, NULL, NULL);

	// Use heap to allocate a chunk of memory, remember to take your pointer from the above and use that.
	pServiceprocess = (ENUM_SERVICE_STATUS_PROCESSA*)KERNEL32$HeapAlloc(KERNEL32$GetProcessHeap(), HEAP_ZERO_MEMORY, pcbBytesNeeded);

    // Pass 2 with a buffer assigned.
	result = ADVAPI32$EnumServicesStatusExA(cuh, SC_ENUM_PROCESS_INFO, SERVICE_WIN32, SERVICE_STATE_ALL, (LPBYTE)pServiceprocess, pcbBytesNeeded, &pcbBytesNeeded, &lpServicesReturned, NULL, NULL);

    // Loop through all the services on the local system.
	if (result) {
		for (DWORD i = 0; i < lpServicesReturned; i++) {
			BeaconPrintf(CALLBACK_OUTPUT, "Service display name: %s\n", pServiceprocess[i].lpDisplayName);
			BeaconPrintf(CALLBACK_OUTPUT, "Service names: %s\n", pServiceprocess[i].lpServiceName);
		}
	}

	// Close handle, no handle leaks sir.
	KERNEL32$HeapFree(KERNEL32$GetProcessHeap, 0, pServiceprocess);
	ADVAPI32$CloseServiceHandle(cuh);
}
