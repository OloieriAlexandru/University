package database.dao;

import database.connection.Database;
import exception.ChartCreationFailedException;
import model.Album;
import model.Chart;
import model.ChartAlbumEntry;

import java.sql.*;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;

public class ChartController {
    private Connection connection;

    public ChartController(Database database) {
        connection = database.getConnection();
    }

    public Chart create(String name) throws ChartCreationFailedException {
        return create(name, null);
    }

    /**
     * Returns a list containing all the charts from the database (including the album objects corresponding to a chart)
     */
    public List<Chart> findAll(Map<Integer, Album> albumMap) {
        List<Chart> charts = findAllDB();
        List<ChartAlbumEntry> albumEntries = findAllEntriesDB();
        int albumEntryIndex = 0;
        for (int i = 0; i < charts.size() && albumEntryIndex < albumEntries.size(); ++i) {
            while (albumEntryIndex < albumEntries.size()) {
                if (charts.get(i).getId() == albumEntries.get(albumEntryIndex).getChartId()) {
                    charts.get(i).addAlbum(albumMap.get(albumEntries.get(albumEntryIndex).getAlbumId()));
                } else {
                    break;
                }
                ++albumEntryIndex;
            }
        }
        return charts;
    }

    /**
     * Inserts in the database a chart having name = 'name' and the albums 'albums'
     */
    public Chart create(String name, List<Album> albums) throws ChartCreationFailedException {
        int id = createDB(name);
        if (id == -1) {
            throw new ChartCreationFailedException(name);
        }
        Chart chart = new Chart(id, name, albums);
        if (albums != null) {
            insertChartEntries(chart);
        }
        return chart;
    }

    /**
     * Inserts all the albums from a chart
     */
    private void insertChartEntries(Chart chart) {
        List<Album> albums = chart.getAlbums();
        for (int i = 0; i < albums.size(); ++i) {
            insertChartEntry(chart, albums.get(i), i + 1);
        }
    }

    /**
     * Inserts a chart entry (an album with a specific rank)
     */
    private void insertChartEntry(Chart chart, Album album, int rank) {
        String query = "INSERT INTO charts_albums (rank, chart_id, album_id) VALUES (?, ?, ?)";
        try (PreparedStatement pstmt = connection.prepareStatement(query)) {
            pstmt.setInt(1, rank);
            pstmt.setInt(2, chart.getId());
            pstmt.setInt(3, album.getId());
            pstmt.executeUpdate();
        } catch (SQLException e) {
            System.out.println(e.getMessage());
        }
    }

    /**
     * Returns a list of charts (the Chart object doesn't contain any album at this point)
     */
    private List<Chart> findAllDB() {
        List<Chart> charts = new ArrayList<>();

        String query = "SELECT * FROM charts ORDER BY id";
        ResultSet rs = null;
        Statement stmt = null;
        try {
            stmt = connection.createStatement();
            rs = stmt.executeQuery(query);

            if (rs != null) {
                while (rs.next()) {
                    Chart chart = new Chart(rs.getInt("id"), rs.getString("name"));
                    charts.add(chart);
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
            return charts;
        }
    }

    /**
     * Returns all the albums from all charts
     */
    private List<ChartAlbumEntry> findAllEntriesDB() {
        List<ChartAlbumEntry> albumEntries = new ArrayList<>();

        String query = "SELECT * FROM charts_albums ORDER BY chart_id, rank";
        ResultSet rs = null;
        Statement stmt = null;
        try {
            stmt = connection.createStatement();
            rs = stmt.executeQuery(query);

            if (rs != null) {
                while (rs.next()) {
                    ChartAlbumEntry chartAlbumEntry = new ChartAlbumEntry(rs.getInt("rank"), rs.getInt("chart_id"), rs.getInt("album_id"));
                    albumEntries.add(chartAlbumEntry);
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
            return albumEntries;
        }
    }

    /**
     * Creates a chart
     */
    private int createDB(String name) {
        int id = -1;
        String query = "INSERT INTO charts(name) VALUES (?)";
        ResultSet rs = null;
        PreparedStatement pstmt = null;
        try {
            pstmt = connection.prepareStatement(query, Statement.RETURN_GENERATED_KEYS);
            pstmt.setString(1, name);
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
