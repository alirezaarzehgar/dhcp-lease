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

  DHCP_LEASE_CHECK_FOR_INIT (db, DHCP_LEASE_COUNT_TO_ZERO);

  int
  callback (void *countPtr, int argc, char **argv, char **col)
  {
    int *count = (int *)countPtr;

    *count = atoi (argv[0]);

    return SQLITE_OK;
  }

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

  DHCP_LEASE_DECLARE_AS_NULL (dhcpLeasePoolResult_t, nullPool);

  char sql[MAX_QUERY_LEN];

  DHCP_LEASE_CHECK_FOR_INIT (db, nullPool);

  int
  callback (void *lease, int argc, char **argv, char **col)
  {
    dhcpLeasePoolResult_t *localLease = (dhcpLeasePoolResult_t *)lease;

    localLease->id = atoi (argv[0]);

    localLease->config = dhcpLeaseGetConfigById (atoi (argv[1]));

    strncpy (localLease->ip, argv[2], DHCP_LEASE_IP_STR_LEN);

    return SQLITE_OK;
  }

  dhcpLeaseSqlBuilderGetLeaseById (PoolTbl, sql, id);

  retval = sqlite3_exec (db, sql, callback, &pool, NULL);

  DHCP_LEASE_SQLITE_FAILURE (retval, nullPool);

  return pool;
}

dhcpLeaseConfigResult_t
dhcpLeaseGetConfigById (unsigned int id)
{
  int retval;

  dhcpLeaseConfigResult_t config;

  DHCP_LEASE_DECLARE_AS_NULL (dhcpLeaseConfigResult_t, nullConfig);

  char sql[MAX_QUERY_LEN];

  DHCP_LEASE_CHECK_FOR_INIT (db, nullConfig);

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

  bzero (&sql, sizeof (sql));

  dhcpLeaseSqlBuilderGetConfigById (ConfigTbl, PoolTbl, sql, id);

  retval = sqlite3_exec (db, sql, callback, &config, NULL);

  DHCP_LEASE_SQLITE_FAILURE (retval, nullConfig);

  return config;
}

dhcpLeasePoolResult_t
dhcpLeaseGetIpFromPool (char *mac)
{
  unsigned int retval;

  dhcpLeasePoolResult_t lease;

  DHCP_LEASE_DECLARE_AS_NULL (dhcpLeasePoolResult_t, nullLease);

  char sql[MAX_QUERY_LEN];

  DHCP_LEASE_CHECK_FOR_INIT (db, nullLease);

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

  if ((retval = dhcpLeaseMacAddressAlreadyExists (mac)) >
      DHCP_LEASE_COUNT_TO_ZERO)
    lease = dhcpLeaseGetPoolById (retval);
  else
    {
      dhcpLeaseSqlBuilderGetNonLeasedIp (PoolTbl, sql);

      retval = sqlite3_exec (db, sql, callback, &lease, NULL);

      DHCP_LEASE_SQLITE_FAILURE (retval, nullLease);
    }

  return lease;
}

bool
dhcpLeaseIpAddress (unsigned int id, const char *mac, const char *host)
{
  DHCP_LEASE_CHECK_FOR_INIT (db, false);

  int retval;

  char sql[MAX_QUERY_LEN];

  int flag = false;

  if (id == 0 || strlen (mac) == 0)
    return false;

  dhcpLeaseSqlBuilderLeaseIp (PoolTbl, sql, (char *)mac, (char *)host, id);

  retval = sqlite3_exec (db, sql, NULL, NULL, NULL);

  flag = retval == SQLITE_OK;

  return flag;
}

bool
dhcpLeaseInitHelper (bool (*callback) (void *, char *), void *tbl)
{
  int retval;

  char sql[MAX_QUERY_LEN];

  callback (tbl, sql);

  retavl = sqlite3_exec (db, sql, NULL, NULL, NULL);

  return retval == SQLITE_OK;
}

bool
dhcpLeaseInitPool()
{
  return dhcpLeaseInitHelper (dhcpLeaseSqlBuilderInitPoolTable, PoolTbl);
}

bool
dhcpLeaseInitConf()
{
  return dhcpLeaseInitHelper (dhcpLeaseSqlBuilderInitConfTable, configTbl);
}