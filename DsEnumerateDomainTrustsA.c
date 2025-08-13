#include <windows.h>
#include <dsgetdc.h>
#include <lm.h>
#include "beacon.h"

// Flags me + BeaconPrintf.
DECLSPEC_IMPORT DWORD NETAPI32$DsEnumerateDomainTrustsA(LPSTR, ULONG, PDS_DOMAIN_TRUSTS*, PULONG);
DECLSPEC_IMPORT NET_API_STATUS NETAPI32$NetApiBufferFree(LPVOID);

void exec() {
	DWORD i;
	PDS_DOMAIN_TRUSTS dtrusts = NULL;
	ULONG domaincount = 0;
	DWORD mando = NETAPI32$DsEnumerateDomainTrustsA(NULL, DS_DOMAIN_IN_FOREST, &dtrusts, &domaincount);
		if (mando == ERROR_SUCCESS) {
			for (i = 0; i < domaincount; i++) {
				BeaconPrintf(CALLBACK_OUTPUT, "Flags %lu", dtrusts[i].Flags);
				BeaconPrintf(CALLBACK_OUTPUT, "DnsDomainName: %s, NetBios Domain name %s,  trust type %lu\n", dtrusts[i].DnsDomainName, dtrusts[i].NetbiosDomainName, dtrusts[i].TrustType);
			}
		}
		else {
			BeaconPrintf(CALLBACK_ERROR, "error %lu", mando);
		}
		NETAPI32$NetApiBufferFree(dtrusts);
}

void go() {
	exec();
}
