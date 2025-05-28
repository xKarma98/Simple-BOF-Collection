#include "beacon.h" // Beacon API's.
#include <windows.h> // Windows API include.

// Manual imports, write rules here.
DECLSPEC_IMPORT LPCH WINAPI KERNEL32$GetEnvironmentStrings();
DECLSPEC_IMPORT BOOL WINAPI KERNEL32$FreeEnvironmentStringsA(LPCH);
DECLSPEC_IMPORT int WINAPI KERNEL32$lstrlenA(LPCSTR);

void exec() {
// Point to this function.
	LPTCH lpEnv = KERNEL32$GetEnvironmentStrings();
	LPTSTR lpszVariable; // LPCSTR is a pointer to a const string

	if (lpEnv == NULL) { // MSDN doc's if it's null it failed.
		BeaconPrintf(CALLBACK_ERROR, "NULL pointer for env strings: %lu", lpEnv);
		return; // Return to close it.
	}


    // Type cast to lpEnv.
	lpszVariable = (LPTSTR)lpEnv;

	while (*lpszVariable) { // While loop to printf out environment variables.
		BeaconPrintf(CALLBACK_OUTPUT, "%s\n", lpszVariable);
		// Add entries to the output, by incrementing by 1s.
		lpszVariable += KERNEL32$lstrlenA(lpszVariable) + 1;
	}
	KERNEL32$FreeEnvironmentStringsA(lpEnv); // Free the environmentstrings.
	return; // Return at the end.
}


void go() { // Coff entrypoint.
	exec(); // Function call.
}
