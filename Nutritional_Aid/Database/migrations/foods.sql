CREATE TABLE foods (
    id BIGINT AUTO_INCREMENT PRIMARY KEY,

    external_id VARCHAR(100),

    name VARCHAR(255) NOT NULL,
    calories DECIMAL(10,2),
    protein DECIMAL(10,2),
    carbs DECIMAL(10,2),
    fat DECIMAL(10,2),
    serving_size VARCHAR(100),
    nutri_score CHAR(1),

    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,

);