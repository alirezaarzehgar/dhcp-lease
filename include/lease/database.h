#if !defined(LEASE_DATABASE_H)
#define LEASE_DATABASE_H

/* config table */
#define LEASE_CONFIG_TABLE_NAME                 "config"
#define LEASE_CONFIG_ID                         "conf_id"
#define LEASE_CONFIG_MASK                       "mask"
#define LEASE_CONFIG_ROUTER                     "router"
#define LEASE_CONFIG_DOMAIN                     "domain"
#define LEASE_CONFIG_LEASE_TIME                 "lease_time"

/* pool table */
#define LEASE_POOL_TABLE_NAME                   "pool"
#define LEASE_POOL_ID                           "id"
#define LEASE_POOL_CONFIG_ID                    "conf_id"
#define LEASE_POOL_IP                           "ip"
#define LEASE_POOL_HOST                         "host"
#define LEASE_POOL_MAC                          "mac"
#define LEASE_POOL_LEASE_FLAG                   "lease_flag"

#endif // LEASE_DATABASE_H
