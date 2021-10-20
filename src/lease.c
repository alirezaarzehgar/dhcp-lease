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

    localLease->config = dhcpLeaseGetConfigByUserId (atoi (argv[1]));

    strncpy (localLease->ip, argv[2], DHCP_LEASE_IP_STR_LEN);

    return SQLITE_OK;
  }

  dhcpLeaseSqlBuilderGetLeaseById (PoolTbl, sql, id);

  retval = sqlite3_exec (db, sql, callback, &pool, NULL);

  DHCP_LEASE_SQLITE_FAILURE (retval, nullPool);

  return pool;
}

dhcpLeaseConfigResult_t
dhcpLeaseGetConfigByUserId (unsigned int id)
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

    localLease->config = dhcpLeaseGetConfigByUserId (id);

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

#define GET_POOL_BY_X_PATTERN(function, value)     \
  int retval;     \
      \
  DHCP_LEASE_DECLARE_AS_NULL(dhcpLeasePoolResult_t, lease);    \
    \
  char sql[MAX_QUERY_LEN];    \
      \
  DHCP_LEASE_DECLARE_AS_NULL (dhcpLeasePoolResult_t, nullLease);      \
      \
  DHCP_LEASE_CHECK_FOR_INIT (db, nullLease);      \
      \
  function (PoolTbl, sql, value);     \
      \
  retval = sqlite3_exec (db, sql, getLeaseCallback, &lease, NULL);    \
      \
  DHCP_LEASE_SQLITE_FAILURE (retval, nullLease);      \
      \
  return lease


int
getLeaseCallback (void *leasePtr, int argc, char **argv, char **col)
{
  dhcpLeasePoolResult_t *lease = (dhcpLeasePoolResult_t *)leasePtr;

  int id = atoi (argv[POOL_TBL_ID]);
  int conf_id = atoi (argv[POOL_TBL_CONF_ID]);
  int lease_flag = atoi (argv[POOL_TBL_LEASE_FLAG]);

  lease->id = id;
  lease->lease_flag = lease_flag;
  lease->config = dhcpLeaseGetConfigByUserId (conf_id);

  if (lease->lease_flag == 0)
    {
      bzero (&lease->host, sizeof (lease->host));

      strncpy (lease->ip, argv[POOL_TBL_IP], DHCP_LEASE_IP_STR_LEN);

      bzero (&lease->mac, sizeof (lease->mac));

      return SQLITE_OK;
    }

  strncpy (lease->host, argv[POOL_TBL_HOST], DHCP_LEASE_HOSTNAME_STR_MAX_LEN);
  strncpy (lease->ip, argv[POOL_TBL_IP], DHCP_LEASE_IP_STR_LEN);
  strncpy (lease->mac, argv[POOL_TBL_MAC], DHCP_LEASE_MAC_STR_MAX_LEN);

  return SQLITE_OK;
}

dhcpLeasePoolResult_t
dhcpLeasePoolGetById (int id)
{
  GET_POOL_BY_X_PATTERN (dhcpLeaseSqlBuilderPoolFindById, id);
}

dhcpLeasePoolResult_t
dhcpLeasePoolGetByMac (char *mac)
{
  GET_POOL_BY_X_PATTERN (dhcpLeaseSqlBuilderPoolFindByMac, mac);
}

dhcpLeasePoolResult_t
dhcpLeasePoolGetByHostname (char *hostname)
{
  GET_POOL_BY_X_PATTERN (dhcpLeaseSqlBuilderPoolFindByHostname, hostname);
}

dhcpLeasePoolResult_t
dhcpLeasePoolGetByIp (char *ip)
{
  GET_POOL_BY_X_PATTERN (dhcpLeaseSqlBuilderPoolFindByIp, ip);
}

int
dhcpLeaseXCount (void *tbl)
{
  int retval;

  char sql[MAX_QUERY_LEN];

  int count;

  dhcpLeaseSqlBuilderXCount (tbl, sql);

  int callback (void *count, int argc, char **argv, char **col)
  {
    int *c = (int *)count;

    if (argv[0] != NULL)
      *c = atoi (argv[0]);

    return SQLITE_OK;
  }

  retval = sqlite3_exec (db, sql, callback, &count, NULL);

  return count;
}

int
dhcpLeasePoolCount()
{
  return dhcpLeaseXCount ((void *)&PoolTbl);
}

int
dhcpLeaseConfCount()
{
  return dhcpLeaseXCount ((void *)&ConfigTbl);
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
dhcpLeaseInitPool()
{
  int retval;

  char sql[MAX_QUERY_LEN];

  dhcpLeaseSqlBuilderInitPoolTable (PoolTbl, sql);

  retval = sqlite3_exec (db, sql, NULL, NULL, NULL);

  return retval == SQLITE_OK;
}

bool
dhcpLeaseInitConf()
{
  int retval;

  char sql[MAX_QUERY_LEN];

  dhcpLeaseSqlBuilderInitConfTable (ConfigTbl, sql);

  retval = sqlite3_exec (db, sql, NULL, NULL, NULL);

  return retval == SQLITE_OK;
}

typedef void runAQueryCallback_t (void *tbl, char *sqlPtr, void *lease);

#define runAQuery(function, tbl, data)      \
  int retval;       \
        \
  char sql[MAX_QUERY_LEN];      \
        \
  DHCP_LEASE_CHECK_FOR_INIT (db, false);      \
        \
  function (tbl, sql, data);      \
        \
  retval = sqlite3_exec (db, sql, NULL, NULL, NULL);        \
        \
  return retval == SQLITE_OK;       \

bool
dhcpLeaseSaveNewLease (dhcpLeasePoolResult_t lease)
{
  runAQuery (dhcpLeaseSqlBuilderNewPool, PoolTbl, lease);
}

bool
dhcpLeaseUpdateLease (dhcpLeasePoolResult_t lease)
{
  runAQuery (dhcpLeaseSqlBuilderPoolUpdate, PoolTbl, lease);
}

bool
dhcpLeaseDeleteLeaseById (int id)
{
  runAQuery (dhcpLeaseSqlBuilderPoolDeleteById, PoolTbl, id);
}

bool
dhcpLeaseSaveNewConfig (dhcpLeaseConfigResult_t conf)
{
  runAQuery (dhcpLeaseSqlBuilderNewConf, ConfigTbl, conf);
}

bool
dhcpLeaseUpdateConfig (dhcpLeaseConfigResult_t conf)
{
  runAQuery (dhcpLeaseSqlBuilderConfUpdate, ConfigTbl, conf);
}

bool
dhcpLeaseDeleteConfigById (int id)
{
  runAQuery (dhcpLeaseSqlBuilderConfigDeleteById, ConfigTbl, id);
}
