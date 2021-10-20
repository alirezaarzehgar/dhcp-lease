/**
 * @file lease_test.c
 * @author alirezaarzehgar (alirezaarzehgar82@gmail.com)
 * @brief Incomplete and wrong test
 * @version 0.1
 * @date 2021-10-11
 *
 * @copyright Copyright (c) 2021
 *
 */

/**
 *              TODO We should refactor this tests and improve cases
 */

#include "lease_test.h"
#include "lease/sql_builder.h"
#include <CUnit/CUnit.h>
#include <time.h>

#define CLIENT_MAC_ADDERSS  "d4:f2:76:d0:a4:0f"
#define CLIENT_HOSTNAME     "ali"
#define MAX_LEASE           200

#define CODE_BEHINDE_INITCLOSE_DB(TODO)      \
do {      \
  dhcpLeaseInit (FAKE_DATABASE_PATH);     \
      \
  { TODO }      \
        \
  dhcpLeaseClose();     \
} while(0)

int
initSuiteLease()
{
  srand (time (NULL));
  return 0;
}

int
cleanupSuiteLease()
{
  return 0;
}

void
printLease (dhcpLeasePoolResult_t lease)
{
  printf (
    "\n\n"
    "lease :\n"
    "\tid -> %d\n"
    "\tip -> %s\n"
    "\thost -> %s\n"
    "\tmac -> %s\n\n", lease.id, lease.ip, lease.host, lease.mac);

  printf ("configs : \n"
          "\tid -> %d\n"
          "\tmask -> %s\n"
          "\trouter -> %s\n"
          "\tdomain -> %s \n"
          "\tlease_time -> %d\n", lease.config.id, lease.config.mask,
          lease.config.router,
          lease.config.domain, lease.config.lease_time);

  printf ("\n\n");
}

void
dhcpLeaseGetIpFromPoolTest()
{
  CODE_BEHINDE_INITCLOSE_DB (
  {
    dhcpLeasePoolResult_t lease;

    bzero (&lease, sizeof (dhcpLeasePoolResult_t));

    lease = dhcpLeaseGetIpFromPool (CLIENT_MAC_ADDERSS);

    printLease (lease);
  });
}

void
dhcpLeaseIpAddressTest()
{
  CODE_BEHINDE_INITCLOSE_DB (
  {
    dhcpLeasePoolResult_t lease;

    int retval;

    lease = dhcpLeaseGetIpFromPool (CLIENT_MAC_ADDERSS);

    retval = dhcpLeaseIpAddress (lease.id, CLIENT_MAC_ADDERSS, CLIENT_HOSTNAME);

    CU_ASSERT_TRUE (retval);
  });
}

void
dhcpLeaseInitConfTest()
{
  dhcpLeaseInit ("test.db");
  dhcpLeaseInitConf();
}

void
dhcpLeaseInitPoolTest()
{
  dhcpLeaseInitPool();
  dhcpLeaseClose();
}

void
dhcpLeaseInitTest()
{
  /*  TODO dhcpLeaseInitTest */
}

void
dhcpLeaseCloseTest()
{
  /*  TODO dhcpLeaseCloseTest */
}

void
dhcpLeaseMacAddressAlreadyExistsTest()
{
  /*  TODO dhcpLeaseMacAddressAlreadyExistsTest */
}

void
dhcpLeasePoolGetByIdTest()
{
  BASE_GET_BY_X (dhcpLeasePoolGetById, rand() % MAX_LEASE);
}

void
dhcpLeasePoolGetByMacTest()
{
  BASE_GET_BY_X (dhcpLeasePoolGetByMac, CLIENT_MAC_ADDERSS);
}

void
dhcpLeasePoolGetByHostnameTest()
{
  BASE_GET_BY_X (dhcpLeasePoolGetByHostname, CLIENT_HOSTNAME);
}

void
dhcpLeasePoolGetByIpTest()
{
  BASE_GET_BY_X (dhcpLeasePoolGetByIp, "192.168.133.6");
}

void
dhcpLeasePoolCountTest()
{
  CODE_BEHINDE_INITCLOSE_DB (
  {
    CU_ASSERT_EQUAL (dhcpLeasePoolCount(), MAX_LEASE);
  });
}

void
dhcpLeaseConfCountTest()
{
  CODE_BEHINDE_INITCLOSE_DB (
  {
    CU_ASSERT_EQUAL (dhcpLeaseConfCount(), 5);
  });
}


void
dhcpLeaseSqlBuilderPoolUpdateTest()
{
  dhcpLeasePoolResult_t lease =
  {
    .id = 1, .mac = "some_mac",
    .host = "ali", .lease_flag = 1,
    .ip = "ip", .config = {
      .id = 1
    }
  };

  char sql[MAX_QUERY_LEN];

  dhcpLeaseSqlBuilderPoolUpdate (PoolTbl, sql, lease);

  CU_ASSERT_STRING_EQUAL (sql,
                          "UPDATE pool SET conf_id = 1, ip = \"ip\", host = \"ali\", mac = \"some_mac\", lease_flag = 1 WHERE id = 1;");
}

void
dhcpLeaseSqlBuilderConfUpdateTest()
{
  dhcpLeaseConfigResult_t conf =
  {
    .id = 1,
    .mask = "255.255.255.0",
    .router = "ip",
    .domain = "bs.com",
    .lease_time = 500,
  };

  char sql[MAX_QUERY_LEN];

  dhcpLeaseSqlBuilderConfUpdate (ConfigTbl, sql, conf);

  CU_ASSERT_STRING_EQUAL (sql,
                          "UPDATE config SET mask = \"255.255.255.0\",  router = \"ip\",  domain = \"bs.com\",  lease_time = 500 WHERE id = 1;");
}

void
dhcpLeaseSqlBuilderPoolDeleteByIdTest()
{
  char sql[MAX_QUERY_LEN];

  dhcpLeaseSqlBuilderPoolDeleteById (PoolTbl, sql, 1);

  CU_ASSERT_STRING_EQUAL (sql, "DELETE FROM pool WHERE id = 1;");
}

void
dhcpLeaseSqlBuilderConfigDeleteByIdTest()
{
  char sql[MAX_QUERY_LEN];

  dhcpLeaseSqlBuilderConfigDeleteById (ConfigTbl, sql, 1);

  CU_ASSERT_STRING_EQUAL (sql, "DELETE FROM config WHERE id = 1;");
}

void
dhcpLeaseSqlBuilderNewPoolTest()
{
  char sql[MAX_QUERY_LEN];

  dhcpLeasePoolResult_t lease =
  {
    .mac = "some_mac",
    .host = "ali", .lease_flag = 1,
    .ip = "ip", .config = {
      .id = 1
    }
  };

  dhcpLeaseSqlBuilderNewPool (PoolTbl, sql, lease);

  CU_ASSERT_STRING_EQUAL (sql,
                          "INSERT INTO pool (conf_id, ip, host, mac, lease_flag) VALUES (1, \"ip\", \"ali\", \"some_mac\", 1);");
}

void
dhcpLeaseSqlBuilderNewConfTest()
{
  char sql[MAX_QUERY_LEN];

  dhcpLeaseConfigResult_t conf =
  {
    .id = 1,
    .mask = "255.255.255.0",
    .router = "ip",
    .domain = "bs.com",
    .lease_time = 500,
  };

  dhcpLeaseSqlBuilderNewConf (ConfigTbl, sql, conf);

  CU_ASSERT_STRING_EQUAL (sql,
                          "INSERT INTO config (mask, router, domain, lease_time) VALUES (\"255.255.255.0\", \"ip\", \"bs.com\", 500);");
}

void
dhcpLeaseSaveNewLeaseTest()
{
  int retval;

  dhcpLeasePoolResult_t lease =
  {
    .mac = "some_mac",
    .host = "ali", .lease_flag = 1,
    .ip = "ip", .config = {
      .id = 1
    }
  };

  CODE_BEHINDE_INITCLOSE_DB (
  {
    retval = dhcpLeaseSaveNewLease (lease);
  });

  CU_ASSERT_TRUE (retval);
}

void
dhcpLeaseUpdateLeaseTest()
{
  int retval;

  dhcpLeasePoolResult_t lease =
  {
    .id = 1, .mac = "some_mac",
    .host = "ali", .lease_flag = 1,
    .ip = "ip", .config = {
      .id = 1
    }
  };

  CODE_BEHINDE_INITCLOSE_DB (
  {
    retval = dhcpLeaseUpdateLease (lease);
  });

  CU_ASSERT_TRUE (retval);
}

void
dhcpLeaseDeleteLeaseByIdTest()
{
  int retval;

  CODE_BEHINDE_INITCLOSE_DB (
  {
    retval = dhcpLeaseDeleteLeaseById (dhcpLeasePoolCount());
  });

  CU_ASSERT_TRUE (retval);
}

void
dhcpLeaseSaveNewConfigTest()
{
  int retval;

  dhcpLeaseConfigResult_t conf =
  {
    .mask = "255.255.255.0",
    .router = "ip",
    .domain = "bs.com",
    .lease_time = 500,
  };

  CODE_BEHINDE_INITCLOSE_DB (
  {
    retval = dhcpLeaseSaveNewConfig (conf);
  });

  CU_ASSERT_TRUE (retval);
}

void
dhcpLeaseUpdateConfigTest()
{
  int retval;

  dhcpLeaseConfigResult_t conf =
  {
    .id = 1,
    .mask = "255.255.255.0",
    .router = "ip",
    .domain = "bs.com",
    .lease_time = 500,
  };

  CODE_BEHINDE_INITCLOSE_DB (
  {
    retval = dhcpLeaseUpdateConfig (conf);
  });

  CU_ASSERT_TRUE (retval);
}

void
dhcpLeaseDeleteConfigByIdTest()
{
  int retval;

  CODE_BEHINDE_INITCLOSE_DB (
  {
    retval = dhcpLeaseDeleteLeaseById (dhcpLeaseDeleteConfigById (1));
  });

  CU_ASSERT_TRUE (retval);
}

void
dhcpLeaseGetConfigByIdTest()
{
  dhcpLeaseConfigResult_t config;

  CODE_BEHINDE_INITCLOSE_DB (
  {
    config = dhcpLeaseGetConfigById (3);
  });

  printf ("configs : \n"
          "\tid -> %d\n"
          "\tmask -> %s\n"
          "\trouter -> %s\n"
          "\tdomain -> %s \n"
          "\tlease_time -> %d\n", config.id, config.mask, config.router, config.domain,
          config.lease_time);
}