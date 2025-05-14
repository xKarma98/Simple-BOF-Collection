#include "beacon.h" // Cobalt strike api include.
#include <windows.h> // Windows api include.
#include <lm.h> // Working with NETAPI, NetUserModalsGet, NetApiBufferFree.

// Imports for our code to work.
DECLSPEC_IMPORT NET_API_STATUS WINAPI NETAPI32$NetUserModalsGet(LPCWSTR servername, DWORD level, LPBYTE* bufptr);
DECLSPEC_IMPORT NET_API_STATUS WINAPI NETAPI32$NetApiBufferFree(LPVOID buffer);

// Pass this parameter.
// wchar is wide character.
void callyou(const wchar_t* servername) {
  // We use these 2 structs to retrive data for our printing.
	USER_MODALS_INFO_0* buf = NULL; // Pointer to struct member.
	USER_MODALS_INFO_3* buf2 = NULL; // Pointer to the struct member.

  // Passing servername information level & fixing our buffer.
	NET_API_STATUS yesyou = NETAPI32$NetUserModalsGet(servername, 0, (LPBYTE*)&buf);
	if (yesyou == NERR_Success) {
    // In %d for decimal value we print out the time in seconds or by our following division.
		BeaconPrintf(CALLBACK_OUTPUT, "[+] Min pass length: %d\n", buf->usrmod0_min_passwd_len);
		// Numbers have came from MSDN Doc's of NetUserModalsGet :)
		BeaconPrintf(CALLBACK_OUTPUT, "[+] max password age: %d\n", buf->usrmod0_max_passwd_age/86400);
		BeaconPrintf(CALLBACK_OUTPUT, "[+] min password age: %d\n", buf->usrmod0_min_passwd_age/86400);
		BeaconPrintf(CALLBACK_OUTPUT, "[+] amount of time between the end of logon: %d\n", buf->usrmod0_force_logoff);
		BeaconPrintf(CALLBACK_OUTPUT, "[+] Password history length: %d\n", buf->usrmod0_password_hist_len);
	}
	else { // Else statement to pull the error codes.
		BeaconPrintf(CALLBACK_ERROR, "Unable to get information %lu\n", yesyou);
	} 
	if (buf) { // Clear out the bufferm repeat next line.
		NETAPI32$NetApiBufferFree(buf);
	}

  // Getting information with our pointer to the level 3 information.
  // Server -> info level -> pointer.
	NET_API_STATUS runthisfade = NETAPI32$NetUserModalsGet(servername, 3, (LPBYTE*)&buf2);
	if (runthisfade == NERR_Success) {
		BeaconPrintf(CALLBACK_OUTPUT, "[+](sec) lockout durations: %d\n", buf2->usrmod3_lockout_duration);
		BeaconPrintf(CALLBACK_OUTPUT, "[+](seconds) max time between 2 failed attempts: %d\n", buf2->usrmod3_lockout_observation_window);
		BeaconPrintf(CALLBACK_OUTPUT, "[+] How many attmepts before lockout: %d\n", buf2->usrmod3_lockout_threshold);
	}
	else {
		BeaconPrintf(CALLBACK_ERROR, "Unable to get information %lu\n", runthisfade);
	}
	if (buf2) {
		NETAPI32$NetApiBufferFree(buf2);
	}
}

// Handle arguments.
void go(int argc, char* argv[]) {
	if (argc < 2) {
		callyou(L"localhost");
	}
}
