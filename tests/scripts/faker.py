from abc import abstractclassmethod, abstractmethod
import sqlite3
import pathlib
from scapy.all import RandMAC
from scapy.volatile import RandIP, RandNum, RandString


class Queries():
    def __init__(self) -> None:
        self.tables = {
            "config" : "config",
            "pool" : "pool"
        }

    @property
    def config(self):
        return '''CREATE TABLE {} (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            mask TEXT NOT NULL,
            router TEXT NOT NULL,
            domain TEXT NOT NULL,
            lease_time INTEGER NOT NULL DEFAULT 600
            );'''.format(self.tables["config"])

    @property
    def pool(self):
        return '''CREATE TABLE {} ( 
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            conf_id INTEGER NOT NULL,
            ip TEXT NOT NULL,
            host TEXT NOT NULL,
            mac TEXT NOT NULL,
            lease_flag INTEGER NOT NULL DEFAULT 0,
            FOREIGN KEY (conf_id) REFERENCES config(id)
            );'''.format(self.tables["pool"])

    def deleteAll(self, conn: sqlite3.Connection):
        for table in self.tables:
            conn.execute("DROP TABLE IF EXISTS {};".format(table))

    def insertToConf(self, mask: str, router: str, domain: str, lease_time: int) -> None:
        return '''INSERT INTO {} (
            mask,
            router,
            domain,
            lease_time
        ) VALUES (
            "{}", "{}", "{}", {}
        );
        '''.format(self.tables["config"], mask, router, domain, lease_time)

    def insertToPool(self, config_id: int, ip: str, host: str, mac: str, lease_flag: bool) -> None:
        return '''INSERT INTO {} (
            conf_id,
            ip,
            host,
            mac,
            lease_flag
        ) VALUES (
            {}, "{}", "{}", "{}", {}
        );
        '''.format(self.tables["pool"], config_id, ip, host, mac, lease_flag)

class Faker():
    def __init__(self, conn: sqlite3.Connection, quiery: Queries) -> None:
        self.conn = conn
        self.qr = quiery

    def createTables(self):
        self.qr.deleteAll(self.conn)

        self.conn.execute(self.qr.config)

        self.conn.execute(self.qr.pool)

        self.conn.commit()

        return self

    def conf(self):
        for i in range(1, 5):
            self.conn.execute(
                self.qr.insertToConf(
                    "255.255.255." + str(5 * i),
                    "192.168.1." + str(i * 8),
                    str(RandString(5)) + ".example.com",
                    600
                )
            )

        self.conn.commit()

        return self

    def pool(self):
        for i in range(15):
            ip = None
            hostname = None
            mac = None
            
            if i % 2 == 0:
                ip = "192.168.1." + str(RandNum(1, 255))
                hostname = RandString(7)
                mac = RandMAC()
                
            self.conn.execute(
                self.qr.insertToPool(
                    (i % 5) + 1, ip,
                    hostname, mac,
                    i % 2
                )
            )

        self.conn.commit()

        return self

currentPath = str(pathlib.Path(__file__).parent.resolve())

dhcpdConn = sqlite3.connect(currentPath + "/dhcpd.db")

faker = Faker(dhcpdConn, Queries())

faker.createTables().conf().pool()