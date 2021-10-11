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

dhcpLeaseConfigResult_t
dhcpLeaseGetConfigById (sqlite3 *db, unsigned int id)
{

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

  int retval;

  dhcpLeaseConfigResult_t config;

  char *errMsg = malloc (12);

  char sql[strlen (DHCP_LEASE_GET_CONFIG_BY_ID_FORMAT_STRING) + 5];

  bzero (&config, sizeof (dhcpLeaseConfigResult_t));

  bzero (&sql, sizeof (sql));

  sprintf (sql, DHCP_LEASE_GET_CONFIG_BY_ID_FORMAT_STRING, id);

  retval = sqlite3_exec (db, sql, callback, &config, &errMsg);

  if (retval != SQLITE_OK)
    bzero (&config, sizeof (dhcpLeaseConfigResult_t));

  return config;
}

dhcpLeasePoolResult_t
dhcpLeaseGetIpFromPool (const char *dbpath)
{
  sqlite3 *db;

  unsigned int retval;

  dhcpLeasePoolResult_t lease;

  int
  callback (void *lease, int argc, char **argv, char **col)
  {
    dhcpLeasePoolResult_t *localLease = (dhcpLeasePoolResult_t *)lease;

    unsigned int id;

    id = atoi (argv[1]);

    localLease->id = atoi (argv[0]);

    localLease->config = dhcpLeaseGetConfigById (db, id);

    strncpy (localLease->ip, argv[2], DHCP_LEASE_IP_STR_LEN);

    localLease->lease_flag = false;

    return SQLITE_OK;
  }

  bzero (&lease, sizeof (dhcpLeasePoolResult_t));

  retval = sqlite3_open (dbpath, &db);

  if (retval != SQLITE_OK)
    return lease;

  retval = sqlite3_exec (db, DHCP_LEASE_GET_NON_RESERVED_IP, callback, &lease,
                         NULL);

  if (retval != SQLITE_OK)
    bzero (&lease, sizeof (dhcpLeasePoolResult_t));

  sqlite3_close (db);

  return lease;
}

bool
dhcpLeaseIpAddress (dhcpLeasePoolResult_t lease)
{
  /* TODO */
}
