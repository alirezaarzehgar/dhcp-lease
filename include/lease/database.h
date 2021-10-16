#if !defined(LEASE_DATABASE_H)
#define LEASE_DATABASE_H

/* config table */
#define LEASE_CONFIG_FIELD_ID                   "id"
#define LEASE_CONFIG_TABLE_NAME                 "config"
#define LEASE_CONFIG_ID                         "conf_id"
#define LEASE_CONFIG_MASK                       "mask"
#define LEASE_CONFIG_ROUTER                     "router"
#define LEASE_CONFIG_DOMAIN                     "domain"
#define LEASE_CONFIG_LEASE_TIME                 "lease_time"

/* Config Table */
static struct configTbl
{
  char name[7];
  char id[3];
  char mask[5];
  char router[7];
  char domain[7];
  char lease_time[11];
} ConfigTbl =
{
  .name = "config",
  .id = "id",
  .mask = "mask",
  .router = "router",
  .domain = "domain",
  .lease_time = "lease_time",
};

/* pool table */
#define LEASE_POOL_TABLE_NAME                   "pool"
#define LEASE_POOL_ID                           "id"
#define LEASE_POOL_CONFIG_ID                    "conf_id"
#define LEASE_POOL_IP                           "ip"
#define LEASE_POOL_HOST                         "host"
#define LEASE_POOL_MAC                          "mac"
#define LEASE_POOL_LEASE_FLAG                   "lease_flag"

static struct poolTbl
{
  char name[5];
  char id[3];
  char conf_id[8];
  char ip[3];
  char host[5];
  char mac[4];
  char lease_flag[11];
} PoolTbl =
{
  .name = "pool",
  .id = "id",
  .conf_id = "conf_id",
  .ip = "ip",
  .host = "host",
  .mac = "mac",
  .lease_flag = "lease_flag",
};


#endif // LEASE_DATABASE_H
