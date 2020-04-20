package repo.jdbc.dao;

import entity.Artist;
import repo.ArtistRepo;
import repo.jdbc.connection.Database;

import java.sql.*;

public class ArtistController implements ArtistRepo {
    private Connection connection;

    public ArtistController(Database database) {
        System.out.println("JDBC Repository Created!");
        this.connection = database.getConnection();
    }

    @Override
    public Artist create(Artist artist) {
        int id = createDB(artist.getName(), artist.getCountry());
        artist.setId(id);
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
}
