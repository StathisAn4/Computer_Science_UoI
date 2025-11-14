--4630
--4396
--4869

-- Πίνακας Χρήστης
CREATE TABLE User (
    NameU VARCHAR(50),
    Birthdate VARCHAR(50),
    Country VARCHAR(50),
	primary key(NameU)
);

-- Πίνακας Παιχνίδι
CREATE TABLE Game (
    NameG VARCHAR(50),
    Description VARCHAR(50),
    Rating REAL,
	primary key(NameG)
);

-- Πίνακας Συνδρομή
CREATE TABLE Subscriptions (
    NameU VARCHAR(50),
	NameG VARCHAR(50),
    SubscriptionStartDate VARCHAR(50),
    SubscriptionDuration INT,
    PaymentAmount REAL,
    PRIMARY KEY (NameU, NameG),
    FOREIGN KEY (NameU) REFERENCES User(NameU) ON UPDATE CASCADE ON DELETE CASCADE,
    FOREIGN KEY (NameG) REFERENCES Game(NameG) ON UPDATE CASCADE ON DELETE CASCADE
);

-- Πίνακας Παίζει
CREATE TABLE Playing (
	NameU VARCHAR(50),
	NameG VARCHAR(50),
	PlayingDate VARCHAR(50),
	PlayingStartTime VARCHAR(50),
	PlayingEndTime VARCHAR(50),
	PRIMARY KEY (NameU, NameG,PlayingDate),
	FOREIGN KEY (NameU) REFERENCES User(NameU) ON UPDATE CASCADE ON DELETE CASCADE,
    FOREIGN KEY (NameG) REFERENCES Game(NameG) ON UPDATE CASCADE ON DELETE CASCADE
);

-- Πίνακας Eπίπεδο-μονάδες 
CREATE TABLE LvlNumber (
	NameU VARCHAR(50),
	NameG VARCHAR(50),
	Level INT,
	NumberMon INT,
	PRIMARY KEY (NameU, NameG),
	FOREIGN KEY (NameU) REFERENCES User(NameU) ON UPDATE CASCADE ON DELETE CASCADE,
    FOREIGN KEY (NameG) REFERENCES Game(NameG) ON UPDATE CASCADE ON DELETE CASCADE
);


-- Εισαγωγή πλειάδων στον πίνακα User
INSERT INTO User (NameU, Birthdate, Country) VALUES
    ('player1', '1990-05-15', 'USA'),
    ('player2', '1985-12-10', 'Canada'),
    ('player3', '2000-03-20', 'Spain');
	
-- Εισαγωγή πλειάδων στον πίνακα Game
INSERT INTO Game (NameG, Description, Rating) VALUES
    ('GameA', 'Adventure Game', 4.5),
    ('GameB', 'Puzzle Game', 4.0),
    ('GameC', 'Strategy Game', 4.2);
	
-- Εισαγωγή πλειάδων στον πίνακα Subscriptions
INSERT INTO Subscriptions (NameU, NameG, SubscriptionStartDate, SubscriptionDuration, PaymentAmount) VALUES
    ('player1', 'GameA', '2023-01-15', 30, 15.99),
    ('player2', 'GameB', '2023-02-20', 15, 10.99),
    ('player3', 'GameC', '2023-03-10', 60, 29.99);

INSERT INTO Playing (NameU, NameG, PlayingDate, PlayingStartTime, PlayingEndTime) VALUES 
	('player1', 'GameA', '2023-05-20', '18:00', '20:00'),
	('player2', 'GameB', '2023-06-10', '15:30', '17:00'),
	('player3', 'GameC', '2023-07-05', '20:00', '22:00');

INSERT INTO LvlNumber (NameU, NameG, Level, NumberMon) VALUES 
	('player1', 'GameA', 5, 120),
	('player2', 'GameB', 7, 200),
	('player3', 'GameC', 3, 90);

DELETE FROM User WHERE NameU = 'player1';
