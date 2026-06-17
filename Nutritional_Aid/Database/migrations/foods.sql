CREATE TABLE foods (
    id BIGINT AUTO_INCREMENT PRIMARY KEY,

    name VARCHAR(255) NOT NULL,
    calories DECIMAL(10,2),
    protein DECIMAL(10,2),
    carbs DECIMAL(10,2),
    fat DECIMAL(10,2),
    servingSize VARCHAR(100),
    nutriScore CHAR(1),

    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,

);