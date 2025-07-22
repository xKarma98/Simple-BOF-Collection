#include <windows.h>
#include <iphlpapi.h>
#include "beacon.h"

// Flag me.
DECLSPEC_IMPORT BOOL WINAPI KERNEL32$HeapFree(HANDLE, DWORD, LPVOID);
DECLSPEC_IMPORT HANDLE WINAPI KERNEL32$GetProcessHeap();
DECLSPEC_IMPORT LPVOID WINAPI KERNEL32$HeapAlloc(HANDLE, DWORD, SIZE_T);

DECLSPEC_IMPORT DWORD IPHLPAPI$GetNetworkParams(PFIXED_INFO, PULONG);
DECLSPEC_IMPORT DWORD KERNEL32$GetLastError();

void go() {
    // This pointer will allow us to gather data of host, domain etc.
	FIXED_INFO* Pfixed;
	// To get ip addresses of the dns server.
	IP_ADDR_STRING* pIPAddr;
	// Second parameter for function.
	ULONG pOutBufLen;
	DWORD dwret = 0; // DWORD value we use later on.
	
    // Manually allocate heap.
	Pfixed = (FIXED_INFO*)KERNEL32$HeapAlloc(KERNEL32$GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(FIXED_INFO));
	if (Pfixed == NULL) { // NULL pointer no no.
		BeaconPrintf(CALLBACK_OUTPUT, "Failed to allocate memory %lu", KERNEL32$GetLastError());
		return;
	}
	pOutBufLen = sizeof(FIXED_INFO); // Set this to the size of fixed info.


    // Handling for a buffer overflow.
	if (IPHLPAPI$GetNetworkParams(Pfixed, &pOutBufLen) == ERROR_BUFFER_OVERFLOW) {
		KERNEL32$HeapFree(KERNEL32$GetProcessHeap, 0, Pfixed);
		Pfixed = (FIXED_INFO*)KERNEL32$HeapAlloc(KERNEL32$GetProcessHeap, 0, sizeof(FIXED_INFO));
		if (Pfixed == NULL) {
			BeaconPrintf(CALLBACK_OUTPUT, "Failed to allocate memory to heap %lu", KERNEL32$GetLastError());
			return;
		}
	}


	// If it passes without a error begin printing the information.
	if (dwret = IPHLPAPI$GetNetworkParams(Pfixed, &pOutBufLen) == NO_ERROR) {
		BeaconPrintf(CALLBACK_OUTPUT, "HostName %s\n", Pfixed->HostName);
		BeaconPrintf(CALLBACK_OUTPUT, "Domain: %s\n", Pfixed->DomainName);

		BeaconPrintf(CALLBACK_OUTPUT, "DNS Servers:\n");
		BeaconPrintf(CALLBACK_OUTPUT, "\t%s\n", Pfixed->DnsServerList.IpAddress.String);

		pIPAddr = Pfixed->DnsServerList.Next;

		// Thx microsoft.
		while (pIPAddr) {
			BeaconPrintf(CALLBACK_OUTPUT, "Dns server ip %s\n", pIPAddr->IpAddress.String);
			pIPAddr = pIPAddr->Next;
		}

		BeaconPrintf(CALLBACK_OUTPUT, "Node type: ");
		switch (Pfixed->NodeType) {
		case BROADCAST_NODETYPE:
			BeaconPrintf(CALLBACK_OUTPUT, "Broadcast node\n");
			break;

		case PEER_TO_PEER_NODETYPE:
			BeaconPrintf(CALLBACK_OUTPUT, "It's peer to peer\n");
			break;

		case MIXED_NODETYPE:
			BeaconPrintf(CALLBACK_OUTPUT, "Mixed node type\n");
			break;

		case HYBRID_NODETYPE:
			BeaconPrintf(CALLBACK_OUTPUT, "Hybrid node\n");
			break;

		default:
			BeaconPrintf(CALLBACK_OUTPUT, "Unknown node type %0lx\n", Pfixed->NodeType);
			break;
		}
		BeaconPrintf(CALLBACK_OUTPUT, "DHCP scope name: %s\n", Pfixed->ScopeId);

		if (Pfixed->EnableRouting)
			BeaconPrintf(CALLBACK_OUTPUT, "Routing: enabled\n");
		else
			BeaconPrintf(CALLBACK_OUTPUT, "Routing: Disabled\n");

		if (Pfixed->EnableProxy)
			BeaconPrintf(CALLBACK_OUTPUT, "ARP proxy: enabled\n");
		else
			BeaconPrintf(CALLBACK_OUTPUT, "ARP proxy: disabled\n");
	}
	else {
		BeaconPrintf(CALLBACK_OUTPUT, "GetNetworkParams failed with error: %d", dwret);
		return; 
	} if (Pfixed) { // free our memory to avoid common problems.
		KERNEL32$HeapFree(KERNEL32$GetProcessHeap, 0, Pfixed);
	}
}
