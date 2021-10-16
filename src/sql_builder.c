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

void
dhcpLeaseSqlBuilderGetNonLeasedIp (struct poolTbl tbl, char *sqlPtr)
{
  char format[] = "SELECT %s, %s, %s FROM %s WHERE %s = 0 LIMIT 1;";

  char *sql = (char *)malloc (
                L (format)
                + L (tbl.id)
                + L (tbl.conf_id)
                + L (tbl.ip)
                + L (tbl.name)
                + L (tbl.lease_flag)
              );

  sprintf (sql, format, tbl.id, tbl.conf_id, tbl.ip, tbl.name, tbl.lease_flag);

  memcpy (sqlPtr, sql, MAX_QUERY_LEN);

  free (sql);
}

void
dhcpLeaseSqlBuilderLeaseIp (struct poolTbl tbl, char *sqlPtr, char *mac,
                            char *host, int id)
{
  char format[] = "UPDATE %s SET %s = \"%s\", %s = %s, %s = 1 WHERE %s = %d;";

  /* +2 len for double qoutes for hostname ( "query" ) */
  char hostField[DHCP_LEASE_HOSTNAME_STR_MAX_LEN + 2];

  char *sql = (char *)malloc (
                L (format)
                + L (tbl.name)
                + L (tbl.mac)
                + DHCP_LEASE_MAC_STR_MAX_LEN + 2 /* +2 len for double qoutes ( "query" ) */
                + L (tbl.host) + 2 /* +2 len for double qoutes ( "query" ) */
                + DHCP_LEASE_HOSTNAME_STR_MAX_LEN
                + 2 /* +2 len for double qoutes for hostname ( "query" ) */
                + L (tbl.id)
                + 9   /* 9 digit id */
              );

  if (host == NULL)
    strcpy (hostField, "NULL");
  else
    sprintf (hostField, "\"%s\"", host);

  sprintf (sql, format, tbl.name, tbl.mac, mac, tbl.host, hostField,
           tbl.lease_flag, tbl.id, id);

  memcpy (sqlPtr, sql, MAX_QUERY_LEN);

  free (sql);
}

void
dhcpLeaseSqlBuilderInitConfTable (struct configTbl tbl, char *sqlPtr)
{
  char format[] = "CREATE TABLE %s (\n"
                  "   %s INTEGER PRIMARY KEY AUTOINCREMENT,\n"
                  "   %s TEXT NOT NULL,\n"
                  "   %s TEXT NOT NULL,\n"
                  "   %s TEXT NOT NULL,\n"
                  "   %s INTEGER NOT NULL DEFAULT 600\n"
                  ");\n\n";

  char *sql = malloc (
                L (format)
                + L (tbl.name)
                + L (tbl.id)
                + L (tbl.mask)
                + L (tbl.router)
                + L (tbl.domain)
                + L (tbl.lease_time)
              );

  sprintf (sql, format, tbl.name, tbl.id, tbl.mask, tbl.router, tbl.domain,
           tbl.lease_time);

  memcpy (sqlPtr, sql, MAX_QUERY_LEN);

  free (sql);
}

void
dhcpLeaseSqlBuilderInitPoolTable (struct poolTbl tbl, char *sqlPtr)
{
  char format[] = "CREATE TABLE %s (\n"
                  "   %s INTEGER PRIMARY KEY AUTOINCREMENT,\n"
                  "   %s INTEGER NOT NULL,\n"
                  "   %s TEXT NOT NULL,\n"
                  "   %s TEXT,\n"
                  "   %s TEXT,\n"
                  "   %s INTEGER NOT NULL DEFAULT 0,\n"
                  "   FOREIGN KEY (conf_id) REFERENCES config(id)\n"
                  ");\n\n";

  char *sql = malloc (
                L (format)
                + L (tbl.name)
                + L (tbl.id)
                + L (tbl.conf_id)
                + L (tbl.ip)
                + L (tbl.host)
                + L (tbl.mac)
                + L (tbl.lease_flag)
              );

  sprintf (sql, format, tbl.name, tbl.id, tbl.conf_id, tbl.ip, tbl.host, tbl.mac,
           tbl.lease_flag);

  memcpy (sqlPtr, sql, MAX_QUERY_LEN);

  free (sql);
}