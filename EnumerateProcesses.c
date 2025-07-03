#include <windows.h>
#include <wtsapi32.h>
#include <sddl.h>
#include "beacon.h"

typedef HANDLE (WINAPI *PPN_WTSOpenServerA)(LPSTR);
typedef BOOL (WINAPI *PPN_WTSEnumerateProcessesA)(HANDLE, DWORD, DWORD, PWTS_PROCESS_INFOA* ppSessionInfo, DWORD *pcount);
typedef BOOL(WINAPI *PPN_LookupAccountSidA)(LPCSTR lpSystemName, PSID Sid, LPSTR Name, LPDWORD  cchName, LPSTR ReferencedDomainName, LPDWORD cchReferencedDomainName, PSID_NAME_USE peUse);
typedef void(WINAPI* PPN_WTSCloseServer)(HANDLE);
typedef void(WINAPI* PPN_WTSFreeMemory)(PVOID);

void exec() {
	// Load dll
	HMODULE hat = LoadLibraryA("Wtsapi32.dll");
	// GetProcAddresses.
	FARPROC dat = GetProcAddress(hat, "WTSOpenServerA");
	FARPROC rat = GetProcAddress(hat, "WTSEnumerateSessionsA");
	FARPROC das = GetProcAddress(hat, "WTSCloseServer");
	FARPROC las = GetProcAddress(hat, "WTSFreeMemory");

	// Loading WTSOpenServerA & EnumProc & close serv.
	PPN_WTSOpenServerA pWTSOpenServerA = (PPN_WTSOpenServerA)GetProcAddress(hat, "WTSOpenServerA");
	PPN_WTSEnumerateProcessesA pWTSEnumerateProcessesA = (PPN_WTSEnumerateProcessesA)GetProcAddress(hat, "WTSEnumerateProcessesA");
	PPN_WTSCloseServer pWTSCloseServer = (PPN_WTSCloseServer)GetProcAddress(hat, "WTSCloseServer");
	PPN_WTSFreeMemory pWTSFreeMemory = (PPN_WTSFreeMemory)GetProcAddress(hat, "WTSFreeMemory");

	// Loading lookupaccountSid.
	HMODULE ds = LoadLibraryA("Advapi32.dll"); // DLL load for LookupAccountSidA.
	FARPROC mop = GetProcAddress(ds, "LookupAccountSidA");
	PPN_LookupAccountSidA pLookupAccountSidA = (PPN_LookupAccountSidA)(GetProcAddress(ds, "LookupAccountSidA"));


	// Handle to our localhost, 2. we can use WTS_CURRENT_SERVER but it doesn't work much :/
	HANDLE ser = pWTSOpenServerA("localhost");
	if (!ser) {
		BeaconPrintf(CALLBACK_ERROR, "Failure to get a handle: %lu", ser);
	}
	else {
		BeaconPrintf(CALLBACK_OUTPUT, "Successfully got a handle\n");
	}
	// WTSEnumerateSessions
	    WTS_PROCESS_INFOA* pi = NULL; // Pointer to the pointer to an arry of WTS_PROCESS_INFO.
		DWORD pcount = 0; // Required for the loop to be followed.
		// 1. Handle, 2. version, 3. pointer to array, 4. recieves the number of WTS_PROCCES_INFO structs returned in the process info buffer.
		BOOL da = pWTSEnumerateProcessesA(ser, 0, 1, &pi, &pcount);
		if (!da) {
			BeaconPrintf(CALLBACK_ERROR, "Failed to enumerate sessions: %lu", da);
		}
		else {
			BeaconPrintf(CALLBACK_OUTPUT, "Successfully called the api.\n");
		}


		// For loop to gather processes and domain attached.
		for (DWORD i = 0; i < pcount; i++) {
			PSID usersid = pi[i].pUserSid; // User sid.
			char name[256] = { 0 }; // Null terminators.
			char domain[256] = { 0 };
			// Sizeof buffers.
			DWORD cchname = sizeof(name);
			DWORD cchrefdomain = sizeof(domain);

			// Pointer to variable.
			SID_NAME_USE suse;
			// 1. Server name = NULL, 2. a pointer to SID for lookup, 3. pointer to buffer that recieves null terminated strings, 4. sizeof(buffer), 5. domain, 6. sizeof(domain), 7. pointer to variable that recieves SID_NAME_USE values that indicate the account type.
			if (pLookupAccountSidA(NULL, usersid, name, &cchname, domain, &cchrefdomain, &suse)) {
				BeaconPrintf(CALLBACK_OUTPUT, "[+] Process = %s, PID = %d, Domain = %s\\%s\n", pi[i].pProcessName, pi[i].ProcessId, domain, name);
			}
		}
		// Close & free memory to avoid leaks.
		pWTSCloseServer(ser);
		pWTSFreeMemory(pi);
	}

void go() {
	exec();
}
