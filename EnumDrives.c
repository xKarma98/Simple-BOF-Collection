#include "beacon.h" // Include for cobalt strike apis, BeaconPrintf
#include <windows.h> // Windows api include.
#include <fileapi.h> // Include for GetLogicalDriveStringsA

// Manual import or we will have symbol errors.
DECLSPEC_IMPORT DWORD WINAPI KERNEL32$GetLogicalDriveStringsA(DWORD nbufferlength, LPSTR buffer);


void enumdrives() {
	DWORD nbuflen = 512; // Buffer length how many chars can it hold.
	char buf[512]; // Buffer size.
	// We use this for the strings.
	// GetLogicalDriveStringsA will print this code in utf-16 windows ansi.
	DWORD mandobama = KERNEL32$GetLogicalDriveStringsA(nbuflen, buf);

	if (mandobama == 0) { // If this is 0 it will error out refer to documentation.
		BeaconPrintf(CALLBACK_ERROR, "Failed to get drives: %lu", mandobama);
	}

	char* drive = buf; // Char pointer to our buffer.
	while (*drive) { // While loop.
		BeaconPrintf(CALLBACK_OUTPUT, "Drives on the computer %s", drive); // Print the drives on the computer as a string.
		drive += strlen(drive) + 1; // Needed to add new entries.
	}
}

void go() { // Entrypoint for coffloader64 to execute.
	enumdrives(); // Function call.
}
