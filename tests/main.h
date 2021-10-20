#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>

#include "lease_test.h"

#if !defined(TESTS_MAIN_H)
#define TESTS_MAIN_H

CU_TestInfo lease_tests[] =
{
  {"dhcpLeaseGetIpFromPoolTest", dhcpLeaseGetIpFromPoolTest},
  {"dhcpLeaseIpAddressTest", dhcpLeaseIpAddressTest},
  {"dhcpLeaseInitConfTest", dhcpLeaseInitConfTest},
  {"dhcpLeaseInitPoolTest", dhcpLeaseInitPoolTest},
  {"dhcpLeaseMacAddressAlreadyExistsTest", dhcpLeaseMacAddressAlreadyExistsTest},
  {"dhcpLeaseGetConfigByIdTest", dhcpLeaseGetConfigByIdTest},
  {"dhcpLeasePoolGetByIdTest", dhcpLeasePoolGetByIdTest},
  {"dhcpLeasePoolGetByMacTest", dhcpLeasePoolGetByMacTest},
  {"dhcpLeasePoolGetByHostnameTest", dhcpLeasePoolGetByHostnameTest},
  {"dhcpLeasePoolGetByIpTest", dhcpLeasePoolGetByIpTest},
  {"dhcpLeasePoolCountTest", dhcpLeasePoolCountTest},
  {"dhcpLeaseConfCountTest", dhcpLeaseConfCountTest},
  {"dhcpLeaseSqlBuilderPoolUpdateTest", dhcpLeaseSqlBuilderPoolUpdateTest},
  {"dhcpLeaseSqlBuilderConfUpdateTest", dhcpLeaseSqlBuilderConfUpdateTest},
  {"dhcpLeaseSqlBuilderPoolDeleteByIdTest", dhcpLeaseSqlBuilderPoolDeleteByIdTest},
  {"dhcpLeaseSqlBuilderConfigDeleteByIdTest", dhcpLeaseSqlBuilderConfigDeleteByIdTest},
  {"dhcpLeaseSqlBuilderNewPoolTest", dhcpLeaseSqlBuilderNewPoolTest},
  {"dhcpLeaseSqlBuilderNewConfTest", dhcpLeaseSqlBuilderNewConfTest},
  CU_TEST_INFO_NULL,
};

CU_SuiteInfo suites[] =
{
  {
    .pName = "dhcp lease suite",
    .pInitFunc = initSuiteLease,
    .pCleanupFunc = cleanupSuiteLease,
    .pTests = lease_tests,
  },
  CU_SUITE_INFO_NULL,
};

#endif
