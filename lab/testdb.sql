-- in sqlite3 test.db
PRAGMA foreign_keys = OFF;  -- speed up bulk import prep
CREATE TABLE users (id INTEGER PRIMARY KEY AUTOINCREMENT, username TEXT UNIQUE, salt BLOB, passwd BLOB);
CREATE TABLE records (
  id INTEGER PRIMARY KEY AUTOINCREMENT,
  userID INTEGER NOT NULL,
  accNbr INTEGER NOT NULL UNIQUE,
  fullname TEXT NOT NULL,
  country TEXT NOT NULL,
  phone TEXT NOT NULL UNIQUE,
  accType TEXT NOT NULL,
  balance DOUBLE NOT NULL,
  created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
  FOREIGN KEY(userID) REFERENCES users(id)
);

-- create some users
INSERT INTO users(username, salt, passwd) VALUES ('u0', X'00', X'00');
INSERT INTO users(username, salt, passwd) SELECT 'u' || i, X'00', X'00' FROM (WITH RECURSIVE t(i) AS (SELECT 1 UNION ALL SELECT i+1 FROM t WHERE i<1000) SELECT i FROM t);

-- insert many records (e.g. 200k rows)
WITH RECURSIVE cnt(x) AS (SELECT 1 UNION ALL SELECT x+1 FROM cnt WHERE x < 200000)
INSERT INTO records(userID, accNbr, fullname, country, phone, accType, balance)
SELECT (x % 1000)+1, 10000000000 + x, 'Name'||x, 'Country', '+100000' || x, 'Current', (x % 10000)/100.0
FROM cnt;
