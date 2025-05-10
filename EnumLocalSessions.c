#include "beacon.h" // Cobalt api's.
#include <windows.h> // Windows api include.
#include <WtsApi32.h> // Include for Terminal services, ENUMsess, querysess, freemem.

// Pragma to include.
#pragma comment(lib, "wtsapi32.lib")


// Flag the _imps
DECLSPEC_IMPORT BOOL WINAPI WTSAPI32$WTSEnumerateSessionsA(HANDLE hserver, DWORD reserved, DWORD version, PWTS_SESSION_INFOA* ppSessionInfo, DWORD* pcount);
DECLSPEC_IMPORT BOOL WINAPI WTSAPI32$WTSQuerySessionInformationA(HANDLE hserver, DWORD SessionId, WTS_INFO_CLASS wtsinfoclass, LPSTR *ppBuffer, DWORD *pBytesReturned);
DECLSPEC_IMPORT VOID WINAPI WTSAPI32$WTSFreeMemory(PVOID memory);


// Void function.
void enumlocsessions() {
    // pointer to the SESSION_INFO struct intialized to NULL to maintain a bit of protection.
	WTS_SESSION_INFO* wtsinfo = NULL;
	DWORD wtscount; // For the looping.
	DWORD index; // Our dword for the loop.

    // Getting our current server handle our reserved, ver, & pointers to our class & buffer, tldr. get information about terminal sessions.
	BOOL hk = WTSAPI32$WTSEnumerateSessionsA(WTS_CURRENT_SERVER_HANDLE, 0, 1, &wtsinfo, &wtscount);
	if (hk != 0) { // If not equal to 0.
		DWORD size; // we use this in the size.
		// Strings.
		LPTSTR username = NULL;
		LPTSTR Domain = NULL;
        // For loop 
		for (index = 0; index < wtscount; index++) {
		// Detail + printing using the member of the SESSION_INFO struct.
			BeaconPrintf(CALLBACK_OUTPUT, "---------------------- [Session Information] ----------------------\n");
			BeaconPrintf(CALLBACK_OUTPUT, "Session Id's: %lu\n", wtsinfo[index].SessionId);

            // We use the session id if it's equal to 0 we will print the system sessions out to avoid it being blank.
			if (wtsinfo[index].SessionId == 0) {
				BeaconPrintf(CALLBACK_OUTPUT, "username: <System session>\n");
				BeaconPrintf(CALLBACK_OUTPUT, "Domain/hostname: Localsystem\n");
			}

         // WTSQuerySessionInformationA - Cur handle, Dword to sessID, INFO_CLASS, buffer pointers.
			BOOL jek = WTSAPI32$WTSQuerySessionInformationA(WTS_CURRENT_SERVER_HANDLE, wtsinfo[index].SessionId, WTSUserName, &username, &size);
			if (jek != 0) {
				BeaconPrintf(CALLBACK_OUTPUT, "Username on the machine is: %s\n", username);
			}
			else {
				BeaconPrintf(CALLBACK_ERROR, "Unable to get the username: %lu\n", jek);
			} // Free memory.
			if (username) {
				WTSAPI32$WTSFreeMemory(username);
			}
			// We will do the same but our INFO_CLASS is now domain name and our reference is to domain and pointing to our size.
			BOOL iya = WTSAPI32$WTSQuerySessionInformationA(WTS_CURRENT_SERVER_HANDLE, wtsinfo[index].SessionId, WTSDomainName, &Domain, &size);
			if (iya && Domain != NULL && strlen(Domain) > 0) {
				BeaconPrintf(CALLBACK_OUTPUT, "Domain name/hostname is %s\n", Domain);
			}
			else {
				BeaconPrintf(CALLBACK_ERROR, "Domain wasn't resolved: %lu\n", iya);
			}
			// Switch statements to get the connectivity state.
			if (Domain) {
				WTSAPI32$WTSFreeMemory(Domain);
			}
			switch (wtsinfo[index].State) {
			case WTSActive:
				BeaconPrintf(CALLBACK_OUTPUT, "Session is active\n");
				break;
			case WTSDisconnected:
				BeaconPrintf(CALLBACK_OUTPUT, "Session is disconnected\n");
				break;
			case WTSIdle:
				BeaconPrintf(CALLBACK_OUTPUT, "Session is idle\n");
				break;
			case WTSDown:
				BeaconPrintf(CALLBACK_OUTPUT, "Session is down\n");
			}
		}
	}
	WTSAPI32$WTSFreeMemory(wtsinfo);
}


void go() { // Coffloader entrypoint.
	enumlocsessions(); // Function call.
}
