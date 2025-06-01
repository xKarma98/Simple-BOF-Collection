// I was statically testing this so I have hardcoded paths this is just a sample from the testing phases, you will have to change variables in here and these appear as strings so more IOC's.
#include <windows.h>
#include "Beacon.h"

// BLue team hi.
DECLSPEC_IMPORT LSTATUS WINAPI ADVAPI32$RegCloseKey(HKEY);
DECLSPEC_IMPORT LSTATUS WINAPI ADVAPI32$RegQueryValueExA(HKEY, LPCSTR, LPDWORD, LPDWORD, LPBYTE, LPDWORD);
DECLSPEC_IMPORT LSTATUS WINAPI ADVAPI32$RegOpenKeyExA(HKEY, LPCSTR, DWORD, REGSAM, PHKEY);

void exec() {
	LPCSTR subkey = "Printers\\ConvertUserDevModesCount"; // Subkey location.
	LPCSTR valueName = "OneNote (Desktop)"; // Value to query.
	HKEY nsakey = HKEY_CURRENT_USER; // Pass this value to the key.
  // Pass key + subkey skip options, Desired access is to read the key, &nsakey pointer to variable that recieves the handle.
	LSTATUS blaster = ADVAPI32$RegOpenKeyExA(nsakey, subkey, 0, KEY_READ, &nsakey);

	if (blaster == ERROR_SUCCESS) { // Check if enabled.
		BeaconPrintf(CALLBACK_OUTPUT, "We have enabled a key\n");
	}
	else {
		BeaconPrintf(CALLBACK_ERROR, "Failure to get a key: %lu\n", blaster);
		return;
	}

  // Initialize to 0.
	DWORD valtype = 0;
	DWORD data; // Data.
	DWORD datsize = sizeof(data); // Sizeofdata.

  // Query value pass the key -> pass the valueName, reservedbytes is 0, pass the size of valtpe & data as a LPBYTE, & the size of data.
	LSTATUS oscar = ADVAPI32$RegQueryValueExA(nsakey, valueName, NULL, &valtype, (LPBYTE)&data, &datsize);
	if (oscar == ERROR_SUCCESS) { // If successfully done.
		if (valtype != REG_DWORD) { // If it's not a DWORD.
			BeaconPrintf(CALLBACK_ERROR, "Value is not a dword\n");
			return;
		}
		else if (data == 1) { // If it returns 1 enabled.
			BeaconPrintf(CALLBACK_OUTPUT, "OneNote is enabled in registry\n");
			return;
		}
		else if (data == 0) { // If 0 not enabled.
			BeaconPrintf(CALLBACK_OUTPUT, "OneNote is not enabled or doesn't exist %lu\n", oscar);
			return;
		}
	}
	ADVAPI32$RegCloseKey(nsakey); // Close the key or we leave it dangling around.
}

void go() { // Entrypoint for coff to execute exec();
	exec();
}
