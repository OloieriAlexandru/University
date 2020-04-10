package database.dao;

import database.connection.Database;
import exception.AlbumCreationFailedException;
import model.Album;

import java.sql.*;
import java.util.ArrayList;
import java.util.List;

public class AlbumController {
    private Connection connection;

    public AlbumController(Database database) {
        connection = database.getConnection();
    }

    public Album create(String name, int artistId, int releaseYear) throws AlbumCreationFailedException {
        int id = createDB(name, artistId, releaseYear);
        if (id == -1) {
            throw new AlbumCreationFailedException(name, releaseYear);
        }
        return new Album(id, name, artistId, releaseYear);
    }

    public List<Album> findAll() {
        return findAllDB();
    }

    public List<Album> findByArtistId(int artistId) {
        return findByArtistIdDB(artistId);
    }

    /**
     * Inserts a new album in the database. Returns its id
     */
    private int createDB(String name, int artistId, int releaseYear) {
        int id = -1;
        String query = "INSERT INTO albums (name, artist_id, release_year) VALUES (?, ?, ?)";
        ResultSet rs = null;
        PreparedStatement pstmt = null;
        try {
            pstmt = connection.prepareStatement(query, Statement.RETURN_GENERATED_KEYS);
            pstmt.setString(1, name);
            pstmt.setInt(2, artistId);
            pstmt.setInt(3, releaseYear);
            pstmt.executeUpdate();

            rs = pstmt.getGeneratedKeys();
            if (rs != null && rs.next()) {
                id = rs.getInt(1);
            }
        } catch (SQLException e) {
            System.out.println(e.getMessage());
        } finally {
            try {
                if (rs != null) {
                    rs.close();
                }
                if (pstmt != null) {
                    pstmt.close();
                }
            } catch (SQLException ignored) {
            }
            return id;
        }
    }

    /**
     * Returns a list containing all the albums from the database
     */
    private List<Album> findAllDB() {
        String query = "SELECT * FROM albums";
        return findByQuery(query);
    }

    /**
     * Returns a list of albums, each of them having artist_id = 'artistId'
     */
    private List<Album> findByArtistIdDB(int artistId) {
        String query = "SELECT * FROM albums WHERE artist_id = " + artistId;
        return findByQuery(query);
    }

    /**
     * Returns a list of albums from the database
     */
    private List<Album> findByQuery(String query) {
        List<Album> albums = new ArrayList<>();
        ResultSet rs = null;
        Statement stmt = null;
        try {
            stmt = connection.createStatement();
            rs = stmt.executeQuery(query);

            if (rs != null) {
                while (rs.next()) {
                    Album album = new Album(rs.getInt("id"), rs.getString("name"), rs.getInt("artist_id"), rs.getInt("release_year"));
                    albums.add(album);
                }
            }
        } catch (SQLException e) {
            System.out.println(e.getMessage());
        } finally {
            try {
                if (rs != null) {
                    rs.close();
                }
                if (stmt != null) {
                    stmt.close();
                }
            } catch (SQLException ignored) {
            }
            return albums;
        }
    }
}
