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

#define DHCP_LEASE_GET_NON_RESERVED_IP      \
    "SELECT "     \
          LEASE_POOL_ID ", "      \
          LEASE_POOL_CONFIG_ID ", "     \
          LEASE_POOL_IP " "     \
    "FROM " LEASE_POOL_TABLE_NAME " WHERE " LEASE_POOL_LEASE_FLAG " = 0 LIMIT 1;"

#define DHCP_LEASE_RESERVE_ADDRESS_FORMAT_STRING      \
    "UPDATE "       \
    LEASE_POOL_TABLE_NAME " "   \
    "SET "          \
    LEASE_POOL_MAC " = \"%s\", "    \
    LEASE_POOL_HOST " = %s, "    \
    LEASE_POOL_LEASE_FLAG " = 1 "     \
    "WHERE " LEASE_POOL_ID " = %d;"

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

int dhcpLeaseInit (const char *path);

void dhcpLeaseClose();

int dhcpLeaseMacAddressAlreadyExists (char *mac);

dhcpLeaseConfigResult_t dhcpLeaseGetConfigById (unsigned int id);

dhcpLeasePoolResult_t dhcpLeaseGetIpFromPool (char *mac);

bool dhcpLeaseIpAddress (unsigned int id, const char *mac, const char *host);

#endif // LEASE_H
