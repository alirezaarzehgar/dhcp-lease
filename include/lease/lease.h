#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <sqlite3.h>
#include <errno.h>
#include <string.h>

#include "database.h"

#if !defined(LEASE_H)
#define LEASE_H

#define DHCP_LEASE_SUBNET_STR_LEN               16
#define DHCP_LEASE_IP_STR_LEN                   16
#define DHCP_LEASE_DOMAIN_STR_MAX_LEN           255     /* RFC 2821 */
#define DHCP_LEASE_HOSTNAME_STR_MAX_LEN         253     /* en.wikipedia.org/wiki/Hostname */
#define DHCP_LEASE_MAC_STR_MAX_LEN              17

#define DHCP_LEASE_COUNT_TO_ZERO                0

#define DHCP_LEASE_SQLITE_FAILURE(retval, valueForReturn) do { \
    if (retval != SQLITE_OK)  \
    return valueForReturn;  \
} while(0);

#define DHCP_LEASE_DECLARE_AS_NULL(type, name)  \
  type    name;   \
  bzero (&name, sizeof (type))

#define DHCP_LEASE_CHECK_FOR_INIT(db, valueForReturn)   \
  if (db == NULL)   \
    return valueForReturn;

enum dhcpLeaseStandarPoolTbl
{
  POOL_TBL_ID,
  POOL_TBL_CONF_ID,
  POOL_TBL_IP,
  POOL_TBL_HOST,
  POOL_TBL_MAC,
  POOL_TBL_LEASE_FLAG,
};

typedef struct
{
  unsigned int id;

  char mask[DHCP_LEASE_SUBNET_STR_LEN];

  char router[DHCP_LEASE_IP_STR_LEN];

  char domain[DHCP_LEASE_DOMAIN_STR_MAX_LEN];

  unsigned int lease_time;

} dhcpLeaseConfigResult_t;

typedef struct
{
  unsigned int id;

  char ip[DHCP_LEASE_IP_STR_LEN];

  char host[DHCP_LEASE_HOSTNAME_STR_MAX_LEN];

  char mac[DHCP_LEASE_MAC_STR_MAX_LEN];

  bool lease_flag;

  dhcpLeaseConfigResult_t config;

} dhcpLeasePoolResult_t;


typedef struct
{
  char name[8];
  char id[3];
  char pad[4];
} commonTbl_t;

int dhcpLeaseInit (const char *path);

void dhcpLeaseClose();

int dhcpLeaseMacAddressAlreadyExists (char *mac);

dhcpLeaseConfigResult_t dhcpLeaseGetConfigById (unsigned int id);

dhcpLeasePoolResult_t dhcpLeaseGetIpFromPool (char *mac);

dhcpLeasePoolResult_t dhcpLeasePoolGetById (int id);

dhcpLeasePoolResult_t dhcpLeasePoolGetByMac (char *mac);

dhcpLeasePoolResult_t dhcpLeasePoolGetByHostname (char *hostname);

dhcpLeasePoolResult_t dhcpLeasePoolGetByIp (char *ip);

bool dhcpLeaseIpAddress (unsigned int id, const char *mac, const char *host);

bool dhcpLeaseInitPool();

bool dhcpLeaseInitConf();

int dhcpLeasePoolCount();

int dhcpLeaseConfCount();

#endif // LEASE_H
