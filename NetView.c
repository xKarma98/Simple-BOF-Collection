#include <windows.h> // Windows api, DWORDS, LMCSTR, etc.
#include <lm.h> // Netapi.
#include "beacon.h" // Cobalt strike beacon api's.

// Write a rule about me please, also these are our imports.
// These are also the parameters.
DECLSPEC_IMPORT NET_API_STATUS WINAPI NETAPI32$NetServerEnum(LMCSTR, DWORD, LPBYTE*, DWORD, LPDWORD, LPDWORD, DWORD, LMCSTR, LPDWORD);
DECLSPEC_IMPORT NET_API_STATUS WINAPI NETAPI32$NetApiBufferFree(LPVOID);

void exec() { // Function.
	// Order of function parameters.
	LMCSTR servername = NULL; // Unknown servername so NULL.
	DWORD level = 101; // We want more detail so we choose 101.
	SERVER_INFO_101 *pServinfo = NULL; // Init to 0 to be sane.
	DWORD maxpreflen = MAX_PREFERRED_LENGTH; // Give me everything you got!
	DWORD entriesread = 0; // Initialize to 0.
	DWORD totalentries = 0; // Initialize to 0.
	DWORD servtype = SV_TYPE_ALL; // We want everything just like your local feds.
	LMCSTR domain = NULL; // Unknown domain so 0.
	DWORD resumehandle = 0;

	// netapi function -> 1. servername, level, buffer, max preferred length, entriesread, totals, servtype, domain, resumehandle.
	NET_API_STATUS netview = NETAPI32$NetServerEnum(servername, level, (LPBYTE*)&pServinfo, maxpreflen, &entriesread, &totalentries, servtype, domain, &resumehandle);
	if (netview == NERR_Success) {
		if (entriesread == 0) { // If we have no entries being read print this.
			BeaconPrintf(CALLBACK_OUTPUT, "No servers exist on this host\n");
		}
		else { // Loop through the entries and print out data.
			for (DWORD i = 0; i < entriesread; ++i) {
				// Print simple data.
				BeaconPrintf(CALLBACK_OUTPUT, "Server name's on the host are %s\n", pServinfo[i].sv101_name); // Print dom names.
				BeaconPrintf(CALLBACK_OUTPUT, "Server types %s\n", pServinfo[i].sv101_type); // The server type ranging from mssql to etc.
				BeaconPrintf(CALLBACK_OUTPUT, "Server versions %s\n", pServinfo[i].sv101_version_major); // Give me your domain server versions.
				// More members of this struct can be called but we leave this stupid basic.
			}
		}
	}
	else { // Error handling.
		BeaconPrintf(CALLBACK_ERROR, "Error code: %lu\n", netview);
	}
	if (pServinfo) { // Clear out the memory.
		NETAPI32$NetApiBufferFree(pServinfo);
	}
}

void go() { // Coffloader64 entrypoint.
	exec(); // Function call.
}
