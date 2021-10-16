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

/**
 * @@@@@@@@@@@   Important Note    @@@@@@@@@@@
 *
 * This code just running and is very very shit code.
 * We should refactor it and improve it's clean code.
 */

#include "lease/lease.h"
#include "lease/sql_builder.h"

static sqlite3 *db = NULL;

int
dhcpLeaseInit (const char *path)
{
  int retval;

  retval = sqlite3_open (path, &db);

  DHCP_LEASE_SQLITE_FAILURE (retval, false);

  return true;
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

  char sql[MAX_QUERY_LEN];

  int
  callback (void *countPtr, int argc, char **argv, char **col)
  {
    int *count = (int *)countPtr;

    *count = atoi (argv[0]);

    return SQLITE_OK;
  }

  if (db == NULL)
    return DHCP_LEASE_COUNT_TO_ZERO;

  dhcpLeaseSqlBuilderFindIdByMac (PoolTbl, sql, mac);

  retval = sqlite3_exec (db, sql, callback, &count, NULL);

  DHCP_LEASE_SQLITE_FAILURE (retval, DHCP_LEASE_COUNT_TO_ZERO);

  return count;
}

dhcpLeasePoolResult_t
dhcpLeaseGetPoolById (unsigned int id)
{
  int retval;

  dhcpLeasePoolResult_t pool;

  dhcpLeasePoolResult_t nullPool;

  bzero (&nullPool, sizeof (dhcpLeaseConfigResult_t));

  char sql[MAX_QUERY_LEN];

  int
  callback (void *lease, int argc, char **argv, char **col)
  {
    dhcpLeasePoolResult_t *localLease = (dhcpLeasePoolResult_t *)lease;

    localLease->id = atoi (argv[0]);

    localLease->config = dhcpLeaseGetConfigById (atoi (argv[1]));

    strncpy (localLease->ip, argv[2], DHCP_LEASE_IP_STR_LEN);

    return SQLITE_OK;
  }

  if (db == NULL)
    return nullPool;

  dhcpLeaseSqlBuilderGetLeaseById (PoolTbl, sql, id);

  retval = sqlite3_exec (db, sql, callback, &pool, NULL);

  if (retval != SQLITE_OK)
    return nullPool;

  return pool;
}

dhcpLeaseConfigResult_t
dhcpLeaseGetConfigById (unsigned int id)
{
  int retval;

  dhcpLeaseConfigResult_t config;

  char sql[MAX_QUERY_LEN];

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

  dhcpLeaseSqlBuilderGetConfigById (ConfigTbl, PoolTbl, sql, id);

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

  char sql[MAX_QUERY_LEN];

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

  if ((retval = dhcpLeaseMacAddressAlreadyExists (mac)) >
      DHCP_LEASE_COUNT_TO_ZERO)
    lease = dhcpLeaseGetPoolById (retval);
  else
    {
      dhcpLeaseSqlBuilderGetNonLeasedIp (PoolTbl, sql);

      retval = sqlite3_exec (db, sql, callback, &lease, NULL);

      if (retval != SQLITE_OK)
        bzero (&lease, sizeof (dhcpLeasePoolResult_t));
    }

  return lease;
}

bool
dhcpLeaseIpAddress (unsigned int id, const char *mac, const char *host)
{
  int retval;

  char sql[MAX_QUERY_LEN];

  int flag = false;

  if (db == NULL || id == 0 || strlen (mac) == 0)
    return false;

  dhcpLeaseSqlBuilderLeaseIp (PoolTbl, sql, (char *)mac, (char *)host, id);

  retval = sqlite3_exec (db, sql, NULL, NULL, NULL);

  flag = retval == SQLITE_OK;

  return flag;
}
