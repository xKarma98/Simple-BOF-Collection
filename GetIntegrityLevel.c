// Before grabbing this I recommend you to grab your user and domain first.
#include <windows.h>
#include "beacon.h"

// Threat intel here.
DECLSPEC_IMPORT BOOL WINAPI ADVAPI32$OpenProcessToken(HANDLE, DWORD, PHANDLE);

DECLSPEC_IMPORT BOOL WINAPI ADVAPI32$GetTokenInformation(HANDLE, TOKEN_INFORMATION_CLASS, LPVOID, DWORD, PDWORD);

DECLSPEC_IMPORT HANDLE WINAPI KERNEL32$GetCurrentProcess();

DECLSPEC_IMPORT DWORD WINAPI KERNEL32$GetLastError();

DECLSPEC_IMPORT BOOL WINAPI KERNEL32$CloseHandle(HANDLE);

DECLSPEC_IMPORT PDWORD WINAPI ADVAPI32$GetSidSubAuthority(PSID, DWORD);

DECLSPEC_IMPORT PUCHAR WINAPI ADVAPI32$GetSidSubAuthorityCount(PSID);

// Heap.
DECLSPEC_IMPORT HANDLE WINAPI KERNEL32$GetProcessHeap();
DECLSPEC_IMPORT LPVOID WINAPI KERNEL32$HeapAlloc(HANDLE, DWORD, SIZE_T);
DECLSPEC_IMPORT BOOL WINAPI KERNEL32$HeapFree(HANDLE, DWORD, LPVOID);

void exec() {
	// Parameter for OpenProcessToken.
	HANDLE htoken;

	// GetTokenInformation parameters.
	TOKEN_MANDATORY_LABEL* PTML = NULL;
	DWORD TokenInfoLength = 0;

	// Open the process token.
	BOOL dude = ADVAPI32$OpenProcessToken(KERNEL32$GetCurrentProcess(), TOKEN_QUERY, &htoken);
	if (dude > 0) {
		BeaconPrintf(CALLBACK_OUTPUT, "Successfully called\n");
	}
	else {
		BeaconPrintf(CALLBACK_ERROR, "Failed to open token %lu", KERNEL32$GetLastError());
	}

	// First call.
	ADVAPI32$GetTokenInformation(htoken, TokenIntegrityLevel, NULL, 0, &TokenInfoLength);

	// Allocate memory, so we can pass the function with our buffer.
	PTML = (TOKEN_MANDATORY_LABEL*)KERNEL32$HeapAlloc(KERNEL32$GetProcessHeap(), 0, TokenInfoLength);
	if (PTML == NULL) {
		BeaconPrintf(CALLBACK_ERROR, "Failure allocating memory with: %lu\n", KERNEL32$GetLastError());
		KERNEL32$CloseHandle(htoken);
		return;
	}
	else {
		BeaconPrintf(CALLBACK_OUTPUT, "Successfully allocated memory\n");
	}

  // If mando is greater then 0 print out the following.
	BOOL mandob = ADVAPI32$GetTokenInformation(htoken, TokenIntegrityLevel, PTML, TokenInfoLength, &TokenInfoLength);
		if (mandob > 0) {
			BeaconPrintf(CALLBACK_OUTPUT, "Successfully called function\n");
		}
		else {
			BeaconPrintf(CALLBACK_ERROR, "Failure to call it: %lu", KERNEL32$GetLastError());
		}

		DWORD integritylevel = *ADVAPI32$GetSidSubAuthority(PTML->Label.Sid, (*ADVAPI32$GetSidSubAuthorityCount(PTML->Label.Sid)) -1);
		if (integritylevel >= SECURITY_MANDATORY_LOW_RID && integritylevel < SECURITY_MANDATORY_MEDIUM_RID)
			BeaconPrintf(CALLBACK_OUTPUT, "Integrity Level: Low\n");
		else if (integritylevel >= SECURITY_MANDATORY_MEDIUM_RID && integritylevel < SECURITY_MANDATORY_HIGH_RID)
			BeaconPrintf(CALLBACK_OUTPUT, "Integrity Level: Medium\n");
		else if (integritylevel >= SECURITY_MANDATORY_HIGH_RID && integritylevel < SECURITY_MANDATORY_SYSTEM_RID)
			BeaconPrintf(CALLBACK_OUTPUT, "Integrity Level: High\n");
		else if (integritylevel >= SECURITY_MANDATORY_SYSTEM_RID)
			BeaconPrintf(CALLBACK_OUTPUT, "Integrity Level: System\n");
		else
			BeaconPrintf(CALLBACK_OUTPUT, "Unknown Integrity Level: %lu\n", integritylevel);

  // Close handle and free memory.
	KERNEL32$CloseHandle(htoken);
	KERNEL32$HeapFree(KERNEL32$GetProcessHeap(), 0, PTML);
}

void go() {
	exec();
}
