#include "beacon.h"

void go() {
	if (BeaconIsAdmin) {
		BeaconPrintf(CALLBACK_OUTPUT, "We are admin let's gooooo\n");
	}
	else {
		BeaconPrintf(CALLBACK_ERROR, "we are not admin :(\n");
	}
}
