-- Create the database if it does not already exist
CREATE DATABASE IF NOT EXISTS nbaDatabase;
-- Use the created or existing database
USE nbaDatabase;

-- Enable local file loading
SET GLOBAL local_infile = 1;

-- Create the `teams` table to store team information
CREATE TABLE teams (
    TEAM_ID INTEGER NOT NULL,               -- Unique identifier for the team
    NICKNAME VARCHAR(50),                  -- Team's nickname
    YEARFOUNDED INTEGER,                   -- Year the team was founded
    CITY VARCHAR(50),                      -- City of the team
    PRIMARY KEY(TEAM_ID)                   -- Primary key on TEAM_ID
);

-- Load data into the `teams` table from a CSV file
LOAD DATA LOCAL INFILE "/home/stathis_an4/Desktop/teams.csv" IGNORE
INTO TABLE teams
FIELDS TERMINATED BY ","
LINES TERMINATED BY "\n"

-- Create the `players` table to store player information
CREATE TABLE players (
    PLAYER_ID INTEGER NOT NULL,            -- Unique identifier for the player
    PLAYER_NAME VARCHAR(50),              -- Name of the player
    TEAM_ID INTEGER NOT NULL,             -- Associated team ID
    SEASON INTEGER,                       -- Season the player participated in
    INDEX (PLAYER_ID),                    -- Index for faster player lookups
    PRIMARY KEY(TEAM_ID, PLAYER_ID, SEASON), -- Composite primary key
    FOREIGN KEY(TEAM_ID) REFERENCES teams(TEAM_ID) ON UPDATE CASCADE ON DELETE CASCADE -- Enforce relationship with teams
);

-- Load data into the `players` table from a CSV file
LOAD DATA LOCAL INFILE "/home/stathis_an4/Desktop/players.csv" IGNORE
INTO TABLE players
FIELDS TERMINATED BY ','
LINES TERMINATED BY '\n'

-- Create the `games` table to store game information
CREATE TABLE games (
    GAME_ID INTEGER NOT NULL,             -- Unique identifier for the game
    HOME_TEAM_ID INTEGER,                 -- ID of the home team
    VISITOR_TEAM_ID INTEGER,              -- ID of the visiting team
    SEASON INTEGER,                       -- Season in which the game was played
    HOME_TEAM_WINS INTEGER,               -- Indicator if the home team won (1: Yes, 0: No)
    PRIMARY KEY(GAME_ID),                 -- Primary key on GAME_ID
    FOREIGN KEY(HOME_TEAM_ID) REFERENCES teams(TEAM_ID) ON UPDATE CASCADE ON DELETE CASCADE, -- Relationship with teams
    FOREIGN KEY(VISITOR_TEAM_ID) REFERENCES teams(TEAM_ID) ON UPDATE CASCADE ON DELETE CASCADE -- Relationship with teams
);

-- Load data into the `games` table from a CSV file
LOAD DATA LOCAL INFILE "/home/stathis_an4/Desktop/games.csv" IGNORE
INTO TABLE games
FIELDS TERMINATED BY ','
LINES TERMINATED BY '\n'

-- Create the `games_detail` table to store game performance details
CREATE TABLE games_detail (
    GAME_ID INTEGER NOT NULL,             -- Unique identifier for the game
    TEAM_ID INTEGER,                      -- Team ID associated with the game
    PLAYER_ID INTEGER,                    -- Player ID associated with the game
    MINUTES INTEGER,                      -- Minutes played by the player
    PTS REAL,                             -- Points scored by the player
    REB REAL,                             -- Rebounds by the player
    AST REAL,                             -- Assists by the player
    STL REAL,                             -- Steals by the player
    BLK REAL,                             -- Blocks by the player
    PRIMARY KEY(GAME_ID, TEAM_ID, PLAYER_ID), -- Composite primary key
    FOREIGN KEY(GAME_ID) REFERENCES games(GAME_ID) ON UPDATE CASCADE ON DELETE CASCADE, -- Relationship with games
    FOREIGN KEY(TEAM_ID) REFERENCES teams(TEAM_ID) ON UPDATE CASCADE ON DELETE CASCADE, -- Relationship with teams
    FOREIGN KEY(PLAYER_ID) REFERENCES players(PLAYER_ID) ON UPDATE CASCADE ON DELETE CASCADE -- Relationship with players
);

-- Load data into the `games_detail` table from a CSV file
LOAD DATA LOCAL INFILE "/home/stathis_an4/Desktop/games_detail.csv" IGNORE
INTO TABLE games_detail
FIELDS TERMINATED BY ','
LINES TERMINATED BY '\n'

-- (a) Find the nickname of the oldest team
WITH ValidYears AS (
    SELECT MIN(YEARFOUNDED) AS MinYear    -- Find the earliest year a team was founded
    FROM teams
    WHERE YEARFOUNDED > 0                 -- Ignore invalid or zero years
)
SELECT NICKNAME                          -- Retrieve the nickname of the oldest team
FROM teams
WHERE YEARFOUNDED = (SELECT MinYear FROM ValidYears);

-- (b) For each team with at least 3 matches, find the player with the most cumulative points
WITH TeamsWithThreeGames AS (
    SELECT TEAM_ID                        -- Identify teams that participated in at least 3 matches
    FROM games_detail
    GROUP BY TEAM_ID
    HAVING COUNT(*) >= 3
),
PlayerPoints AS (
    SELECT TEAM_ID, PLAYER_ID, SUM(PTS) AS TotalPoints -- Calculate total points for each player in each team
    FROM games_detail
    GROUP BY TEAM_ID, PLAYER_ID
)
SELECT TopPlayers.TEAM_ID, TopPlayers.PLAYER_ID -- Retrieve top player per team
FROM TeamsWithThreeGames
JOIN PlayerPoints AS TopPlayers ON TeamsWithThreeGames.TEAM_ID = TopPlayers.TEAM_ID
WHERE TopPlayers.TotalPoints >= ALL ( -- Ensure the player has the maximum points for their team
    SELECT OtherPlayers.TotalPoints
    FROM PlayerPoints AS OtherPlayers
    WHERE OtherPlayers.TEAM_ID = TopPlayers.TEAM_ID
);

-- (c) Calculate the percentage of home games won by each team
WITH HomeTeamWins AS (
    SELECT HOME_TEAM_ID, COUNT(*) AS HomeWins -- Count home wins per team
    FROM games
    WHERE HOME_TEAM_WINS = 1
    GROUP BY HOME_TEAM_ID
),
HomeTeamGames AS (
    SELECT HOME_TEAM_ID, COUNT(*) AS TotalHomeGames -- Count total home games per team
    FROM games
    GROUP BY HOME_TEAM_ID
)
SELECT HomeWin.HOME_TEAM_ID, 
       100 * (HomeWin.HomeWins / NULLIF(HomeGames.TotalHomeGames, 0)) AS WinPercentage -- Calculate percentage
FROM HomeTeamWins AS HomeWin
JOIN HomeTeamGames AS HomeGames ON HomeWin.HOME_TEAM_ID = HomeGames.HOME_TEAM_ID;

-- (d) Find the name of the player who scored at least 2 points in all games they played
SELECT DISTINCT player.PLAYER_NAME
FROM players AS player
JOIN games_detail AS goalPlayer ON player.PLAYER_ID = goalPlayer.PLAYER_ID
WHERE player.PLAYER_ID NOT IN (
    SELECT PLAYER_ID
    FROM games_detail
    WHERE PTS < 2 -- Exclude players who scored less than 2 points in any game
);

-- (e) Find the nickname of the team with the most wins overall
WITH HomeWins AS (
    SELECT HOME_TEAM_ID AS TEAM_ID, COUNT(*) AS HomeWinCount -- Count home wins per team
    FROM games
    WHERE HOME_TEAM_WINS = 1
    GROUP BY HOME_TEAM_ID
),
AwayWins AS (
    SELECT VISITOR_TEAM_ID AS TEAM_ID, COUNT(*) AS AwayWinCount -- Count away wins per team
    FROM games
    WHERE HOME_TEAM_WINS = 0
    GROUP BY VISITOR_TEAM_ID
),
TotalWins AS (
    SELECT HomeTeamWins.TEAM_ID, (HomeTeamWins.HomeWinCount + AwayWins.AwayWinCount) AS TotalWinCount -- Sum wins
    FROM HomeWins AS HomeTeamWins
    JOIN AwayWins AS AwayWins ON HomeTeamWins.TEAM_ID = AwayWins.TEAM_ID
)
SELECT Name.NICKNAME
FROM teams AS Name
JOIN TotalWins AS TotalTeamWins ON Name.TEAM_ID = TotalTeamWins.TEAM_ID
WHERE TotalTeamWins.TotalWinCount >= ALL (
    SELECT TotalTeamWins2.TotalWinCount
    FROM TotalWins AS TotalTeamWins2
);

-- (f) Find the players who have played together in the most matches (same or opposing teams)
SELECT 
    LEAST(gd1.PLAYER_ID, gd2.PLAYER_ID) AS PLAYER1_ID, -- Ensure unique player pair (smallest ID first)
    GREATEST(gd1.PLAYER_ID, gd2.PLAYER_ID) AS PLAYER2_ID, -- Largest ID second
    COUNT(*) AS SHARED_GAMES -- Count games played together
FROM 
    games_detail gd1
JOIN 
    games_detail gd2 
    ON gd1.GAME_ID = gd2.GAME_ID -- Same game
    AND gd1.PLAYER_ID != gd2.PLAYER_ID -- Different players
GROUP BY 
    LEAST(gd1.PLAYER_ID, gd2.PLAYER_ID), 
    GREATEST(gd1.PLAYER_ID, gd2.PLAYER_ID)
ORDER BY 
    SHARED_GAMES DESC -- Order by the most shared games
LIMIT 1; -- Retrieve the top pair (adjust limit as needed)

