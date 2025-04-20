#include "beacon.h"
#include <winsock2.h>

DECLSPEC_IMPORT int WINAPI WS2_32$WSAStartup(WORD wVersionRequired, LPWSADATA lpWSAData); 

DECLSPEC_IMPORT int WINAPI WS2_32$gethostname(char *name, int namelen);

void go(char* args, int len) {
	WSADATA wsaData;
	char host[256];
	int result;

	result = WS2_32$WSAStartup(2, &wsaData);
	if (WS2_32$gethostname(host, sizeof(host)) == 0) {
		BeaconPrintf(CALLBACK_OUTPUT, "Hostname of the machine is %s", host);
	}
	WS2_32$WSACleanup();
}
