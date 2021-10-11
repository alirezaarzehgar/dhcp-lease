#include "lease/lease.h"

#if !defined(TESTS_LEASE)
#define TESTS_LEASE

int initSuiteLease();

int cleanupSuiteLease();

void dhcpLeaseGetIpFromPoolTest();

void dhcpLeaseIpAddressTest();

#endif // TESTS_LEASE
