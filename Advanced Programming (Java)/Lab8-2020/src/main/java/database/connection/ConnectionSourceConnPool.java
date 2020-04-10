package database.connection;

import java.sql.Connection;

public class ConnectionSourceConnPool implements ConnectionSource {
    @Override
    public Connection getConnection() {
        return DataSource.getConnection();
    }

    @Override
    public String getType() {
        return "connection-pool";
    }
}
