#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "database.h"

#if !defined(LEASE_SQL_BUILDER_H)
#define LEASE_SQL_BUILDER_H

#define MAX_QUERY_LEN                500
#define L(var)                       strlen(var)

#define SQL_BUILDER(fmt, size, fprintfHandler)   \
  char format[] = fmt;   \
        \
  char *sql = (char *)malloc (      \
                size      \
              );      \
        \
  fprintfHandler;      \
      \
  memcpy (sqlPtr, sql, MAX_QUERY_LEN);      \
        \
  free (sql);

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

#endif // LEASE_SQL_BUILDER_H
