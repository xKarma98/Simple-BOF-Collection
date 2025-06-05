#define SECURITY_WIN32
#include <windows.h>
#include "beacon.h"
#include <security.h>


DECLSPEC_IMPORT DWORD WINAPI KERNEL32$GetLastError();

// Resolve as a function pointer cause we having too many problems.
typedef BOOLEAN (WINAPI* fnGetUserNameExA)(EXTENDED_NAME_FORMAT, LPSTR, PULONG);

void go() {
	char name[256]; // Buffer for name.
	ULONG namsize = sizeof(name); // unsinged long pointer to allocate the size of the name.

	// Load library.
	HMODULE jet = LoadLibraryA("Secur32.dll");
	if (!jet) {
		BeaconPrintf(CALLBACK_ERROR, "Unable to load secur32.dll: %lu", KERNEL32$GetLastError);
		return;
	}
  // Function pointer and name, and get the address of the exported function.
	fnGetUserNameExA pGetUserNameExA = (fnGetUserNameExA)GetProcAddress(jet, "GetUserNameExA");
	if (!pGetUserNameExA) {
		BeaconPrintf(CALLBACK_OUTPUT, "procaddressfailure: %lu", KERNEL32$GetLastError);
    return;
	}

  // Get the legacy account name, buffer that recieves the name, and the size of the buffer.
	if (pGetUserNameExA(NameSamCompatible, name, &namsize)) {
		BeaconPrintf(CALLBACK_OUTPUT, "Domain & user are: %s", name);
	}
	else { // if it fails we print.
		BeaconPrintf(CALLBACK_ERROR, "Failure to get the data %lu", KERNEL32$GetLastError());
    return;
	}
}
