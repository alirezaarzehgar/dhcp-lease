/**
 * @file lease_test.c
 * @author alirezaarzehgar (alirezaarzehgar82@gmail.com)
 * @brief Incomplete and wrong test
 * @version 0.1
 * @date 2021-10-11
 *
 * @copyright Copyright (c) 2021
 *
 */

/**
 *              TODO We should refactor this tests and improve cases
 */

#include "lease_test.h"
#include <CUnit/CUnit.h>
#include <time.h>

#define CLIENT_MAC_ADDERSS  "d4:f2:76:d0:a4:0f"
#define CLIENT_HOSTNAME     "ali"
#define MAX_LEASE           200

#define CODE_BEHINDE_INITCLOSE_DB(TODO)      \
do {      \
  dhcpLeaseInit (FAKE_DATABASE_PATH);     \
      \
  { TODO }      \
        \
  dhcpLeaseClose();     \
} while(0)

int
initSuiteLease()
{
  srand (time (NULL));
  return 0;
}

int
cleanupSuiteLease()
{
  return 0;
}

void
printLease (dhcpLeasePoolResult_t lease)
{
  printf (
    "\n\n"
    "lease :\n"
    "\tid -> %d\n"
    "\tip -> %s\n"
    "\thost -> %s\n"
    "\tmac -> %s\n\n", lease.id, lease.ip, lease.host, lease.mac);

  printf ("configs : \n"
          "\tid -> %d\n"
          "\tmask -> %s\n"
          "\trouter -> %s\n"
          "\tdomain -> %s \n"
          "\tlease_time -> %d\n", lease.config.id, lease.config.mask,
          lease.config.router,
          lease.config.domain, lease.config.lease_time);

  printf ("\n\n");
}

void
dhcpLeaseGetIpFromPoolTest()
{
  CODE_BEHINDE_INITCLOSE_DB (
  {
    dhcpLeasePoolResult_t lease;

    bzero (&lease, sizeof (dhcpLeasePoolResult_t));

    lease = dhcpLeaseGetIpFromPool (CLIENT_MAC_ADDERSS);

    printLease (lease);
  });
}

void
dhcpLeaseIpAddressTest()
{
  CODE_BEHINDE_INITCLOSE_DB (
  {
    dhcpLeasePoolResult_t lease;

    int retval;

    lease = dhcpLeaseGetIpFromPool (CLIENT_MAC_ADDERSS);

    retval = dhcpLeaseIpAddress (lease.id, CLIENT_MAC_ADDERSS, CLIENT_HOSTNAME);

    CU_ASSERT_TRUE (retval);
  });
}

void
dhcpLeaseInitConfTest()
{
  dhcpLeaseInit ("test.db");
  dhcpLeaseInitConf();
}

void
dhcpLeaseInitPoolTest()
{
  dhcpLeaseInitPool();
  dhcpLeaseClose();
}

void
dhcpLeaseInitTest()
{
  /*  TODO dhcpLeaseInitTest */
}

void
dhcpLeaseCloseTest()
{
  /*  TODO dhcpLeaseCloseTest */
}

void
dhcpLeaseMacAddressAlreadyExistsTest()
{
  /*  TODO dhcpLeaseMacAddressAlreadyExistsTest */
}

void
dhcpLeaseGetConfigByIdTest()
{
  /*  TODO dhcpLeaseGetConfigByIdTest */
}

void
dhcpLeasePoolGetByIdTest()
{
  BASE_GET_BY_X (dhcpLeasePoolGetById, rand() % MAX_LEASE);
}

void
dhcpLeasePoolGetByMacTest()
{
  BASE_GET_BY_X (dhcpLeasePoolGetByMac, CLIENT_MAC_ADDERSS);
}

void
dhcpLeasePoolGetByHostnameTest()
{
  BASE_GET_BY_X (dhcpLeasePoolGetByHostname, CLIENT_HOSTNAME);
}

void
dhcpLeasePoolGetByIpTest()
{
  BASE_GET_BY_X (dhcpLeasePoolGetByIp, "192.168.133.6");
}

void
dhcpLeasePoolCountTest()
{
  CODE_BEHINDE_INITCLOSE_DB (
  {
    CU_ASSERT_EQUAL (dhcpLeasePoolCount(), MAX_LEASE);
  });
}

void
dhcpLeaseConfCountTest()
{
  CODE_BEHINDE_INITCLOSE_DB (
  {
    CU_ASSERT_EQUAL (dhcpLeaseConfCount(), 5);
  });
}
