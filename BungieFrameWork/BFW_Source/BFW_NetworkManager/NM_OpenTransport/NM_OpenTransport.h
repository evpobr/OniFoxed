// ======================================================================
// NM_OT_UDP.h
// ======================================================================
#ifndef NM_OT_UDP_H
#define NM_OT_UDP_H

// ======================================================================
// includes
// ======================================================================
#include "BFW.h"

// ======================================================================
// prototypes
// ======================================================================
char*
NMrOT_AddressToString(
	const NMtNetAddress			*inAddress);

UUtError
NMrOT_UDP_Initialize(
	void);

void
NMrOT_UDP_Terminate(
	void);

// ======================================================================
#endif /* NM_OT_UDP_H */