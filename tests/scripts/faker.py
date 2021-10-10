import sqlite3
import pathlib

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
            lease_time INTEGER NOT NULL DEFAULT 600,
            FOREIGN KEY (conf_id) REFERENCES config(id)
            );'''.format(self.tables["pool"])

    def deleteAll(self, conn: sqlite3.Connection):
        for table in self.tables:
            conn.execute("DROP TABLE IF EXISTS {};".format(table))

currentPath = str(pathlib.Path(__file__).parent.resolve())

dhcpdConn = sqlite3.connect(currentPath + "/../database/dhcpd.db")

qr = Queries()

qr.deleteAll(dhcpdConn)

dhcpdConn.execute(qr.config)

dhcpdConn.execute(qr.pool)

dhcpdConn.commit()