#include "beacon.h"
#include <windows.h>
#include <lm.h>

#pragma comment(lib, "Netapi32.lib") // Solves the linker 2019,1120 issue.

// Manual imports, Hi blue team write rules here for the _imp's.
DECLSPEC_IMPORT NET_API_STATUS WINAPI NETAPI32$NetWkstaUserEnum(LMSTR servername, DWORD level, LPBYTE *buf, DWORD prefmaxlen, LPDWORD entriesread, LPDWORD totalentries, LPDWORD resume_handle);
DECLSPEC_IMPORT NET_API_STATUS WINAPI NETAPI32$NetApiBufferFree(LPVOID buffer);

// Target me blue team make a rule on me.
DECLSPEC_IMPORT int WINAPI KERNEL32$WideCharToMultiByte(UINT CodePage, DWORD dwFlags, LPCWCH lpWideCharStr, int cchWideChar, LPSTR lpMultiByteStr, int cbMultiByte, LPCCH lpDefaultChar, LPBOOL lpUsedDefaultChar);

// Void func entry.
void getloggedonusers() {
    // Parameters.
	LPBYTE buf = NULL;
	LMSTR servername = NULL; // Bloat.
	DWORD resumehandle = 0;
	DWORD entriesread = 0;
	DWORD totalentries = 0;

	NET_API_STATUS mandobama = NETAPI32$NetWkstaUserEnum(NULL, 1, &buf, MAX_PREFERRED_LENGTH, &entriesread, &totalentries, &resumehandle);
	if (mandobama == NERR_Success) {
		WKSTA_USER_INFO_1* exterminate = (WKSTA_USER_INFO_1*)buf;
		for (DWORD i = 0; i < entriesread; ++i) {
			char username[256] = { 0 };
			char domain[256] = { 0 };
			char othdomains[256] = { 0 };
			char logonserver[256] = { 0 };

			// Convert wide strings to ANSI strings safely
			// If we don't then it won't work, alternative would be using trustedsec's base.c but this is how I will solve for now.
			if (exterminate[i].wkui1_username)
				KERNEL32$WideCharToMultiByte(CP_ACP, 0, exterminate[i].wkui1_username, -1, username, sizeof(username), NULL, NULL);
			if (exterminate[i].wkui1_logon_domain)
				KERNEL32$WideCharToMultiByte(CP_ACP, 0, exterminate[i].wkui1_logon_domain, -1, domain, sizeof(domain), NULL, NULL);
			if (exterminate[i].wkui1_oth_domains)
				KERNEL32$WideCharToMultiByte(CP_ACP, 0, exterminate[i].wkui1_oth_domains, -1, othdomains, sizeof(othdomains), NULL, NULL);
			if (exterminate[i].wkui1_logon_server)
				KERNEL32$WideCharToMultiByte(CP_ACP, 0, exterminate[i].wkui1_logon_server, -1, logonserver, sizeof(logonserver), NULL, NULL);

      // If we used base.c it would be internal_printf.
			BeaconPrintf(CALLBACK_OUTPUT, "Login domain is %s\n", domain);
			BeaconPrintf(CALLBACK_OUTPUT, "Login domains are %s\n", othdomains);
			BeaconPrintf(CALLBACK_OUTPUT, "Login server is: %s\n", logonserver);
			BeaconPrintf(CALLBACK_OUTPUT, "Logged in users: %s\n", username);
		}
	}
	// If the server is null then just print our error code.
	else if (servername == NULL) {
		BeaconPrintf(CALLBACK_ERROR, "Unable to get the logged in users: %lu", mandobama);
	}
	// Clear the buffer.
	if (buf) {
		NETAPI32$NetApiBufferFree(buf);
	}
}



void go() { // Entry for coff.
	getloggedonusers(); // Function call.
}
