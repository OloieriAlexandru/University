import controller.Controller;

public class App {
    private static final String serverAddress = "127.0.0.1";
    private static final int PORT = 8123;

    public static void main(String[] args) {
        Controller controller = new Controller(serverAddress, PORT);
        controller.startClient();
    }
}
