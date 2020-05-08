package server;

import dtos.GameCreateDTO;
import dtos.GameUpdateDTO;
import org.springframework.http.ResponseEntity;
import org.springframework.web.client.HttpClientErrorException;
import util.Helper;

import java.io.BufferedReader;
import java.io.PrintWriter;
import java.net.Socket;
import java.util.ArrayList;
import java.util.List;

/**
 * Manages all the rooms of the server
 */
public class GomokuServer {
    private List<GomokuGame> games = null;

    public GomokuServer() {
        games = new ArrayList<>();
    }

    /**
     * Writes to a client all the available rooms
     */
    public void writeToNetworkRooms(PrintWriter out) {
        synchronized (games) {
            if (games.size() == 0) {
                out.println(1);
                out.println("There are no rooms!");
                return;
            }

            out.println(games.size());

            for (int i = 0; i < games.size(); ++i) {
                games.get(i).writeToNetworkRoom(i + 1, out);
            }
        }
    }

    /**
     * Creates a new game room
     */
    public String createGame(String gameName) {
        String randomId = Helper.getRandomHexString(6);
        synchronized (games) {
            games.add(new GomokuGame(this, randomId, gameName, false));
            games.get(games.size() - 1).setUpdateDTO(sendGameToServer(gameName, randomId));
        }
        return randomId;
    }

    public String createVsComputerGame(String gameName) {
        String randomId = Helper.getRandomHexString(6);
        synchronized (games) {
            games.add(new GomokuGame(this, randomId, gameName, true));
            games.get(games.size() - 1).setUpdateDTO(sendGameToServer(gameName, randomId));
            games.get(games.size() - 1).setPlayer2();
        }
        return randomId;
    }

    private GameUpdateDTO sendGameToServer(String name, String randomId) {
        ResponseEntity<String> resp = null;
        try {
            resp = Helper.post("games", new GameCreateDTO(name, randomId).asJSON(), Helper.adminJwt);
            return GameUpdateDTO.buildFromJson(resp.getBody());
        } catch (HttpClientErrorException e) {
            return null;
        }
    }

    /**
     * After a game has ended, this function will be called to remove that room
     */
    public void removeGameFromList(GomokuGame game) {
        synchronized (games) {
            games.remove(game);
        }
    }

    /**
     * A function used by client threads to add a player to a room
     */
    public GomokuGame joinRoom(String gameId, Socket socket, BufferedReader reader, PrintWriter writer, String playerUsername) {
        GomokuGame game = getGameById(gameId);

        if (game == null || game.getNumberOfPlayers() == 2) {
            return null;
        }

        game.addPlayer(socket, reader, writer, playerUsername);
        return game;
    }

    /**
     * Checks whether the room having id "gameId" has two players
     */
    public void checkIfGameHasToStart(String gameId) {
        GomokuGame game = getGameById(gameId);
        if (game == null) {
            return;
        }

        if ((game.getNumberOfPlayers() == 2 && !game.isVersusAI()) || (game.getNumberOfPlayers() == 1 && game.isVersusAI())) {
            System.out.println("Game started");
            new Thread(new GomokuRoom(game)).start();
        }
    }

    private GomokuGame getGameById(String gameId) {
        synchronized (games) {
            for (GomokuGame game : games) {
                if (game.getId().equalsIgnoreCase(gameId)) {
                    return game;
                }
            }
        }
        return null;
    }
}
