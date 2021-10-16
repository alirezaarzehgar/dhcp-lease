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

void
dhcpLeaseSqlBuilderFindIdByMac (struct poolTbl tbl, char *sqlPtr, char *mac)
{
  char format[] = "SELECT %s FROM %s WHERE %s = \"%s\";";

  char *sql = (char *)malloc (
                L (format)
                + L (tbl.id)
                + L (tbl.name)
                + L (tbl.mac)
              );

  sprintf (sql, format, tbl.id, tbl.name, tbl.mac, mac);

  memcpy (sqlPtr, sql, MAX_QUERY_LEN);

  free (sql);
}

void
dhcpLeaseSqlBuilderGetLeaseById (struct poolTbl tbl, char *sqlPtr, int id)
{
  char format[] = "SELECT  %s, %s, %s FROM %s WHERE %s = %d";

  char *sql = (char *)malloc (
                L (format)
                + L (tbl.id)
                + L (tbl.conf_id)
                + L (tbl.ip)
                + L (tbl.name)
                + L (tbl.id)
                + 9   /* 9 digit id */
              );

  sprintf (sql, format, tbl.id, tbl.conf_id, tbl.ip, tbl.name, tbl.id, id);

  memcpy (sqlPtr, sql, MAX_QUERY_LEN);

  free (sql);
}

void
dhcpLeaseSqlBuilderGetConfigById (struct configTbl ctbl, struct poolTbl ptbl,
                                  char *sqlPtr, int id)
{
  char format[] = "SELECT %s, %s, %s, %s, %s FROM %s WHERE %s = "
                  "(SELECT %s FROM %s WHERE %s = %d);";

  char *sql = (char *)malloc (
                L (format)
                + L (ctbl.id)
                + L (ctbl.mask)
                + L (ctbl.router)
                + L (ctbl.domain)
                + L (ctbl.lease_time)
                + L (ctbl.name)
                + L (ctbl.id)
                + L (ptbl.id)
                + L (ptbl.name)
                + L (ptbl.id)
                + 9   /* 9 digit id */
              );

  sprintf (sql, format, ctbl.id, ctbl.mask, ctbl.router, ctbl.domain,
           ctbl.lease_time, ctbl.name, ctbl.id, ptbl.id, ptbl.name, ptbl.id, id);

  memcpy (sqlPtr, sql, MAX_QUERY_LEN);

  free (sql);
}
