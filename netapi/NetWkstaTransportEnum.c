#include <windows.h>
#include <lm.h>
#include "beacon.h"

DECLSPEC_IMPORT NET_API_STATUS NETAPI32$NetWkstaTransportEnum(LPTSTR, DWORD, LPBYTE*, DWORD, LPDWORD, LPDWORD, LPDWORD);
DECLSPEC_IMPORT NET_API_STATUS NETAPI32$NetApiBufferFree(LPVOID);
DECLSPEC_IMPORT int WINAPI KERNEL32$WideCharToMultiByte(UINT CodePage, DWORD dwFlags, LPCWCH lpWideCharStr, int cchWideChar, LPSTR lpMultiByteStr, int cbMultiByte, LPCCH lpDefaultChar, LPBOOL lpUsedDefaultChar);


void exec() {
	DWORD level = 0;
	WKSTA_TRANSPORT_INFO_0* workbuf = NULL;
	DWORD prefmaxlen = MAX_PREFERRED_LENGTH;
	DWORD entriesread = 0;
	DWORD totalentries = 0;
	DWORD resume_handle = 0;

	NET_API_STATUS rawr = NETAPI32$NetWkstaTransportEnum(NULL, level, (LPBYTE*)&workbuf, prefmaxlen, &entriesread, &totalentries, &resume_handle);
	if (rawr == NERR_Success) {
		for (DWORD i = 0; i < entriesread; i++) {
			char transportname[256] = { 0 };
			char transportaddr[256] = { 0 };

			if (workbuf[i].wkti0_transport_name)
				KERNEL32$WideCharToMultiByte(CP_ACP, 0, workbuf[i].wkti0_transport_name, -1, transportname, sizeof(transportname), NULL, NULL);
			if (workbuf[i].wkti0_transport_address)
				KERNEL32$WideCharToMultiByte(CP_ACP, 0, workbuf[i].wkti0_transport_address, -1, transportaddr, sizeof(transportaddr), NULL, NULL);

			BeaconPrintf(CALLBACK_OUTPUT, "clients connected %lu, quality of service %lu\n", workbuf[i].wkti0_number_of_vcs, workbuf[i].wkti0_quality_of_service);
			BeaconPrintf(CALLBACK_OUTPUT, "Transport name device %s, transport address is %s", transportname, transportaddr);
		}
	}
	else {
		BeaconPrintf(CALLBACK_ERROR, "Failure with code %lu", rawr);
	}
	NETAPI32$NetApiBufferFree(workbuf);
}


void go() {
	exec();
}
