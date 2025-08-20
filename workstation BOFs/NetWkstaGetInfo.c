#include <windows.h>
#include <lm.h>
#include "beacon.h"

// Flag imports, flag beaconprintf.
DECLSPEC_IMPORT NET_API_STATUS WINAPI NETAPI32$NetWkstaGetInfo(LMSTR, DWORD, LPBYTE*);
DECLSPEC_IMPORT NET_API_STATUS WINAPI NETAPI32$NetApiBufferFree(LPVOID);
DECLSPEC_IMPORT int WINAPI KERNEL32$WideCharToMultiByte(UINT CodePage, DWORD dwFlags, LPCWCH lpWideCharStr, int cchWideChar, LPSTR lpMultiByteStr, int cbMultiByte, LPCCH lpDefaultChar, LPBOOL lpUsedDefaultChar);


void exec() {
	DWORD level = 102; // Set the info level.
	WKSTA_INFO_102* workinfo = NULL; // Buffer and set to NULL for good practice.
	NET_API_STATUS socksonmyproxy = NETAPI32$NetWkstaGetInfo(NULL, level, (LPBYTE*)&workinfo);

	if (socksonmyproxy == NERR_Success) { // If it's successful then we will print the following.
		char computername[256] = { 0 }; // make the 3 with a character limit and null terminated.
		char lanroot[256] = { 0 };
		char langroup[256] = { 0 };

		// All my homies hate widechar
		if (workinfo->wki102_computername)
			KERNEL32$WideCharToMultiByte(CP_ACP, 0, workinfo->wki102_computername, -1, computername, sizeof(computername), NULL, NULL);
		if (workinfo->wki102_lanroot)
			KERNEL32$WideCharToMultiByte(CP_ACP, 0, workinfo->wki102_lanroot, -1, lanroot, sizeof(lanroot), NULL, NULL);
		if (workinfo->wki102_langroup)
			KERNEL32$WideCharToMultiByte(CP_ACP, 0, workinfo->wki102_langroup, -1, langroup, sizeof(langroup), NULL, NULL);

		// Print outputs.
		BeaconPrintf(CALLBACK_OUTPUT, "Platform ID %lu\n", workinfo->wki102_platform_id);
		BeaconPrintf(CALLBACK_OUTPUT, "Computer name %s\n", computername);
		BeaconPrintf(CALLBACK_OUTPUT, "Lan group is %s\n", langroup);
		BeaconPrintf(CALLBACK_OUTPUT, "major version %lu\n", workinfo->wki102_ver_major);
		BeaconPrintf(CALLBACK_OUTPUT, "Minor version of the computer %lu\n", workinfo->wki102_ver_minor);
		BeaconPrintf(CALLBACK_OUTPUT, "Lan root is %s\n", lanroot);
		BeaconPrintf(CALLBACK_OUTPUT, "Logged on users %lu\n", workinfo->wki102_logged_on_users);

	} else { // Failure? Print error code and return.
		BeaconPrintf(CALLBACK_ERROR, "Failure with code %lu", socksonmyproxy);
		return;
	}
	NETAPI32$NetApiBufferFree(workinfo); // Free memory.
}

void go() {
	exec();
}
