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
  {"dhcpLeasePoolGetByIdTest", dhcpLeasePoolGetByIdTest},
  {"dhcpLeasePoolGetByMacTest", dhcpLeasePoolGetByMacTest},
  {"dhcpLeasePoolGetByHostnameTest", dhcpLeasePoolGetByHostnameTest},
  {"dhcpLeasePoolGetByIpTest", dhcpLeasePoolGetByIpTest},
  {"dhcpLeasePoolCountTest", dhcpLeasePoolCountTest},
  {"dhcpLeaseConfCountTest", dhcpLeaseConfCountTest},
  {"dhcpLeaseSqlBuilderPoolUpdateTest", dhcpLeaseSqlBuilderPoolUpdateTest},
  {"dhcpLeaseSqlBuilderConfUpdateTest", dhcpLeaseSqlBuilderConfUpdateTest},
  {"dhcpLeaseSqlBuilderNewPoolTest", dhcpLeaseSqlBuilderNewPoolTest},
  {"dhcpLeaseSqlBuilderNewConfTest", dhcpLeaseSqlBuilderNewConfTest},
  {"dhcpLeaseSaveNewLeaseTest", dhcpLeaseSaveNewLeaseTest},
  {"dhcpLeaseUpdateLeaseTest", dhcpLeaseUpdateLeaseTest},
  {"dhcpLeaseSaveNewConfigTest", dhcpLeaseSaveNewConfigTest},
  {"dhcpLeaseUpdateConfigTest", dhcpLeaseUpdateConfigTest},
  {"dhcpLeaseGetConfigByIdTest", dhcpLeaseGetConfigByIdTest},
  {"dhcpLeaseDeleteLeaseByIdTest", dhcpLeaseDeleteLeaseByIdTest},
  {"dhcpLeaseSqlBuilderPoolDeleteByIdTest", dhcpLeaseSqlBuilderPoolDeleteByIdTest},
  {"dhcpLeaseSqlBuilderConfigDeleteByIdTest", dhcpLeaseSqlBuilderConfigDeleteByIdTest},
  {"dhcpLeaseDeleteConfigByIdTest", dhcpLeaseDeleteConfigByIdTest},
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
