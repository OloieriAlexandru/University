package server;

import com.jcraft.jsch.*;
import game.Game;
import util.Helper;
import util.Pair;

import java.io.*;
import java.net.Socket;
import java.util.ArrayList;
import java.util.List;

public class GomokuGame {
    private GomokuServer server;
    private String id;
    private String name;
    private List<BufferedReader> readers;
    private List<PrintWriter> writers;
    private List<Socket> playersSockets;
    private List<Object> locks;

    public GomokuGame(GomokuServer server, String gameId, String gameName) {
        this.server = server;
        id = gameId;
        name = gameName;
        readers = new ArrayList<>();
        writers = new ArrayList<>();
        playersSockets = new ArrayList<>();

        locks = new ArrayList<>();
        for (int i = 0; i < 2; ++i) {
            locks.add(new Object());
        }
    }

    /**
     * This function is used by a client thread to get its lock and to sleep until the game will end
     */
    public synchronized Object getLock(Socket socket) {
        for (int i = 0; i < playersSockets.size(); ++i) {
            if (playersSockets.get(i).equals(socket)) {
                return locks.get(i);
            }
        }
        return null;
    }

    /**
     * The main loop of a room game
     */
    public void startGame() throws IOException {
        notifyPlayersThatTheGameHasStarted();
        Game game = new Game(19);

        int currentPlayer = 0;
        while (true) {
            notifyPlayerAboutHisTurn(currentPlayer);
            Pair<Integer, Integer> move = Helper.splitMoveString(readers.get(currentPlayer).readLine());

            boolean gameEnded = game.addPiece(currentPlayer, move.getKey(), move.getValue());

            if (gameEnded) {
                notifyPlayerThatHeHasWon(currentPlayer);
                notifyPlayerThatHeHasLost(1 - currentPlayer);
                break;
            }

            notifyOtherPlayerAboutCurrentPlayersMove(1 - currentPlayer, move.getKey(), move.getValue());
            currentPlayer = (currentPlayer + 1) % 2;
        }

        sendHTMLRepresentationToServer(game.getAsHTML());
    }

    private void notifyPlayerAboutHisTurn(int player) {
        PrintWriter writer = writers.get(player);
        writer.println(0);
        writer.flush();
    }

    private void notifyOtherPlayerAboutCurrentPlayersMove(int player, int x, int y) {
        PrintWriter writer = writers.get(player);
        writer.println(1);
        writer.println(x + " " + y);
        writer.flush();
    }

    private void notifyPlayerThatHeHasWon(int player) {
        PrintWriter writer = writers.get(player);
        writer.println(2);
        writer.flush();
    }

    private void notifyPlayerThatHeHasLost(int player) {
        PrintWriter writer = writers.get(player);
        writer.println(3);
        writer.flush();
    }

    /**
     * Notifies the players that the game has started
     */
    private void notifyPlayersThatTheGameHasStarted() {
        for (int i = 0; i < writers.size(); ++i) {
            writers.get(i).println(1);
            writers.get(i).println("The game has started!");
            writers.get(i).println(i);
            writers.get(i).flush();
        }
    }

    /**
     * Opens a SFTP connection with a server and
     * Creates a file with the name "gomoku_game_{{gameId}}.html"
     */
    private void sendHTMLRepresentationToServer(String html) {
        String user = "root";
        String host = "157.245.121.183";
        String password = "-";
        InputStream inputStream = new ByteArrayInputStream(html.getBytes());

        try {
            JSch jsch = new JSch();
            Session session = jsch.getSession(user, host);

            java.util.Properties config = new java.util.Properties();
            config.put("StrictHostKeyChecking", "no");
            session.setConfig(config);

            session.setPassword(password);
            session.connect();

            ChannelSftp sftpChannel = (ChannelSftp) session.openChannel("sftp");
            sftpChannel.connect();

            sftpChannel.put(inputStream, "./gomoku/gomoku_game_" + id + ".html");

            sftpChannel.disconnect();
            session.disconnect();
        } catch (JSchException | SftpException e) {
            e.printStackTrace();
        }
    }

    /**
     * toString over network
     */
    public void writeToNetworkRoom(int index, PrintWriter out) {
        out.println(index + ". name: " + name + ", id: " + id);
    }

    /**
     * Adds a new player to this room
     */
    public void addPlayer(Socket player, BufferedReader reader, PrintWriter writer) {
        readers.add(reader);
        writers.add(writer);
        playersSockets.add(player);
    }

    public List<BufferedReader> getReaders() {
        return readers;
    }

    public List<PrintWriter> getWriters() {
        return writers;
    }

    public int getNumberOfPlayers() {
        return playersSockets.size();
    }

    public String getId() {
        return id;
    }

    public synchronized List<Object> getLocks() {
        return locks;
    }

    public GomokuServer getServer() {
        return server;
    }
}
