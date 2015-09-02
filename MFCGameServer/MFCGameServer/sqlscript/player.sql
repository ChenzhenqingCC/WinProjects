CREATE TABLE playerbase
(
id INT UNSIGNED not null AUTO_INCREMENT,
uid INT UNSIGNED not null,
level INT UNSIGNED not null,
worktype INT UNSIGNED not null,
nickname varchar(100) not null,

PRIMARY KEY (id),
UNIQUE KEY (nickname)

);