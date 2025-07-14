CREATE TABLE users (
    id INT AUTO_INCREMENT PRIMARY KEY,
    username VARCHAR(80) NOT NULL,
    password VARCHAR(120) NOT NULL,
    create_time TIMESTAMP DEFAULT CURRENT_TIMESTAMP
) ENGINE = InnoDB
  DEFAULT CHARSET = utf8mb4
  COLLATE = utf8mb4_general_ci;

INSERT INTO users (username, password) VALUES ('admin', '{{REDACTED}}');

CREATE USER IF NOT EXISTS 'guest'@'%' IDENTIFIED BY 'guest';

GRANT SELECT, UPDATE ON user_db.* TO 'guest'@'%';

FLUSH PRIVILEGES;