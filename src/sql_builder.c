/**
 * @file sql_builder.c
 * @author alirezaarzehgar (alirezaarzehgar82@gmail.com)
 * @brief
 * @version 0.1
 * @date 2021-10-16
 *
 * @copyright Copyright (c) 2021
 *
 */

#include "lease/lease.h"
#include "lease/sql_builder.h"

#define SQL_BUILDER(fmt, fprintfHandler)   \
  char format[] = fmt;   \
        \
  fprintfHandler;

void
dhcpLeaseSqlBuilderFindIdByMac (POOL_PARAM_TBL_SQLPTR, char *mac)
{
  SQL_BUILDER (
    "SELECT %s FROM %s WHERE %s = \"%s\";",
  {
    sprintf (sqlPtr, format, tbl.id, tbl.name, tbl.mac, mac);
  });
}

void
dhcpLeaseSqlBuilderGetLeaseById (POOL_PARAM_TBL_SQLPTR, int id)
{
  SQL_BUILDER (
  {
    sprintf (sqlPtr, format, tbl.id, tbl.conf_id, tbl.ip, tbl.name, tbl.id, id);
  });
}

void
dhcpLeaseSqlBuilderGetConfigById (struct configTbl ctbl, struct poolTbl ptbl,
                                  char *sqlPtr, int id)
{
  SQL_BUILDER (
    "SELECT %s, %s, %s, %s, %s FROM %s WHERE %s = "
    "(SELECT %s FROM %s WHERE %s = %d);",
  {
    sprintf (sqlPtr, format, ctbl.id, ctbl.mask, ctbl.router, ctbl.domain,
             ctbl.lease_time, ctbl.name, ctbl.id, ptbl.id, ptbl.name, ptbl.id, id);
  });
}

void
dhcpLeaseSqlBuilderGetNonLeasedIp (POOL_PARAM_TBL_SQLPTR)
{
  SQL_BUILDER (
    "SELECT %s, %s, %s FROM %s WHERE %s = 0 LIMIT 1;",
  {
    sprintf (sqlPtr, format, tbl.id, tbl.conf_id, tbl.ip, tbl.name, tbl.lease_flag);
  });
}

void
dhcpLeaseSqlBuilderLeaseIp (POOL_PARAM_TBL_SQLPTR, char *mac, char *host,
                            int id)
{
  /* +2 len for double qoutes for hostname ( "query" ) */
  char hostField[DHCP_LEASE_HOSTNAME_STR_MAX_LEN + 2];

  if (host == NULL)
    strcpy (hostField, "NULL");
  else
    sprintf (hostField, "\"%s\"", host);

  SQL_BUILDER (
    "UPDATE %s SET %s = \"%s\", %s = %s, %s = 1 WHERE %s = %d;",
  {
    sprintf (sqlPtr, format, tbl.name, tbl.mac, mac, tbl.host, hostField,
             tbl.lease_flag, tbl.id, id);
  });
}

void
dhcpLeaseSqlBuilderInitConfTable (struct configTbl tbl, char *sqlPtr)
{
  SQL_BUILDER (
    "CREATE TABLE %s (\n"
    "   %s INTEGER PRIMARY KEY AUTOINCREMENT,\n"
    "   %s TEXT NOT NULL,\n"
    "   %s TEXT NOT NULL,\n"
    "   %s TEXT NOT NULL,\n"
    "   %s INTEGER NOT NULL DEFAULT 600\n"
    ");\n\n",
  {
    sprintf (sqlPtr, format, tbl.name, tbl.id, tbl.mask, tbl.router, tbl.domain,
             tbl.lease_time);
  });
}

void
dhcpLeaseSqlBuilderInitPoolTable (POOL_PARAM_TBL_SQLPTR)
{
  SQL_BUILDER (
    "CREATE TABLE %s (\n"
    "   %s INTEGER PRIMARY KEY AUTOINCREMENT,\n"
    "   %s INTEGER NOT NULL,\n"
    "   %s TEXT NOT NULL,\n"
    "   %s TEXT,\n"
    "   %s TEXT,\n"
    "   %s INTEGER NOT NULL DEFAULT 0,\n"
    "   FOREIGN KEY (conf_id) REFERENCES config(id)\n"
    ");\n\n",
  {
    sprintf (sqlPtr, format, tbl.name, tbl.id, tbl.conf_id, tbl.ip, tbl.host, tbl.mac,
             tbl.lease_flag);
  });
}

void
dhcpLeaseSqlBuilderPoolFindByX (POOL_PARAM_TBL_SQLPTR, char *condition)
{
  SQL_BUILDER (
    "SELECT * FROM %s WHERE %s;\n\n",
  {
    sprintf (sqlPtr, format, tbl.name, condition);
  });
}

#define POOL_FIND_BY_X(fmt, filed, X, size)   \
  char format[L (fmt) + L (tbl.filed)];   \
        \
  sprintf (format, fmt, tbl.filed, X);      \
        \
  dhcpLeaseSqlBuilderPoolFindByX (tbl, sqlPtr, format);   \

void
dhcpLeaseSqlBuilderPoolFindById (POOL_PARAM_TBL_SQLPTR, int id)
{
  POOL_FIND_BY_X (
    "%s = %d",
    id,
    id,
    9   /* 9 digit id */
  );
}

void
dhcpLeaseSqlBuilderPoolFindByMac (POOL_PARAM_TBL_SQLPTR, char *mac)
{
  POOL_FIND_BY_X (
    "%s = \"%s\"",
    mac, mac,
    L (mac)
  );
}

void
dhcpLeaseSqlBuilderPoolFindByHostname (POOL_PARAM_TBL_SQLPTR, char *hostname)
{
  POOL_FIND_BY_X (
    "%s = \"%s\"",
    host, hostname,
    L (hostname)
  )
}

void
dhcpLeaseSqlBuilderPoolFindByIp (POOL_PARAM_TBL_SQLPTR, char *ip)
{
  POOL_FIND_BY_X (
    "%s = \"%s\"",
    ip, ip,
    L (ip)
  );
}
