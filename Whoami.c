#include <windows.h>
#include "beacon.h"

// Look at me I am loud.
DECLSPEC_IMPORT BOOL WINAPI ADVAPI32$GetUserNameA(LPSTR lpbuf, LPDWORD pcbBuffer);
DECLSPEC_IMPORT DWORD WINAPI KERNEL32$GetLastError();
DECLSPEC_IMPORT BOOL WINAPI ADVAPI32$LookupAccountNameA(LPCSTR, LPCSTR, PSID, LPDWORD, LPSTR, LPDWORD, PSID_NAME_USE);
DECLSPEC_IMPORT BOOL WINAPI ADVAPI32$OpenProcessToken(HANDLE, DWORD, PHANDLE);
DECLSPEC_IMPORT BOOL WINAPI ADVAPI32$GetTokenInformation(HANDLE, TOKEN_INFORMATION_CLASS, LPVOID, DWORD, PDWORD);
DECLSPEC_IMPORT HANDLE WINAPI KERNEL32$GetCurrentProcess();
DECLSPEC_IMPORT BOOL WINAPI KERNEL32$CloseHandle(HANDLE hObject);
DECLSPEC_IMPORT BOOL WINAPI ADVAPI32$LookupPrivilegeNameA(LPCSTR, PLUID, LPSTR, LPDWORD);
DECLSPEC_IMPORT BOOL WINAPI ADVAPI32$LookupAccountSidA(LPCSTR lpSystemName, PSID Sid, LPSTR Name, LPDWORD cchName, LPSTR ReferencedDomainName, LPDWORD cchReferencedDomainName, PSID_NAME_USE peUse);

// Using heap over malloc because we have nothing but problems apparently.
DECLSPEC_IMPORT BOOL WINAPI KERNEL32$HeapFree(HANDLE, DWORD, LPVOID);
DECLSPEC_IMPORT HANDLE WINAPI KERNEL32$GetProcessHeap();
DECLSPEC_IMPORT LPVOID WINAPI KERNEL32$HeapAlloc(HANDLE, DWORD, SIZE_T);

void exec() {
	/////////// name ////////////////
	char name[50];
	DWORD namsize = sizeof(name);
	if (ADVAPI32$GetUserNameA(name, &namsize)) {
		BeaconPrintf(CALLBACK_OUTPUT, "User is %s\n", name);
	}
	else {
		BeaconPrintf(CALLBACK_ERROR, "Error unable to get the name %lu\n", KERNEL32$GetLastError());
	}

	/////// Domain ///////////
	char sidbuf[50];
	DWORD sidsize = sizeof(sidbuf);

    // Domain buffer and size.
	char domainname[50];
	DWORD dsize = sizeof(domainname);

	SID_NAME_USE psid; // member pointer.
	if (ADVAPI32$LookupAccountNameA(NULL, name, sidbuf, &sidsize, domainname, &dsize, &psid)) {
		BeaconPrintf(CALLBACK_OUTPUT, "Domain name is %s\n", domainname);
	}
	else {
		BeaconPrintf(CALLBACK_ERROR, "No domain resolved %lu\n", KERNEL32$GetLastError());
	}

	////// Token yoink ///////
	HANDLE htoken = NULL;
	// If our OpenProcess with token fails print error.
	if (!ADVAPI32$OpenProcessToken(KERNEL32$GetCurrentProcess(), TOKEN_QUERY, &htoken)) {
		BeaconPrintf(CALLBACK_ERROR, "failed to get process token %lu\n", KERNEL32$GetLastError());
		return;
	}

	DWORD tokenlength = 0;
	// Get the token information about the privledges and read the token length.
	ADVAPI32$GetTokenInformation(htoken, TokenPrivileges, NULL, 0, &tokenlength);

	// For Heap, to allocate memory -> heap_zero_memory initailizes it to 0, we then read the token length.
	LPVOID pPrivs = KERNEL32$HeapAlloc(KERNEL32$GetProcessHeap(), HEAP_ZERO_MEMORY, tokenlength);
	// If pPrivs fails close the handle.
	if (!pPrivs) {
		BeaconPrintf(CALLBACK_ERROR, "Failure to allocate heap %lu\n", KERNEL32$GetLastError);
		KERNEL32$CloseHandle(htoken);
		return;
	}

    // If gettokeninformation fails we print our error codes.
	if (!ADVAPI32$GetTokenInformation(htoken, TokenPrivileges, pPrivs, tokenlength, &tokenlength)) {
		BeaconPrintf(CALLBACK_ERROR, "Failed to get the token information %lu\n", KERNEL32$GetLastError());
		KERNEL32$HeapFree(KERNEL32$GetProcessHeap(), 0, pPrivs);
		return;
	}

    // Pointer to PTOKEN privs.
	PTOKEN_PRIVILEGES privs = (PTOKEN_PRIVILEGES)pPrivs;

    // Loop through priv count, store a buffer for priv names initialize to 0, we then lookupprivs and find the LUID 
	for (DWORD i = 0; i < privs->PrivilegeCount; ++i) {
		char privname[256] = { 0 };
		DWORD nameLen = sizeof(privname);
		if (ADVAPI32$LookupPrivilegeNameA(NULL, &privs->Privileges[i].Luid, privname, &nameLen)) {
			BeaconPrintf(CALLBACK_OUTPUT, "Privilege: %s (Attributes: 0x%x)\n", privname, privs->Privileges[i].Attributes);
		}
		else {
			BeaconPrintf(CALLBACK_ERROR, "Failed to resolve privilege: %lu\n", KERNEL32$GetLastError());
		}
	}
	// Free heap memory, evading UAF.
	KERNEL32$HeapFree(KERNEL32$GetProcessHeap(), 0, pPrivs);


	//////// Groups /////////
	DWORD groupLen = 0; // Initialize the length to 0.
	// Get tokeninformation about groups.
	ADVAPI32$GetTokenInformation(htoken, TokenGroups, NULL, 0, &groupLen);
	// Pointer to heapAlloc get our current process heap init to 0.
	LPVOID pGroups = KERNEL32$HeapAlloc(KERNEL32$GetProcessHeap(), HEAP_ZERO_MEMORY, groupLen);
	if (!pGroups) {
		BeaconPrintf(CALLBACK_ERROR, "Failure to allocate heap for groups %lu\n", KERNEL32$GetLastError());
		KERNEL32$CloseHandle(htoken);
		return;
	}
    Get token info pass the token, desired access = tokengroups -> pointer allocate the bytes of grouplen.
	if (!ADVAPI32$GetTokenInformation(htoken, TokenGroups, pGroups, groupLen, &groupLen)) {
		BeaconPrintf(CALLBACK_ERROR, "Failure to retrieve token info %lu\n", KERNEL32$GetLastError());
		KERNEL32$HeapFree(KERNEL32$GetProcessHeap(), 0, pGroups);
		KERNEL32$CloseHandle(htoken);
		return;
	}

    // Point to PTOKEN_GROUPS
	PTOKEN_GROUPS groups = (PTOKEN_GROUPS)pGroups;
	BeaconPrintf(CALLBACK_OUTPUT, "Group number count: %lu\n", groups->GroupCount); // Count in numbers.

	for (DWORD i = 0; i < groups->GroupCount; ++i) {
		char groupName[256] = { 0 };
		char domain[50] = { 0 };
		DWORD groupSize = sizeof(groupName);
		DWORD domainSize = sizeof(domain);
		SID_NAME_USE sidUse;
         // We lookup account security idenfitifer, pass null point to group sid to see what groups you belong to.
		if (ADVAPI32$LookupAccountSidA(NULL, groups->Groups[i].Sid, groupName, &groupSize, domain, &domainSize, &sidUse)) {
			BeaconPrintf(CALLBACK_OUTPUT, "Group: %s\\%s (Attributes: 0x%x)\n", domain, groupName, groups->Groups[i].Attributes); // We pull attributes along with groups on the machine.
		}
		else {
			BeaconPrintf(CALLBACK_ERROR, "Failed to resolve group name: %lu\n", KERNEL32$GetLastError());
		}
	}
	// Close handle to evade handle leaking.
	KERNEL32$CloseHandle(htoken);
	// Free heap memory to evade UAF exploitation.
	KERNEL32$HeapFree(KERNEL32$GetProcessHeap(), 0, pGroups);
}

void go() { // Entrypoint for coffloader.
	exec(); // Function call.
}
