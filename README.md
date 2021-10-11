# DHCP Lease Module

This module provide many funcions for leasing IP and getting IP from pools.

### Goals

- <a href="#1">getting ip from pool</a>
- <a href="#2">leasing ip address to pool</a>
- <a href="#3">Data base schema</a>

### <p id="1">getting ip from pool</a>

What this fucntion works ?

 1) pick up pool
 0) get first non-reserved ip and return
 0) if ip requested check on leases and :
    - if exists send ip
    - if does not exists search to leases and if this ip doesn't reserved, send ip

#### in depth of operation

First of all we should get the first no reserved ip address with this query :
```sql
SELECT id, conf_id, ip FROM pool WHERE lease_flag = 0 LIMIT 1;
```
We need id for updating on ack, conf_id for sending typical options
and ip for sending on replay packets.

Another fields like mac, host and lease_flag sets to zero or NULL.

### <p id="2">leasing ip address to pool</p>

 - update lease flag to true
 - fill mac address field
 - fill hostname field

After getting request and when sending ack,
we should update non reserved ip address on previous step.

We have an id and required parameters like mac, host and lease_flag :
```sql
UPDATE pool SET mac = "some-mac-address", host = "some-hostname", lease_flag = 1 WHERE id = 1;
```

### <p id="#3">Database schema</p>

## config table
```sql
CREATE TABLE config (
	id INTEGER PRIMARY KEY AUTOINCREMENT,
	mask TEXT NOT NULL,
	router TEXT NOT NULL,
	domain TEXT NOT NULL,
	lease_time INTEGER NOT NULL DEFAULT 600
);

```


```
                        config table
---------------------------------------------------------
|      NAME     |                  TYPE                 |
---------------------------------------------------------
| id            | INTEGER PRIMARY KEY AUTOINCREMENT     |
---------------------------------------------------------
| mask          | TEXT NOT NULL                         |
---------------------------------------------------------
| router        | TEXT NOT NULL                         |
---------------------------------------------------------
| domain        | TEXT NOT NULL UNIQUE                  |
---------------------------------------------------------
| lease_time    | INTEGER NOT NULL DEFAULT 0            |
---------------------------------------------------------
```

## pool table
```sql
CREATE TABLE pool ( 
	id INTEGER PRIMARY KEY AUTOINCREMENT,
	conf_id INTEGER NOT NULL,
	ip TEXT NOT NULL,
	host TEXT NOT NULL,
	mac TEXT NOT NULL,
	lease_time INTEGER NOT NULL DEFAULT 600,
	FOREIGN KEY (conf_id) REFERENCES config(id)
);
```

```
                        pool table
---------------------------------------------------------
|      NAME     |                  TYPE                 |
---------------------------------------------------------
| id            | INTEGER PRIMARY KEY AUTOINCREMENT     |
---------------------------------------------------------
| config_id     | INTEGER NOT NULL                      |
---------------------------------------------------------
| hostname      | TEXT NOT NULL                         |
---------------------------------------------------------
| mac           | TEXT NOT NULL UNIQUE                  |
---------------------------------------------------------
| lease_flag    | INTEGER NOT NULL DEFAULT 0            |
---------------------------------------------------------
| FOREIGN KEY   | (conf_id) REFERENCES config (id)      |
---------------------------------------------------------
```