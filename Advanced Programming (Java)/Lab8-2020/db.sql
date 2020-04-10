
-- MySQL Queries:

CREATE TABLE artists(
    	id INT AUTO_INCREMENT PRIMARY KEY,
    	name VARCHAR(100) NOT NULL,
    	country VARCHAR(100)
)
/
                     
CREATE TABLE albums(
   	id INT AUTO_INCREMENT PRIMARY KEY,
    	name VARCHAR(100) NOT NULL,
    	artist_id INT NOT NULL,
    	release_year INT,
    	FOREIGN KEY(artist_id) REFERENCES artists(id) ON DELETE CASCADE
)
/

CREATE TABLE charts(
	id INT AUTO_INCREMENT PRIMARY KEY,
	name VARCHAR(100) NOT NULL	
)
/

CREATE TABLE charts_albums(
	id INT AUTO_INCREMENT PRIMARY KEY,
	rank INT NOT NULL,
	chart_id INT NOT NULL,
	album_id INT NOT NULL,
	FOREIGN KEY(chart_id) REFERENCES charts(id) ON DELETE CASCADE,
	FOREIGN KEY(album_id) REFERENCES albums(id) ON DELETE CASCADE,
	CONSTRAINT uc_chart_rank UNIQUE(chart_id, rank)
)
/
