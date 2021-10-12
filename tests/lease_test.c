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
dhcpLeaseGetIpFromPoolTest()
{
  dhcpLeasePoolResult_t lease;

  bzero (&lease, sizeof (dhcpLeasePoolResult_t));

  lease = dhcpLeaseGetIpFromPool (FAKE_DATABASE_PATH);

  printf ("\n\nlease :\n\tid -> %d\n\tip -> %s\n\n", lease.id, lease.ip);

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
dhcpLeaseIpAddressTest()
{
  /* TODO */
}
