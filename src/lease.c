/**
 * @file lease.c
 * @author alirezaarzehgar (alirezaarzehgar82@gmail.com)
 * @brief
 * @version 0.1.3
 * @date 2021-14-10
 *
 * @copyright Copyright (c) 2021
 *
 */

/**
 * @@@@@@@@@@@   Important Note    @@@@@@@@@@@
 *
 * This code just running and is very very shit code.
 * We should refactor it and improve it's clean code.
 */

#include "lease/lease.h"

static sqlite3 *db = NULL;

int
dhcpLeaseInit (const char *path)
{
  int retval;

  retval = sqlite3_open (path, &db);

  if (retval == SQLITE_OK)
    return retval;

  return SQLITE_ERROR;
}

void
dhcpLeaseClose()
{
  sqlite3_close (db);
  db = NULL;
}

int
dhcpLeaseMacAddressAlreadyExists (char *mac)
{
  int retval;

  int count = 0;

  char sql[160];

  int
  callback (void *countPtr, int argc, char **argv, char **col)
  {
    int *count = (int *)countPtr;

    *count = atoi (argv[0]);

    return SQLITE_OK;
  }

  if (db == NULL)
    return SQLITE_ERROR;

  sprintf (sql, DHCP_LEASE_FIND_ID_BY_MAC_FORMAT_STRING, mac);

  retval = sqlite3_exec (db, sql, callback, &count, NULL);

  if (retval != SQLITE_OK)
    return SQLITE_ERROR;

  return count;
}

dhcpLeasePoolResult_t
dhcpLeaseGetPoolById (unsigned int id)
{
  int retval;

  dhcpLeasePoolResult_t pool;

  char sql[strlen (DHCP_LEASE_GET_POOL_BY_ID_FORMAT_STRING) + 5];

  int
  callback (void *lease, int argc, char **argv, char **col)
  {
    dhcpLeasePoolResult_t *localLease = (dhcpLeasePoolResult_t *)lease;

    localLease->id = atoi (argv[0]);

    localLease->config = dhcpLeaseGetConfigById (atoi (argv[1]));

    strncpy (localLease->ip, argv[2], DHCP_LEASE_IP_STR_LEN);

    return SQLITE_OK;
  }

  bzero (&pool, sizeof (dhcpLeaseConfigResult_t));

  bzero (&sql, sizeof (sql));

  if (db == NULL)
    return SQLITE_ERROR;

  sprintf (sql, DHCP_LEASE_GET_POOL_BY_ID_FORMAT_STRING, id);

  retval = sqlite3_exec (db, sql, callback, &pool, NULL);

  if (retval != SQLITE_OK)
    bzero (&pool, sizeof (dhcpLeasePoolResult_t));

  return pool;
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
    return SQLITE_ERROR;

  sprintf (sql, DHCP_LEASE_GET_CONFIG_BY_ID_FORMAT_STRING, id);

  retval = sqlite3_exec (db, sql, callback, &config, NULL);

  if (retval != SQLITE_OK)
    bzero (&config, sizeof (dhcpLeaseConfigResult_t));

  return config;
}

dhcpLeasePoolResult_t
dhcpLeaseGetIpFromPool (char *mac)
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
    return SQLITE_ERROR;

  if ((retval = dhcpLeaseMacAddressAlreadyExists (mac)) > 0)
    lease = dhcpLeaseGetPoolById (retval);
  else
    {
      retval = sqlite3_exec (db, DHCP_LEASE_GET_NON_RESERVED_IP, callback, &lease,
                             NULL);
      if (retval != SQLITE_OK)
        bzero (&lease, sizeof (dhcpLeasePoolResult_t));
    }

  return lease;
}

bool
dhcpLeaseIpAddress (unsigned int id, const char *mac, const char *host)
{
  int retval;

  char sql[strlen (DHCP_LEASE_RESERVE_ADDRESS_FORMAT_STRING) +
                  DHCP_LEASE_MAC_STR_MAX_LEN + DHCP_LEASE_HOSTNAME_STR_MAX_LEN];

  char hostField[DHCP_LEASE_HOSTNAME_STR_MAX_LEN + 2];

  int flag = false;

  if (db == NULL || id == 0 || strlen (mac) == 0)
    return false;

  if (host == NULL)
    strcpy (hostField, "NULL");
  else
    sprintf (hostField, "\"%s\"", host);

  sprintf (sql, DHCP_LEASE_RESERVE_ADDRESS_FORMAT_STRING, mac, hostField, id);

  retval = sqlite3_exec (db, sql, NULL, NULL, NULL);

  flag = retval == SQLITE_OK;

  return flag;
}
