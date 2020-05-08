import org.springframework.boot.autoconfigure.SpringBootApplication;
import server.GameServer;

@SpringBootApplication
public class App {
    private static final int PORT = 8123;

    public static void main(String[] args) {
        javax.net.ssl.HttpsURLConnection.setDefaultHostnameVerifier(
                (hostname, sslSession) -> hostname.equals("127.0.0.1"));

        String certificatesTrustStorePath = "D:\\github\\Uni\\Java\\Lab11-2020\\src\\main\\resources\\keystore\\server.truststore";
        System.setProperty("javax.net.ssl.trustStore", certificatesTrustStorePath);

        GameServer server = new GameServer(PORT);
        if (server.init()) {
            server.waitForClients();
        }
    }
}
