
-- MySQL Queries:

CREATE TABLE genres(
	id INT AUTO_INCREMENT PRIMARY KEY,
	name VARCHAR(20) NOT NULL
)
/

INSERT INTO genres(name) VALUES("rock");
/
INSERT INTO genres(name) VALUES("r&b");
/
INSERT INTO genres(name) VALUES("pop");
/
INSERT INTO genres(name) VALUES("latin");
/
INSERT INTO genres(name) VALUES("jazz");
/
INSERT INTO genres(name) VALUES("hip hop");
/
INSERT INTO genres(name) VALUES("folk");
/
INSERT INTO genres(name) VALUES("flamenco");
/
INSERT INTO genres(name) VALUES("electronic");
/
INSERT INTO genres(name) VALUES("country");
/
INSERT INTO genres(name) VALUES("blues");
/

ALTER TABLE albums
ADD COLUMN genre_id INT;
/
