import server.GameServer;

public class App {
    private static final int PORT = 8123;

    public static void main(String[] args) {
        GameServer server = new GameServer(PORT);
        if (server.init()) {
            server.waitForClients();
        }
    }
}
