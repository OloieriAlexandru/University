package command;

import database.connection.ConnectionSource;
import database.dao.ArtistController;
import model.Artist;

import java.sql.Connection;
import java.sql.SQLException;
import java.util.List;

/**
 * A task which will be passed to the ThreadPoolExecutor
 */
public class CommandConcurrentTask implements Runnable {
    private String name;
    private ConnectionSource connectionSource;

    public CommandConcurrentTask(String name, ConnectionSource connectionSource) {
        this.name = name;
        this.connectionSource = connectionSource;
    }

    @Override
    public void run() {
        Connection connection = connectionSource.getConnection();
        ArtistController artistController = new ArtistController(connection);

        List<Artist> artists = artistController.findAll();

        System.out.println(name + " retrieved " + artists.size() + " artists from the database!");

        if (connectionSource.getType().equals("connection-pool")) {
            try {
                connection.close();
            } catch (SQLException ignored) {
            }
        }
    }
}
