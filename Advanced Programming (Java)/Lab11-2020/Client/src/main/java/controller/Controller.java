package controller;

import model.Board;
import rmi.RMIServices;
import util.Helper;
import util.Pair;
import view.UI;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.Socket;
import java.rmi.NotBoundException;
import java.rmi.RemoteException;
import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;
import java.util.List;
import java.util.Scanner;

public class Controller {
    public static final int BOARD_SQUARES_COUNT = 19;
    private Board board;
    private String serverAddress;
    private int port;
    private UI ui;
    private Boolean clicked;
    private int clickedX;
    private int clickedY;
    private boolean loggedIn;

    public Controller(String serverAddress, int port) {
        this.serverAddress = serverAddress;
        this.port = port;
        this.loggedIn = false;
    }

    /**
     * The entry point of the client app logic
     */
    public void startClient() {
        try {
            Socket socket = new Socket(serverAddress, port);

            gameClientLogic(socket);

            socket.close();
        } catch (IOException e) {
            System.out.println(e.getMessage());
        }
    }

    /**
     * Async function called when the user clicked on the board
     */
    public void notifyMouseClicked(int x, int y) {
        synchronized (clicked) {
            if (clicked) {
                return;
            }
            int side = (view.Board.BOARD_SIDE_PX - 2 * BOARD_SQUARES_COUNT) / BOARD_SQUARES_COUNT;
            int more = (view.Board.BOARD_SIDE_PX - side * BOARD_SQUARES_COUNT) / 2;

            clickedX = (x - more) / (side);
            clickedY = (y - more) / (side);

            if (board.checkIfSpotIsFree(clickedX, clickedY)) {
                clicked = true;
            }
        }
    }

    /**
     * A loop which takes user requests and sends them to the server
     */
    private void gameClientLogic(Socket socket) throws IOException {
        BufferedReader in = new BufferedReader(new InputStreamReader(socket.getInputStream()));
        PrintWriter out = new PrintWriter(socket.getOutputStream(), true);
        Scanner consoleIn = new Scanner(System.in);
        boolean running = true;

        System.out.println("Available commands:");
        readList(in);

        try {
            while (running) {
                String cmd = Helper.readLine(consoleIn, "shell>");

                if (cmd.equals("list-rmi")) {
                    listCommandsRmiCall();
                    continue;
                }

                out.println(cmd);
                int commandIndex = Integer.parseInt(in.readLine());
                running = runCommand(commandIndex, in, out);
            }
        } catch (IOException e) {
            System.out.println("");
        }
    }

    private void listCommandsRmiCall() {
        try {
            Registry registry = LocateRegistry.getRegistry("localhost");
            RMIServices rmiServices = (RMIServices) registry.lookup("RmiServices");
            List<String> commands = rmiServices.getAvailableCommands(loggedIn);
            System.out.println("Commands retrieved with a RMI call:");
            for (String str : commands) {
                System.out.println(str);
            }
        } catch (RemoteException | NotBoundException e) {
            System.out.println("Failed when loading RMI service");
            System.out.println(e.getMessage());
        }
    }

    /**
     * Runs the user's command
     */
    private boolean runCommand(int commandIndex, BufferedReader in, PrintWriter out) throws IOException {
        switch (commandIndex) {
            case 0:
                commandRegister(in, out);
                break;
            case 1:
                commandLogin(in, out);
                break;
            case 2:// create
            case 3:
                commandCreateGame(in, out);
                break;
            case 4: //join
                commandJoinGame(in, out);
                break;
            case 5: // list-rooms
                commandListGames(in, out);
                break;
            case 7:
                commandLogout(in, out);
                break;
            case 8: // quit
            case 9:
                return false;
            default:
                System.out.println(in.readLine());
                break;
        }
        return true;
    }

    private void commandRegister(BufferedReader in, PrintWriter out) throws IOException {
        System.out.println(in.readLine());
    }

    private void commandLogin(BufferedReader in, PrintWriter out) throws IOException {
        String message = in.readLine();
        if (message.equals("You logged in successfully!")) {
            loggedIn = true;
        }
        System.out.println(message);
    }

    private void commandLogout(BufferedReader in, PrintWriter out) throws IOException {
        String message = in.readLine();
        if (message.equals("Logged out successfully!")) {
            loggedIn = false;
        }
        System.out.println(message);
    }

    /**
     * The user has requested a game to be created
     */
    private void commandCreateGame(BufferedReader in, PrintWriter out) throws IOException {
        System.out.println(in.readLine());
    }

    /**
     * The user wants to join a game
     */
    private void commandJoinGame(BufferedReader in, PrintWriter out) throws IOException {
        int flag = readFlag(in);

        if (flag == 0) {
            System.out.println(in.readLine());
            return;
        }
        System.out.println(in.readLine());

        flag = readFlag(in);
        if (flag == 0) {

            return;
        }
        commandJoinGameStartGame(in, out);
        String str = in.readLine();
    }

    /**
     * The entry point of an actual game (in a game room)
     */
    private void commandJoinGameStartGame(BufferedReader in, PrintWriter out) throws IOException {
        System.out.println(in.readLine());
        int playerNumber = readFlag(in);

        board = new Board(BOARD_SQUARES_COUNT);
        ui = new UI(this, playerNumber, BOARD_SQUARES_COUNT);
        ui.setVisible(true);

        boolean running = true;
        while (running) {
            int flag = readFlag(in);

            if (flag == 0) {
                playerTurn(in, out, playerNumber);
            } else if (flag == 1) {
                readOtherPlayerMove(in, out, playerNumber);
            } else if (flag == 2) {
                displayWinMessage();
                running = false;
            } else if (flag == 3) {
                displayLoseMessage();
                running = false;
            }
        }

        ui.setVisible(false);
    }

    /**
     * This player's turn
     */
    private void playerTurn(BufferedReader in, PrintWriter out, int playerNumber) {
        clicked = false;
        while (true) {
            synchronized (clicked) {
                if (clicked) {
                    break;
                }
            }
        }
        ui.drawPiece(clickedX, clickedY, playerNumber);
        out.println(clickedX + " " + clickedY);
        out.flush();
    }

    /**
     * The other player's turn
     */
    private void readOtherPlayerMove(BufferedReader in, PrintWriter out, int playerNumber) throws IOException {
        String strMove = in.readLine();
        Pair<Integer, Integer> move = Helper.splitMoveString(strMove);

        board.addPiece(1 - playerNumber, move.getKey(), move.getValue());
        ui.drawPiece(move.getKey(), move.getValue(), 1 - playerNumber);
    }

    private void displayWinMessage() {
        System.out.println("YOU WON!");
    }

    private void displayLoseMessage() {
        System.out.println("YOU LOST!");
    }

    private void commandListGames(BufferedReader in, PrintWriter out) throws IOException {
        readList(in);
    }

    /**
     * Reads a list over the network
     */
    private void readList(BufferedReader in) throws IOException {
        int number = Integer.parseInt(in.readLine());

        for (int i = 0; i < number; ++i) {
            System.out.println(in.readLine());
        }
    }

    /**
     * Reads a flag which the server sent
     */
    private int readFlag(BufferedReader in) throws IOException {
        return Integer.parseInt(in.readLine());
    }
}
