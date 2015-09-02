CREATE TABLE user
(
id INT UNSIGNED not null AUTO_INCREMENT,
name varchar(100) not null,
password varchar(100) not null,

PRIMARY KEY (id),
UNIQUE KEY (name)

);