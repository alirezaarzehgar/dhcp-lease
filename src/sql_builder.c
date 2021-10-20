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

/**
 * TODO       All of query builders should hanve validator
 */

#include "lease/lease.h"
#include "lease/sql_builder.h"

void
dhcpLeaseSqlBuilderFindIdByMac (POOL_PARAM_TBL_SQLPTR, char *mac)
{
  sprintf (sqlPtr, "SELECT %s FROM %s WHERE %s = \"%s\";", tbl.id, tbl.name,
           tbl.mac, mac);

}

void
dhcpLeaseSqlBuilderGetLeaseById (POOL_PARAM_TBL_SQLPTR, int id)
{
  sprintf (sqlPtr,  "SELECT  %s, %s, %s FROM %s WHERE %s = %d;\n\n", tbl.id,
           tbl.conf_id, tbl.ip, tbl.name, tbl.id, id);
}

void
dhcpLeaseSqlBuilderGetConfigById (struct configTbl ctbl, struct poolTbl ptbl,
                                  char *sqlPtr, int id)
{
  sprintf (sqlPtr,
           "SELECT %s, %s, %s, %s, %s FROM %s WHERE %s = (SELECT %s FROM %s WHERE %s = %d);",
           ctbl.id, ctbl.mask, ctbl.router, ctbl.domain, ctbl.lease_time, ctbl.name,
           ctbl.id, ptbl.id, ptbl.name, ptbl.id, id);
}

void
dhcpLeaseSqlBuilderGetNonLeasedIp (POOL_PARAM_TBL_SQLPTR)
{
  sprintf (sqlPtr, "SELECT %s, %s, %s FROM %s WHERE %s = 0 LIMIT 1;", tbl.id,
           tbl.conf_id, tbl.ip, tbl.name, tbl.lease_flag);
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

  sprintf (sqlPtr, "UPDATE %s SET %s = \"%s\", %s = %s, %s = 1 WHERE %s = %d;",
           tbl.name, tbl.mac, mac, tbl.host, hostField, tbl.lease_flag, tbl.id, id);
}

void
dhcpLeaseSqlBuilderInitConfTable (struct configTbl tbl, char *sqlPtr)
{
  sprintf (sqlPtr, "CREATE TABLE %s (\n"
           "   %s INTEGER PRIMARY KEY AUTOINCREMENT,\n" "   %s TEXT NOT NULL,\n"
           "   %s TEXT NOT NULL,\n" "   %s TEXT NOT NULL,\n"
           "   %s INTEGER NOT NULL DEFAULT 600\n" ");\n\n", tbl.name, tbl.id, tbl.mask,
           tbl.router, tbl.domain, tbl.lease_time);
}

void
dhcpLeaseSqlBuilderInitPoolTable (POOL_PARAM_TBL_SQLPTR)
{
  sprintf (sqlPtr, "CREATE TABLE %s (\n"
           "   %s INTEGER PRIMARY KEY AUTOINCREMENT,\n" "   %s INTEGER NOT NULL,\n"
           "   %s TEXT NOT NULL,\n" "   %s TEXT,\n" "   %s TEXT,\n"
           "   %s INTEGER NOT NULL DEFAULT 0,\n"
           "   FOREIGN KEY (conf_id) REFERENCES config(id)\n" ");\n\n", tbl.name, tbl.id,
           tbl.conf_id, tbl.ip, tbl.host, tbl.mac, tbl.lease_flag);
}

void
dhcpLeaseSqlBuilderPoolFindByX (POOL_PARAM_TBL_SQLPTR, char *condition)
{
  sprintf (sqlPtr, "SELECT * FROM %s WHERE %s;\n\n", tbl.name, condition);
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

void
dhcpLeaseSqlBuilderXCount (void *tbl, char *sqlPtr)
{
  sprintf (sqlPtr, "SELECT count() FROM %s;", ((commonTbl_t *)tbl)->name);
}


void
dhcpLeaseSqlBuilderPoolUpdate (POOL_PARAM_TBL_SQLPTR,
                               dhcpLeasePoolResult_t lease)
{
  sprintf (sqlPtr,
           "UPDATE %s "        /* UPDATE name  */
           "SET %s = %d,"      /* SET conf_id = id */
           " %s = \"%s\","     /* ip = "ip" */
           " %s = \"%s\","     /* host = "host" */
           " %s = \"%s\","     /* mac = "mac" */
           " %s = %d "         /* lease_flag = 1 */
           "WHERE %s = %d;"   /* WHERE id = id; */
           , tbl.name, tbl.conf_id, lease.config.id, tbl.ip, lease.ip, tbl.host,
           lease.host, tbl.mac, lease.mac, tbl.lease_flag, lease.lease_flag, tbl.id,
           lease.id);
}

void
dhcpLeaseSqlBuilderConfUpdate (CONFIG_PARAM_TBL_SQLPTR,
                               dhcpLeaseConfigResult_t conf)
{
  sprintf (sqlPtr,
           "UPDATE %s "          /* UPDATE name */
           "SET %s = \"%s\", "   /* mask = "mask" */
           " %s = \"%s\", "      /* router = "router" */
           " %s = \"%s\", "      /* domain = "domain" */
           " %s = %d "           /* lease_time = lease_time */
           "WHERE %s = %d;"      /* WHERE id = id*/
           , tbl.name, tbl.mask, conf.mask, tbl.router, conf.router, tbl.domain,
           conf.domain, tbl.lease_time, conf.lease_time, tbl.id, conf.id);
}

void
dhcpLeaseSqlBuilderNewPool (POOL_PARAM_TBL_SQLPTR,
                            dhcpLeasePoolResult_t lease)
{
  sprintf (sqlPtr,
           "INSERT INTO %s "   /* INSERT INTO */
           "("
           "%s, "             /* conf_id */
           "%s, "             /* ip */
           "%s, "             /* host */
           "%s, "             /* mac */
           "%s"               /* lease_flag */
           ")"
           " VALUES "
           "("
           "%d, "          /* id */
           "\"%s\", "      /* "ip" */
           "\"%s\", "      /* "host" */
           "\"%s\", "      /* "mac" */
           "%d"            /* lease_flag */
           ");"
           , tbl.name, tbl.conf_id, tbl.ip, tbl.host, tbl.mac, tbl.lease_flag,
           lease.config.id, lease.ip, lease.host, lease.mac, lease.lease_flag, lease.id);
}

void
dhcpLeaseSqlBuilderNewConf (CONFIG_PARAM_TBL_SQLPTR,
                            dhcpLeaseConfigResult_t conf)
{
  sprintf (sqlPtr,
           "INSERT INTO %s "   /* INSERT INTO */
           "("
           "%s, "             /* mask */
           "%s, "             /* router */
           "%s, "             /* domain */
           "%s"             /* lease_time */
           ")"
           " VALUES "
           "("
           "\"%s\", "      /* mask" */
           "\"%s\", "      /* router" */
           "\"%s\", "      /* domain" */
           "%d"           /* lease_time */
           ");"
           , tbl.name, tbl.mask, tbl.router, tbl.domain, tbl.lease_time, conf.mask,
           conf.router, conf.domain, conf.lease_time);
}

#define BASE_DELETE_FORMAT           "DELETE FROM %s WHERE %s = %d;"

void
dhcpLeaseSqlBuilderPoolDeleteById (POOL_PARAM_TBL_SQLPTR, int id)
{
  sprintf (sqlPtr, BASE_DELETE_FORMAT, tbl.name, tbl.id, id);
}

void
dhcpLeaseSqlBuilderConfigDeleteById (CONFIG_PARAM_TBL_SQLPTR,
                                     int id)
{
  sprintf (sqlPtr, BASE_DELETE_FORMAT, tbl.name, tbl.id, id);
}