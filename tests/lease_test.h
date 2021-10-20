#include "lease/lease.h"

#if !defined(TESTS_LEASE)
#define TESTS_LEASE

#define FAKE_DATABASE_PATH                      "databases/dhcpd.db"

#define BASE_GET_BY_X(function, value)   \
  dhcpLeaseInit (FAKE_DATABASE_PATH);     \
      \
  dhcpLeasePoolResult_t lease = function (value);     \
      \
  printLease (lease);     \
      \
  dhcpLeaseClose();

int initSuiteLease();

int cleanupSuiteLease();

void dhcpLeaseGetIpFromPoolTest();

void dhcpLeaseIpAddressTest();

void dhcpLeaseInitConfTest();

void dhcpLeaseInitPoolTest();

void dhcpLeaseMacAddressAlreadyExistsTest();

void dhcpLeaseGetConfigByIdTest();

void dhcpLeasePoolGetByIdTest();

void dhcpLeasePoolGetByMacTest();

void dhcpLeasePoolGetByHostnameTest();

void dhcpLeasePoolGetByIpTest();

void dhcpLeasePoolCountTest();

void dhcpLeaseConfCountTest();

void dhcpLeaseSqlBuilderPoolUpdateTest();

void dhcpLeaseSqlBuilderConfUpdateTest();

void dhcpLeaseSqlBuilderPoolDeleteByIdTest();

void dhcpLeaseSqlBuilderConfigDeleteByIdTest();

void dhcpLeaseSqlBuilderNewPoolTest();

void dhcpLeaseSqlBuilderNewConfTest();

#endif // TESTS_LEASE
