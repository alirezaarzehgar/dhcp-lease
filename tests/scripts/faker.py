from abc import abstractclassmethod, abstractmethod
import sqlite3
import pathlib
from typing import Any
from scapy.all import RandMAC
from scapy.volatile import RandChoice, RandIP, RandNum, RandString


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
            host TEXT,
            mac TEXT,
            lease_flag INTEGER NOT NULL DEFAULT 0,
            FOREIGN KEY (conf_id) REFERENCES config(id)
            );'''.format(self.tables["pool"])

    def deleteAll(self, conn: sqlite3.Connection):
        for table in self.tables:
            conn.execute("DROP TABLE IF EXISTS {};".format(table))

    def _wrapOnQoat(self, str: str) -> str:
        return '"{}"'.format(str)

    def _getQoatedIfStr(self, value: Any) -> str:
        if type(value) == str:
            return self._wrapOnQoat(value)
        else:
            return value

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

    def _createSqlFieldAndValue(self, cols, handler, get_values = False):
        string = ""

        index = 0

        def dataHandler(data):
            return data

        tempHandler = handler

        if handler == None:
            tempHandler = dataHandler

        for col in cols:
            val = col

            if get_values:
                val = tempHandler(cols[col])

            coma = ', '

            if index == len(cols) - 1:
                coma = ''

            if cols[col] != None:
                string += '{}{}'.format(val, coma)

            index += 1

        return string


    def insertToPool(self, conf_id: int, ip: str, host: str, mac: str, lease_flag: bool) -> None:
        cols = {
            "conf_id" : conf_id,
            "ip" : ip,
            "host" : host,
            "mac" : mac,
            "lease_flag" : lease_flag
        }

        query = "INSERT INTO " + self.tables["pool"] + " ("

        query += self._createSqlFieldAndValue(
            cols=cols,
            handler=None
        )

        query += ") VALUES ("

        query += self._createSqlFieldAndValue(
            cols=cols,
            handler=self._wrapOnQoat,
            get_values=True
        )
        
        query += ');'

        return query


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
            lease_flag = 0
            ip = "192.168.133." + str(i + 1)
            hostname = None
            mac = None
            
            if RandNum(0, 6) <= 3:
                hostname = str(RandString(7))
                mac = str(RandMAC())
                lease_flag = 1
                
            self.conn.execute(
                self.qr.insertToPool(
                    lease_flag=lease_flag,
                    ip=ip, host=hostname,
                    mac=mac, conf_id=RandNum(0, 5)
                )
            )


        self.conn.commit()

        return self

currentPath = str(pathlib.Path(__file__).parent.resolve())

dhcpdConn = sqlite3.connect(currentPath + "/dhcpd.db")

faker = Faker(dhcpdConn, Queries())

faker.createTables().conf().pool()