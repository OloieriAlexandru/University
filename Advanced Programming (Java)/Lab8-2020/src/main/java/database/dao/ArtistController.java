package database.dao;

import database.connection.Database;
import exception.ArtistCreationFailedException;
import exception.ArtistNotFoundException;
import model.Artist;

import java.sql.*;
import java.util.ArrayList;
import java.util.List;

public class ArtistController {
    private Connection connection;

    public ArtistController(Connection connection) {
        this.connection = connection;
    }

    public ArtistController(Database database) {
        connection = database.getConnection();
    }

    public Artist create(String name, String country) throws ArtistCreationFailedException {
        int id = createDB(name, country);
        if (id == -1) {
            throw new ArtistCreationFailedException(name, country);
        }
        return new Artist(id, name, country);
    }

    public List<Artist> findAll() {
        return findAllDB();
    }

    public Artist findByName(String name) throws ArtistNotFoundException {
        Artist artist = findByNameDB(name);
        if (artist == null) {
            throw new ArtistNotFoundException(name);
        }
        return artist;
    }

    /**
     * Inserts an artist in the database
     */
    private int createDB(String name, String country) {
        int id = -1;
        String query = "INSERT INTO artists (name, country) VALUES (?, ?)";
        ResultSet rs = null;
        PreparedStatement pstmt = null;
        try {
            pstmt = connection.prepareStatement(query, Statement.RETURN_GENERATED_KEYS);
            pstmt.setString(1, name);
            pstmt.setString(2, country);
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
     * Returns all artists from the database
     */
    private List<Artist> findAllDB() {
        List<Artist> artists = new ArrayList<>();
        String query = "SELECT * FROM artists";
        ResultSet rs = null;
        Statement stmt = null;
        try {
            stmt = connection.createStatement();
            rs = stmt.executeQuery(query);

            if (rs != null) {
                while (rs.next()) {
                    Artist artist = new Artist(rs.getInt("id"), rs.getString("name"), rs.getString("country"));
                    artists.add(artist);
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
            return artists;
        }
    }

    /**
     * Returns the artist which has the name = 'name'
     */
    private Artist findByNameDB(String name) {
        Artist artist = null;
        String query = "SELECT * FROM artists WHERE UPPER(name) = UPPER(?)";
        ResultSet rs = null;
        PreparedStatement pstmt = null;
        try {
            pstmt = connection.prepareStatement(query);
            pstmt.setString(1, name);

            rs = pstmt.executeQuery();
            if (rs != null && rs.next()) {
                artist = new Artist(rs.getInt("id"), rs.getString("name"), rs.getString("country"));
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
            return artist;
        }
    }
}
