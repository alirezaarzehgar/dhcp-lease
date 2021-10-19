/**
 * @file lease_test.c
 * @author alirezaarzehgar (alirezaarzehgar82@gmail.com)
 * @brief
 * @version 0.1
 * @date 2021-10-11
 *
 * @copyright Copyright (c) 2021
 *
 */

#include "lease_test.h"
#include <CUnit/CUnit.h>

#define CLIENT_MAC_ADDERSS "d4:f2:76:d0:a4:0f"

int
initSuiteLease()
{
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
    "\tmac -> %s\n\n", lease.id, lease.ip, lease.mac);

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
  dhcpLeaseInit (FAKE_DATABASE_PATH);

  dhcpLeasePoolResult_t lease;

  bzero (&lease, sizeof (dhcpLeasePoolResult_t));

  lease = dhcpLeaseGetIpFromPool (CLIENT_MAC_ADDERSS);

  printLease (lease);

  dhcpLeaseClose();
}

void
dhcpLeaseIpAddressTest()
{
  dhcpLeaseInit (FAKE_DATABASE_PATH);

  dhcpLeasePoolResult_t lease;

  int retval;

  lease = dhcpLeaseGetIpFromPool (CLIENT_MAC_ADDERSS);

  retval = dhcpLeaseIpAddress (lease.id, CLIENT_MAC_ADDERSS, "ali");

  CU_ASSERT_TRUE (retval);
  
  dhcpLeaseClose();
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
dhcpLeaseGetPoolByIdTest()
{

  dhcpLeaseInit (FAKE_DATABASE_PATH);

  dhcpLeasePoolResult_t lease = dhcpLeasePoolGetById (4);

  printLease (lease);
  
  dhcpLeaseClose();
}

void
dhcpLeaseGetConfigByIdTest()
{
  /*  TODO dhcpLeaseGetConfigByIdTest */
}

void
dhcpLeasePoolGetByIdTest()
{
  /*  TODO dhcpLeasePoolGetByIdTest */
}

void
dhcpLeasePoolGetByMacTest()
{
  /*  TODO dhcpLeasePoolGetByMacTest */
}

void
dhcpLeasePoolGetByHostnameTest()
{
  /*  TODO dhcpLeasePoolGetByHostnameTest */
}

void
dhcpLeasePoolGetByIpTest()
{
  /*  TODO dhcpLeasePoolGetByIpTest */
}