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

dhcpLeasePoolResult_t
dhcpLeaseGetIpFromPool (const char *dbpath)
{
  sqlite3 *db;

  unsigned int retval;

  retval = sqlite3_open (dbpath, &db);


  sqlite3_close (db);
}

bool
dhcpLeaseIpAddress (dhcpLeasePoolResult_t lease)
{
  /* TODO */
}
