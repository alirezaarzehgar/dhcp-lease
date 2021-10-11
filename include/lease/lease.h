#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <sqlite3.h>

#include "database.h"

#if !defined(LEASE_H)
#define LEASE_H

#define DHCP_LEASE_SUBNET_STR_LEN               16
#define DHCP_LEASE_IP_STR_LEN                   16
#define DHCP_LEASE_DOMAIN_STR_MAX_LEN           255     /* RFC 2821 */
#define DHCP_LEASE_HOSTNAME_STR_MAX_LEN         253     /* en.wikipedia.org/wiki/Hostname */
#define DHCP_LEASE_MAC_STR_MAX_LEN              17

#define DHCP_LEASE_GET_NON_RESERVED_IP          \
    "SELECT " \
          LEASE_POOL_ID ", " \
          LEASE_POOL_CONFIG_ID ", " \
          LEASE_POOL_IP ", " \
          LEASE_POOL_HOST ", " \
          LEASE_POOL_MAC ", " \
          LEASE_POOL_LEASE_FLAG ", " \
    "FROM " LEASE_POOL_TABLE_NAME " WHERE " LEASE_POOL_LEASE_FLAG " = 0 LIMIT 1;"

typedef struct
{
  u_int16_t id;

  char mask[DHCP_LEASE_SUBNET_STR_LEN];

  char router[DHCP_LEASE_IP_STR_LEN];

  char domain[DHCP_LEASE_DOMAIN_STR_MAX_LEN];

  u_int32_t lease_time;

} dhcpLeaseConfigResult_t;

typedef struct
{
  u_int16_t id;

  char ip[DHCP_LEASE_IP_STR_LEN];

  char host[DHCP_LEASE_HOSTNAME_STR_MAX_LEN];

  char mac[DHCP_LEASE_MAC_STR_MAX_LEN];

  bool lease_flag;

  dhcpLeaseConfigResult_t config;

} dhcpLeasePoolResult_t;

dhcpLeasePoolResult_t dhcpLeaseGetIpFromPool (const char *dbpath);

bool dhcpLeaseIpAddress (dhcpLeasePoolResult_t lease);

#endif // LEASE_H
