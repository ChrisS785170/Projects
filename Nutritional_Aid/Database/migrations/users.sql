CREATE TABLE users (
    id INT AUTO_INCREMENT PRIMARY KEY,
    email VARCHAR(255) NOT NULL UNIQUE,
    username VARCHAR(100) NOT NULL UNIQUE,
    password_hash VARCHAR(255) NOT NULL,
-- these fields will store the users current totals for the day, they will be updated whenever the user logs a food or a meal
    total_calories DECIMAL(10,2) DEFAULT 0,
    total_protein DECIMAL(10,2) DEFAULT 0,
    total_carbs DECIMAL(10,2) DEFAULT 0,
    total_fat DECIMAL(10,2) DEFAULT 0,

--  these are the default daily targets, calculated based off BMI
    daily_calorie DECIMAL(10,2) DEFAULT 2000,
    daily_protein DECIMAL(10,2) DEFAULT 50,
    daily_carbs DECIMAL(10,2) DEFAULT 250,
    daily_fat DECIMAL(10,2) DEFAULT 70,

    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
        ON UPDATE CURRENT_TIMESTAMP

);