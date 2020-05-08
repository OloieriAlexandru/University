package server;

import cmd.GameServerCommands;
import dtos.UserCredentialsDTO;
import dtos.UserToken;
import org.springframework.http.ResponseEntity;
import org.springframework.web.client.HttpClientErrorException;
import util.Helper;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.Socket;
import java.net.SocketTimeoutException;

public class ClientThread implements Runnable {
    private Socket socket;
    private GomokuServer gomokuServer;
    private GameServerCommands networkCommands;
    private boolean loggedIn;
    private String jwtToken;
    private String username;

    public ClientThread(Socket sd, GameServerCommands gameServerCommands, GomokuServer server) {
        socket = sd;
        networkCommands = gameServerCommands;
        gomokuServer = server;
        loggedIn = false;
    }

    /**
     * This thread is associated with a single client. It solves the client's requests
     */
    @Override
    public void run() {
        try {
            BufferedReader in = new BufferedReader(new InputStreamReader(socket.getInputStream()));
            PrintWriter out = new PrintWriter(socket.getOutputStream());
            String request;
            boolean clientThreadRunning = true;

            networkCommands.writeToNetworkCommands(out, loggedIn);

            while (clientThreadRunning) {
                request = in.readLine();
                clientThreadRunning = solveRequest(in, out, request);
            }
        } catch (SocketTimeoutException ignored) {

        } catch (IOException e) {
            System.out.println("Communication error... " + e.getMessage());
        } finally {
            try {
                socket.close();
            } catch (IOException e) {
                System.out.println(e.getMessage());
            }
        }
    }

    /**
     * Solves the request sent by the client associated with this thread
     */
    private boolean solveRequest(BufferedReader in, PrintWriter out, String request) {
        int requestIndex = networkCommands.getCommandIndex(request, loggedIn);
        String arguments;

        if (requestIndex < networkCommands.getCommandsCount()) {
            out.println(requestIndex);
        }

        switch (requestIndex) {
            case 0: // register
                arguments = networkCommands.getArguments(request, 0);
                commandRegister(in, out, arguments);
                break;
            case 1: // login
                arguments = networkCommands.getArguments(request, 1);
                commandLogin(in, out, arguments);
                break;
            case 2: // create
                arguments = networkCommands.getArguments(request, 2);
                commandCreate(in, out, arguments);
                break;
            case 3: // create ai game
                arguments = networkCommands.getArguments(request, 3);
                commandCreateAI(in, out, arguments);
                break;
            case 4: // join
                arguments = networkCommands.getArguments(request, 4);
                if (commandJoin(in, out, arguments)) {
                    return true;
                }
                break;
            case 5: // list-rooms
                commandList(in, out);
                break;
            case 7: // logout
                loggedIn = false;
                jwtToken = null;
                out.println("Logged out successfully!");
                break;
            case 8: // quit
            case 9:
                return false;
            default:
                out.println("Invalid command!");
                break;
        }
        out.flush();
        return true;
    }

    private void commandRegister(BufferedReader in, PrintWriter out, String arg) {
        String[] args = Helper.splitString(arg);
        if (args[0].length() == 0) {
            out.println("You have to provide an username and a password!");
            return;
        }
        String[] credentials = args[0].split("\\s+");
        if (credentials.length < 2) {
            out.println("You have to provide an username and a password!");
            return;
        }

        ResponseEntity<String> resp = null;
        try {
            resp = Helper.post("players", new UserCredentialsDTO(credentials[0], credentials[1]).asJSON(), null);
        } catch (HttpClientErrorException e) {
            out.println(e.getResponseBodyAsString());
            return;
        }

        out.println("Account created successfully! Now you can login!");
    }

    private void commandLogin(BufferedReader in, PrintWriter out, String arg) {
        String[] args = Helper.splitString(arg);
        if (args[0].length() == 0) {
            out.println("You have to provide an username and a password!");
            return;
        }
        String[] credentials = args[0].split("\\s+");
        if (credentials.length < 2) {
            out.println("You have to provide an username and a password!");
            return;
        }

        ResponseEntity<String> resp = null;
        try {
            resp = Helper.post("auth", new UserCredentialsDTO(credentials[0], credentials[1]).asJSON(), null);
            loggedIn = true;
            jwtToken = UserToken.buildFromJson(resp.getBody()).getToken();
            username = Helper.getUsernameFromJwt(jwtToken);
        } catch (HttpClientErrorException e) {
            out.println(e.getResponseBodyAsString());
            return;
        }

        out.println("You logged in successfully!");
    }

    /**
     * The client requested to create a room
     */
    private void commandCreateAI(BufferedReader in, PrintWriter out, String arg) {
        System.out.println("AI GAME");
        String[] args = Helper.splitString(arg);
        if (args[0].length() == 0) {
            out.println("You have to specify the name of the room!");
            return;
        }

        String roomId = gomokuServer.createVsComputerGame(arg);
        out.println("Room created! Its id is \"" + roomId + "\"");
    }

    private void commandCreate(BufferedReader in, PrintWriter out, String arg) {
        String[] args = Helper.splitString(arg);
        if (args[0].length() == 0) {
            out.println("You have to specify the name of the room!");
            return;
        }

        String roomId = gomokuServer.createGame(arg);
        out.println("Room created! Its id is \"" + roomId + "\"");
    }

    /**
     * The client requested to join a room
     */
    private boolean commandJoin(BufferedReader in, PrintWriter out, String arg) {
        String[] args = Helper.splitString(arg);
        if (args[0].length() == 0) {
            out.println(0);
            out.println("You have to specify the id of the room!");
            return false;
        }

        GomokuGame game = gomokuServer.joinRoom(args[0], socket, in, out, username);
        if (game == null) {
            out.println("Failed to join room!");
            return false;
        }

        commandJoinSleep(in, out, game);

        return true;
    }

    /**
     * This thread will wait until the game will end and then will continue
     * solving client's requests
     */
    private void commandJoinSleep(BufferedReader in, PrintWriter out, GomokuGame game) {
        out.println(1);
        if (game.isVersusAI()) {
            out.println("The game will start immediately!");
        } else {
            out.println("The game will start when there will be two players in the room!");
        }
        out.flush();

        gomokuServer.checkIfGameHasToStart(game.getId());

        Object lock = game.getLock(socket);
        try {
            synchronized (lock) {
                lock.wait();
            }
        } catch (InterruptedException e) {
            e.printStackTrace();
        }

        out.println("The game has ended!");
        out.flush();
    }

    /**
     * The client requested to see a list with all the available commands
     */
    private void commandList(BufferedReader in, PrintWriter out) {
        gomokuServer.writeToNetworkRooms(out);
    }
}
