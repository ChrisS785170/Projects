CREATE DATABASE Dodgyclubs;

USE Dodgyclubs;

CREATE TABLE Users (
  user_id INT PRIMARY KEY AUTO_INCREMENT,
  username VARCHAR(255) NOT NULL,
  email VARCHAR(255) NOT NULL,
  firstname VARCHAR(255) NOT NULL,
  lastname VARCHAR(255) NOT NULL,
  password VARCHAR(255) NOT NULL,
  profile_picture VARCHAR(255),
  is_admin BOOLEAN NOT NULL DEFAULT FALSE
);

CREATE TABLE Clubs (
  club_id INT PRIMARY KEY AUTO_INCREMENT,
  name VARCHAR(255) NOT NULL,
  description TEXT
);

CREATE TABLE Updates (
  update_id INT PRIMARY KEY AUTO_INCREMENT,
  content TEXT NOT NULL,
  timestamp DATETIME NOT NULL,
  club_id INT,
  manager_id INT,
  FOREIGN KEY (club_id) REFERENCES Clubs (club_id),
  FOREIGN KEY (manager_id) REFERENCES Users (user_id)
);

CREATE TABLE Events (
  event_id INT PRIMARY KEY AUTO_INCREMENT,
  name VARCHAR(255) NOT NULL,
  description TEXT,
  event_timestamp TIMESTAMP NOT NULL,
  club_id INT,
  manager_id INT,
  FOREIGN KEY (club_id) REFERENCES Clubs(club_id),
  FOREIGN KEY (manager_id) REFERENCES Users(user_id)
);

CREATE TABLE Rsvp (
  rsvp_id INT PRIMARY KEY AUTO_INCREMENT,
  user_id INT,
  event_id INT,
  FOREIGN KEY (user_id) REFERENCES Users (user_id),
  FOREIGN KEY (event_id) REFERENCES Events (event_id),
  UNIQUE KEY `event_user_key` (`event_id`,`user_id`)
);

CREATE TABLE Memberships (
  membership_id INT PRIMARY KEY AUTO_INCREMENT,
  club_id INT,
  user_id INT,
  FOREIGN KEY (club_id) REFERENCES Clubs(club_id),
  FOREIGN KEY (user_id) REFERENCES Users(user_id)
);

CREATE TABLE Managers (
  manager_id INT PRIMARY KEY AUTO_INCREMENT,
  user_id INT,
  club_id INT,
  FOREIGN KEY (user_id) REFERENCES Users(user_id),
  FOREIGN KEY (club_id) REFERENCES Clubs(club_id)
);

ALTER USER 'root'@'localhost' IDENTIFIED WITH mysql_native_password BY 'password';

-- Ensure auto increment starts at 1
ALTER TABLE Users AUTO_INCREMENT = 1;
ALTER TABLE Clubs AUTO_INCREMENT = 1;
ALTER TABLE Updates AUTO_INCREMENT = 1;
ALTER TABLE Events AUTO_INCREMENT = 1;
ALTER TABLE Rsvp AUTO_INCREMENT = 1;
ALTER TABLE Memberships AUTO_INCREMENT = 1;
ALTER TABLE Managers AUTO_INCREMENT = 1;

-- Inserting dummy data:

INSERT INTO Users (username, email, firstname, lastname, password, profile_picture, is_admin)
VALUES
('johnsmith', 'johnsmith@gmail.com', 'John', 'Smith', 'yourPasswordHere', 'images/johnsmith.png', FALSE),
('emmajones', 'emmajones@gmail.com', 'Emma', 'Jones', 'yourPasswordHere', 'images/emmajones.png', FALSE),
('robertbrown', 'robertbrown@gmail.com', 'Robert', 'Brown', 'yourPasswordHere', 'images/robertbrown.png', FALSE),
('sophiawilliams', 'sophiawilliams@gmail.com', 'Sophia', 'Williams', 'yourPasswordHere', 'images/sophiawilliams.png', FALSE),
('michaeljohnson', 'michaeljohnson@gmail.com', 'Michael', 'Johnson', 'yourPasswordHere', 'images/michaeljohnson.png', TRUE),
('jameswilson', 'jameswilson@gmail.com', 'James', 'Wilson', 'yourPasswordHere', 'images/jameswilson.png', FALSE),
('juliamartin', 'juliamartin@gmail.com', 'Julia', 'Martin', 'yourPasswordHere', 'images/juliamartin.png', FALSE),
('lucasgarcia', 'lucasgarcia@gmail.com', 'Lucas', 'Garcia', 'yourPasswordHere', 'images/lucasgarcia.png', FALSE),
('scarletmiller', 'scarletmiller@gmail.com', 'Scarlet', 'Miller', 'yourPasswordHere', 'images/scarletmiller.png', FALSE),
('benjamindavis', 'benjamindavis@gmail.com', 'Benjamin', 'Davis', 'yourPasswordHere', 'images/benjamindavis.png', FALSE),
('emilyrodriguez', 'emilyrodriguez@gmail.com', 'Emily', 'Rodriguez', 'yourPasswordHere', 'images/emilyrodriguez.png', FALSE),
('oliverthomas', 'oliverthomas@gmail.com', 'Oliver', 'Thomas', 'yourPasswordHere', 'images/oliverthomas.png', FALSE),
('chloemoore', 'chloemoore@gmail.com', 'Chloe', 'Moore', 'yourPasswordHere', 'images/chloemoore.png', FALSE),
('ethananderson', 'ethananderson@gmail.com', 'Ethan', 'Anderson', 'yourPasswordHere', 'images/ethananderson.png', TRUE),
('bellataylor', 'bellataylor@gmail.com', 'Bella', 'Taylor', 'yourPasswordHere', 'images/bellataylor.png', FALSE);

INSERT INTO Clubs (name, description)
VALUES
('Art Club', 'A place for those who appreciate and create art.'),
('Coding Club', 'Join to learn and collaborate on coding projects.'),
('Book Club', 'We love reading and discussing various books.'),
('Fitness Club', 'A group of fitness enthusiasts who support each other.'),
('Music Club', 'For the love of music creation and appreciation.'),
('Photography Club', 'We love capturing the beauty of the world.'),
('Chess Club', 'For all chess enthusiasts - beginners and experts alike.'),
('Film Club', 'Join us to watch and discuss amazing films.'),
('Food Club', 'We love cooking and trying new cuisines.'),
('Nature Club', 'Explore and appreciate the beauty of nature with us.'),
('Science Club', 'For everyone interested in learning more about science.'),
('Writing Club', 'A space for writers to share and critique their work.'),
('Astronomy Club', 'We love exploring the stars and beyond.'),
('Travel Club', 'Join us to share travel experiences and plan new adventures.'),
('Gardening Club', 'We grow together - both plants and as people.');

INSERT INTO Updates (content, timestamp, club_id, manager_id)
VALUES
('Welcome to Art Club! Looking forward to creating together.', CURRENT_TIMESTAMP, 1, 1),
('Coding Club: New project ideas are welcome!', CURRENT_TIMESTAMP, 2, 2),
('Book Club: The book for next month is "1984" by George Orwell.', CURRENT_TIMESTAMP, 3, 3),
('Fitness Club: Remember, every step counts!', CURRENT_TIMESTAMP, 4, 4),
('Music Club: Band practice is postponed to next Friday.', CURRENT_TIMESTAMP, 5, 5),
('Join us this weekend for a photography trip!', NOW(), 6, 1),
('We have a chess tournament next week. All are welcome.', NOW(), 7, 2),
('We are watching The Godfather this Friday!', NOW(), 8, 3),
('Potluck this Sunday! Bring your favorite dish.', NOW(), 9, 4),
('Nature walk scheduled for Saturday morning.', NOW(), 10, 5),
('New research seminar on quantum mechanics coming soon!', NOW(), 11, 6),
('Writing workshop next Monday. Bring your stories!', NOW(), 12, 7),
('Stargazing event coming soon. Clear your schedules!', NOW(), 13, 8),
('We will share our most exciting travel experiences at our next meeting!', NOW(), 14, 9),
('Spring planting session this Saturday. All hands on deck!', NOW(), 15, 10);

INSERT INTO Events (name, description, event_timestamp, club_id, manager_id)
VALUES
('Art Club Exhibition', 'Annual exhibition of club member art.', '2023-06-10 10:00:00', 1, 1),
('Coding Club Hackathon', '24 hours to build something cool.', '2023-06-20 11:00:00', 2, 2),
('Book Club Meeting', 'Discussion about our book of the month.', '2023-06-25 12:00:00', 3, 3),
('Fitness Club Marathon', 'Let us test our limits together.', '2023-07-01 06:00:00', 4, 4),
('Music Club Concert', 'Showcase of our band talents.', '2023-07-15 20:00:00', 5, 5),
('Photography Trip to City Park', 'Bring your cameras and let us capture some amazing shots!', '2023-07-01 10:00:00', 6, 1),
('Chess Tournament', 'All members are welcome to participate. Let the games begin!', '2023-07-07 15:00:00', 7, 2),
('Film Club - The Godfather', 'We will be watching The Godfather this Friday.', '2023-07-06 19:00:00', 8, 3),
('Food Club Potluck', 'Bring your favorite dish to share with the group.', '2023-07-09 12:00:00', 9, 4),
('Nature Walk', 'Join us for a walk in the woods. Do not forget your hiking shoes!', '2023-07-02 09:00:00', 10, 5),
('Quantum Mechanics Seminar', 'A seminar on the fascinating world of quantum mechanics.', '2023-07-10 14:00:00', 11, 6),
('Writing Workshop', 'Bring your stories and get feedback from fellow writers.', '2023-07-03 16:00:00', 12, 7),
('Stargazing Night', 'Join us for a night under the stars.', '2023-07-07 20:00:00', 13, 8),
('Travel Experiences Sharing', 'Come share your best travel stories and get inspiration for your next adventure!', '2023-07-08 14:00:00', 14, 9),
('Spring Planting Session', 'Let us get our hands dirty and plant some new flowers!', '2023-07-04 10:00:00', 15, 10);

INSERT INTO Memberships (club_id, user_id)
VALUES
(1, 1),
(2, 2),
(3, 3),
(4, 4),
(5, 5),
(6, 1),
(7, 2),
(8, 3),
(9, 4),
(10, 5),
(11, 6),
(12, 7),
(13, 8),
(14, 9),
(15, 10);

INSERT INTO Rsvp (user_id, event_id)
VALUES
(1, 1),
(2, 2),
(3, 3),
(4, 4),
(5, 5),
(1, 6),
(2, 7),
(3, 8),
(4, 9),
(5, 10),
(6, 11),
(7, 12),
(8, 13),
(9, 14),
(10, 15);