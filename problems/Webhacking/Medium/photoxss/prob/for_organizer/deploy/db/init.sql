CREATE USER 'hspace'@'%' IDENTIFIED BY 'hspace_pw';

CREATE DATABASE photoxss;
GRANT ALL PRIVILEGES ON photoxss.* TO 'hspace'@'%';

USE `photoxss`;

CREATE TABLE users (
  id VARCHAR(255) NOT NULL,
  pw VARCHAR(255) NOT NULL
);

INSERT INTO users (id, pw) VALUES ('PHOTOXSS_ADMIN', 'photoxss!@@');