import sqlite3
import pathlib

class Queries():
    config = '''CREATE TABLE config (
	  id INTEGER PRIMARY KEY AUTOINCREMENT,
	  mask TEXT NOT NULL,
	  router TEXT NOT NULL,
	  domain TEXT NOT NULL,
	  lease_time INTEGER NOT NULL DEFAULT 600
    );'''

    pool = '''CREATE TABLE pool ( 
	   id INTEGER PRIMARY KEY AUTOINCREMENT,
	   conf_id INTEGER NOT NULL,
	   ip TEXT NOT NULL,
	   host TEXT NOT NULL,
	   mac TEXT NOT NULL,
	   lease_time INTEGER NOT NULL DEFAULT 600,
	   FOREIGN KEY (conf_id) REFERENCES config(id)
    );'''

currentPath = str(pathlib.Path(__file__).parent.resolve())

dhcpdConn = sqlite3.connect(currentPath + "/dhcpd.db")

dhcpdConn.execute(Queries.config)

dhcpdConn.execute(Queries.pool)

dhcpdConn.commit()