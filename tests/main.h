#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>

#include "lease_test.h"

#if !defined(TESTS_MAIN_H)
#define TESTS_MAIN_H

CU_TestInfo lease_tests[] =
{
  {"dhcpLeaseGetIpFromPoolTest", dhcpLeaseGetIpFromPoolTest},
  {"dhcpLeaseIpAddressTest", dhcpLeaseIpAddressTest},
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
