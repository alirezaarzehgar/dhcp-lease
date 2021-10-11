#include "lease/lease.h"

#if !defined(TESTS_LEASE)
#define TESTS_LEASE

#define FAKE_DATABASE_PATH                      "databases/dhcpd.db"

int initSuiteLease();

int cleanupSuiteLease();

void dhcpLeaseGetIpFromPoolTest();

void dhcpLeaseIpAddressTest();

#endif // TESTS_LEASE
