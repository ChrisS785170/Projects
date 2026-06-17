CREATE TABLE food_tokens (
    food_id BIGINT NOT NULL,
    token VARCHAR(100) NOT NULL,

    PRIMARY KEY (food_id, token),

    FOREIGN KEY (food_id)
        REFERENCES foods(id)
        ON DELETE CASCADE,

    INDEX idx_token (token)
);