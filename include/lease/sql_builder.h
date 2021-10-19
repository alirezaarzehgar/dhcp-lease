#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "database.h"

#if !defined(LEASE_SQL_BUILDER_H)
#define LEASE_SQL_BUILDER_H

#define MAX_QUERY_LEN                1000
#define L(var)                       (strlen(var) + (1))

/* POOL_PARAM_{param1}_{param2}_{param-n} */
#define POOL_PARAM_TBL_SQLPTR      struct poolTbl tbl, char *sqlPtr

void dhcpLeaseSqlBuilderFindIdByMac (struct poolTbl tbl, char *sqlPtr,
                                     char *mac);

void dhcpLeaseSqlBuilderGetLeaseById (struct poolTbl tbl, char *sqlPtr,
                                      int id);

void dhcpLeaseSqlBuilderGetConfigById (struct configTbl ctbl,
                                       struct poolTbl ptbl, char *sqlPtr, int id);

void dhcpLeaseSqlBuilderGetNonLeasedIp (struct poolTbl tbl, char *sqlPtr);

void dhcpLeaseSqlBuilderLeaseIp (struct poolTbl tbl, char *sqlPtr, char *mac,
                                 char *host, int id);

void dhcpLeaseSqlBuilderInitConfTable (struct configTbl tbl, char *sqlPtr);

void dhcpLeaseSqlBuilderInitPoolTable (struct poolTbl tbl, char *sqlPtr);

void dhcpLeaseSqlBuilderPoolFindByX (POOL_PARAM_TBL_SQLPTR, char *condition);

void dhcpLeaseSqlBuilderPoolFindById (POOL_PARAM_TBL_SQLPTR, int id);

void dhcpLeaseSqlBuilderPoolFindByMac (POOL_PARAM_TBL_SQLPTR, char *mac);

void dhcpLeaseSqlBuilderPoolFindByHostname (POOL_PARAM_TBL_SQLPTR,
    char *hostname);

void dhcpLeaseSqlBuilderPoolFindByIp (POOL_PARAM_TBL_SQLPTR, char *ip);

#endif // LEASE_SQL_BUILDER_H
