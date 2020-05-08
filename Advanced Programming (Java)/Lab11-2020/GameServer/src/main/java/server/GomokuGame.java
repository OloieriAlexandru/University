package server;

import com.jcraft.jsch.*;
import dtos.GameUpdateDTO;
import dtos.GomokuAiDTO;
import dtos.GomokuAiResponseDTO;
import game.Game;
import org.springframework.http.ResponseEntity;
import org.springframework.web.client.HttpClientErrorException;
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
    private GameUpdateDTO gameUpdateDTO;
    private boolean versusAI;

    public GomokuGame(GomokuServer server, String gameId, String gameName, boolean ai) {
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

        versusAI = ai;
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
        if (versusAI) {
            startAIGame();
            return;
        }
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

        if (currentPlayer == 0) {
            gameUpdateDTO.setWinner(gameUpdateDTO.getPlayer1Username());
        } else {
            gameUpdateDTO.setWinner(gameUpdateDTO.getPlayer2Username());
        }
        sendUpdateToServer();
        sendHTMLRepresentationToServer(game.getAsHTML());
    }

    private void startAIGame() throws IOException {
        notifyPlayersThatTheGameHasStarted();
        Game game = new Game(19);

        int currentPlayer = 0;
        while (true) {
            Pair<Integer, Integer> move = null;

            if (currentPlayer == 0) {
                notifyPlayerAboutHisTurn(currentPlayer);
                move = Helper.splitMoveString(readers.get(currentPlayer).readLine());
            } else {
                move = getAINextMove(game);
            }

            boolean gameEnded = game.addPiece(currentPlayer, move.getKey(), move.getValue());

            if (gameEnded) {
                if (currentPlayer == 0) {
                    notifyPlayerThatHeHasWon(0);
                } else {
                    notifyPlayerThatHeHasLost(0);
                }
                break;
            }

            if (currentPlayer == 1) {
                notifyOtherPlayerAboutCurrentPlayersMove(1 - currentPlayer, move.getKey(), move.getValue());
            }
            currentPlayer = (currentPlayer + 1) % 2;
        }

        if (currentPlayer == 0) {
            gameUpdateDTO.setWinner(gameUpdateDTO.getPlayer1Username());
        } else {
            gameUpdateDTO.setWinner("computer");
        }
        sendUpdateToServer();
        sendHTMLRepresentationToServer(game.getAsHTML());
    }

    private Pair<Integer, Integer> getAINextMove(Game game) {
        GomokuAiDTO gomokuAiDTO = new GomokuAiDTO(1, 19, game.getPieces());

        ResponseEntity<String> resp = null;
        try {
            resp = Helper.post("games/ai", gomokuAiDTO.asJSON(), Helper.adminJwt);
        } catch (HttpClientErrorException e) {
            return null;
        }
        if (resp == null) {
            return new Pair<>(1, 1);
        }
        GomokuAiResponseDTO response = GomokuAiResponseDTO.buildFromJson(resp.getBody());
        return new Pair<>(response.getLine(), response.getColumn());
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

    public void setPlayer2() {
        gameUpdateDTO.setPlayer2Username("computer");
        sendUpdateToServer();
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

    void sendUpdateToServer() {
        try {
            Helper.put("games/" + gameUpdateDTO.getRandomId(), gameUpdateDTO.asJSON(), Helper.adminJwt);
        } catch (HttpClientErrorException ignored) {
        }
    }

    void setUpdateDTO(GameUpdateDTO gameDTO) {
        this.gameUpdateDTO = gameDTO;
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
    public void addPlayer(Socket player, BufferedReader reader, PrintWriter writer, String playerUsername) {
        readers.add(reader);
        writers.add(writer);
        playersSockets.add(player);

        if (playersSockets.size() == 1) {
            gameUpdateDTO.setPlayer1Username(playerUsername);
        } else {
            gameUpdateDTO.setPlayer2Username(playerUsername);
        }
        sendUpdateToServer();
    }

    public List<BufferedReader> getReaders() {
        return readers;
    }

    public List<PrintWriter> getWriters() {
        return writers;
    }

    public int getNumberOfPlayers() {
        return writers.size();
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

    public boolean isVersusAI() {
        return versusAI;
    }
}
