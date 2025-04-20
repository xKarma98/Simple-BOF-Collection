#include "beacon.h" // Visual studio BOF include.
#include <windows.h> // Windows api include.

DECLSPEC_IMPORT HANDLE WINAPI KERNEL32$CreateFileA(LPCSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile); 

DECLSPEC_IMPORT DWORD WINAPI KERNEL32$GetLastError(void);

// Good practice to remember this nonsense.
/*
HANDLE CreateFileA(
  [in]           LPCSTR                lpFileName,
  [in]           DWORD                 dwDesiredAccess,
  [in]           DWORD                 dwShareMode,
  [in, optional] LPSECURITY_ATTRIBUTES lpSecurityAttributes,
  [in]           DWORD                 dwCreationDisposition,
  [in]           DWORD                 dwFlagsAndAttributes,
  [in, optional] HANDLE                hTemplateFile
);
*/

void go(char* args, int alen) {
	HANDLE NOTAIOCMAN = KERNEL32$CreateFileA(
	    // If you try to write this to C:\ you will get code 5 permission denied.
		"C:\\Users\\username\\onedrive\\desktop\\lol.txt", // file name
		GENERIC_READ, // Desired access.
		FILE_SHARE_READ, // Share mode.
		NULL, // sec attributes.
		CREATE_NEW, // Create new file
		FILE_ATTRIBUTE_NORMAL, // File attributes.
		NULL // The template file.
		);
	// Check if valid or not.
	if (NOTAIOCMAN == INVALID_HANDLE_VALUE) {
		DWORD dwError = KERNEL32$GetLastError();
		BeaconPrintf(CALLBACK_ERROR, "Not a valid handle: %ld", dwError);
	}
	else if (NOTAIOCMAN == NULL) {
		BeaconPrintf(CALLBACK_OUTPUT, "Valid handle successfully created");
	}
}
