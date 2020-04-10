package database.connection;

import java.sql.Connection;

/**
 * An interface which allows to write the logic for the second task from Bonus section only once,
 * And to inject different class which have the function 'getConnection()' implemented
 */
public interface ConnectionSource {
    Connection getConnection();

    String getType();
}
