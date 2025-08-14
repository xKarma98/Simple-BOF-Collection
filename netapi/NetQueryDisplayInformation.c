#include <windows.h>
#include <lm.h>
#include "beacon.h"

// Flag em, also flag BeaconPrintf if your a blue boy/woman/them
DECLSPEC_IMPORT NET_API_STATUS NETAPI32$NetQueryDisplayInformation(LPCWSTR, DWORD, DWORD, DWORD, DWORD, LPDWORD, PVOID);
DECLSPEC_IMPORT NET_API_STATUS NETAPI32$NetApiBufferFree(LPVOID);
DECLSPEC_IMPORT int WINAPI KERNEL32$WideCharToMultiByte(UINT CodePage, DWORD dwFlags, LPCWCH lpWideCharStr, int cchWideChar, LPSTR lpMultiByteStr, int cbMultiByte, LPCCH lpDefaultChar, LPBOOL lpUsedDefaultChar);

void hi() {
  // Function parameters.
	DWORD level = 1;
	DWORD index = 0;
	DWORD entriesRequested = 100;
	DWORD prefmaxlen = MAX_PREFERRED_LENGTH;
	DWORD ReturnedEntryCount = 0;
	NET_DISPLAY_USER* dispusers = NULL;

  // Function and it's parameters passed.
	NET_API_STATUS bruhman = NETAPI32$NetQueryDisplayInformation(NULL, level, index, entriesRequested, prefmaxlen, &ReturnedEntryCount, (LPVOID*)&dispusers);
	if (bruhman == NERR_Success || bruhman == ERROR_MORE_DATA) { // If successful proceed.
    // For loop, converting wide char to ansi and printing basic information out.
		for (DWORD i = 0; i < ReturnedEntryCount; i++) {
			char name[256] = { 0 };
			char fullname[256] = { 0 };
			char comment[256] = { 0 };

			if (dispusers[i].usri1_name)
				KERNEL32$WideCharToMultiByte(CP_ACP, 0, dispusers[i].usri1_name, -1, name, sizeof(name), NULL, NULL);
			if (dispusers[i].usri1_full_name)
				KERNEL32$WideCharToMultiByte(CP_ACP, 0, dispusers[i].usri1_full_name, -1, fullname, sizeof(fullname), NULL, NULL);
			if (dispusers[i].usri1_comment)
				KERNEL32$WideCharToMultiByte(CP_ACP, 0, dispusers[i].usri1_comment, -1, comment, sizeof(comment), NULL, NULL);

			BeaconPrintf(CALLBACK_OUTPUT, "Name %s full name %s comment: %s\n", name, fullname, comment);
		}
	}
	else { // If the above fails we spit out a error code, if you do not understand this "net helpmsg errorcode" -> put the code outputted in place of error code.
		BeaconPrintf(CALLBACK_ERROR, "Error code %lu", bruhman);
	}
	NETAPI32$NetApiBufferFree(dispusers); // Free memory or leak.
}

// Coffloader function entrypoint.
void go() {
	hi(); // Function call to hi();
}
