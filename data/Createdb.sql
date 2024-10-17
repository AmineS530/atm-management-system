CREATE TABLE records (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    userID INTEGER NOT NULL,
    name TEXT NOT NULL,
    country TEXT NOT NULL,
    phone TEXT NOT NULL,
    accType TEXT NOT NULL,
    accNbr TEXT NOT NULL UNIQUE,
    balance FLOAT NOT NULL,
    username TEXT NOT NULL UNIQUE,
    created_at DATE DEFAULT CURRENT_DATE
);


CREATE TABLE users (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    username TEXT NOT NULL UNIQUE,
    salt BLOB NOT NULL,
    passwd BLOB NOT NULL
);

INSERT INTO users (id, username, salt, passwd) VALUES
(1, 'john_doe', X'12ab34cd56ef78', X'23bc45de67fa89'),
(2, 'jane_smith', X'45ef12ab78cd34', X'56fa34cd23bc78'),
(3, 'alice_jones', X'78cd45ef12ab67', X'89bc56fa34de12'),
(4, 'bob_lee', X'89fa34bc12de56', X'12cd23fa45bc67'),
(5, 'charlie_brown', X'56ab89cd34ef12', X'23fa12cd45bc34');


INSERT INTO records (userID, name, country, phone, accType, accNbr, balance, username, created_at) VALUES
(1, 'John Doe', 'USA', '+1234567890', 'Checking', '123456789012', 1050.75, 'john_doe', '2023-08-01 10:30:00'),
(2, 'Jane Smith', 'Canada', '+1987654321', 'Savings', '234567890123', 2000.54, 'jane_smith', '2023-08-02 11:45:00'),
(3, 'Alice Jones', 'UK', '+447700900123', 'Checking', '345678901234', 1575.90, 'alice_jones', '2023-08-03 12:00:00'),
(4, 'Bob Lee', 'Australia', '+61412345678', 'Savings', '456789012345', 3200.21, 'bob_lee', '2023-08-04 14:15:00'),
(5, 'Charlie Brown', 'Germany', '+491234567890', 'Checking', '567890123456', 950.08, 'charlie_brown', '2023-08-05 09:30:00'),
(6, 'test Brown', 'Germany', '+491234567890', 'Checking', '56781230123456', 950.08, '123', '2023-08-05 09:30:00');

