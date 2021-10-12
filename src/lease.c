/**
 * @file lease.c
 * @author alirezaarzehgar (alirezaarzehgar82@gmail.com)
 * @brief
 * @version 0.1
 * @date 2021-10-10
 *
 * @copyright Copyright (c) 2021
 *
 */

#include "lease/lease.h"

static sqlite3 *db = NULL;

int
dhcpLeaseInit (const char *path)
{
  int retval;

  retval = sqlite3_open (path, &db);

  if (retval != SQLITE_OK)
    return retval;
}

void
dhcpLeaseClose()
{
  sqlite3_close (db);
  db = NULL;
}

dhcpLeaseConfigResult_t
dhcpLeaseGetConfigById (unsigned int id)
{
  int retval;

  dhcpLeaseConfigResult_t config;

  char sql[strlen (DHCP_LEASE_GET_CONFIG_BY_ID_FORMAT_STRING) + 5];

  int
  callback (void *config, int argc, char **argv, char **col)
  {
    dhcpLeaseConfigResult_t *localConf = (dhcpLeaseConfigResult_t *)config;

    localConf->id = atoi (argv[0]);

    strncpy (localConf->mask, argv[1], DHCP_LEASE_SUBNET_STR_LEN);

    strncpy (localConf->router, argv[2], DHCP_LEASE_IP_STR_LEN);

    strncpy (localConf->domain, argv[3], DHCP_LEASE_DOMAIN_STR_MAX_LEN);

    localConf->lease_time = atoi (argv[4]);

    return SQLITE_OK;
  }

  bzero (&config, sizeof (dhcpLeaseConfigResult_t));

  bzero (&sql, sizeof (sql));

  if (db == NULL)
    return config;

  sprintf (sql, DHCP_LEASE_GET_CONFIG_BY_ID_FORMAT_STRING, id);

  retval = sqlite3_exec (db, sql, callback, &config, NULL);

  if (retval != SQLITE_OK)
    bzero (&config, sizeof (dhcpLeaseConfigResult_t));

  return config;
}

dhcpLeasePoolResult_t
dhcpLeaseGetIpFromPool()
{
  unsigned int retval;

  dhcpLeasePoolResult_t lease;

  int
  callback (void *lease, int argc, char **argv, char **col)
  {
    dhcpLeasePoolResult_t *localLease = (dhcpLeasePoolResult_t *)lease;

    unsigned int id;

    id = atoi (argv[1]);

    localLease->id = atoi (argv[0]);

    localLease->config = dhcpLeaseGetConfigById (id);

    strncpy (localLease->ip, argv[2], DHCP_LEASE_IP_STR_LEN);

    localLease->lease_flag = false;

    return SQLITE_OK;
  }

  bzero (&lease, sizeof (dhcpLeasePoolResult_t));

  if (db == NULL)
    return lease;

  retval = sqlite3_exec (db, DHCP_LEASE_GET_NON_RESERVED_IP, callback, &lease,
                         NULL);

  if (retval != SQLITE_OK)
    bzero (&lease, sizeof (dhcpLeasePoolResult_t));

  return lease;
}

bool
dhcpLeaseIpAddress (dhcpLeasePoolResult_t lease)
{
  int retval;

}
