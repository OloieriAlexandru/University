import common.Helper;
import game.Game;
import game.arithmetic.ArithmeticProgressionGame;
import game.clique.CliqueGame;
import shell.Command;
import shell.CommandEnum;
import shell.Shell;

import java.util.Scanner;

public class Program {
    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        Shell shell = getShell();
        System.out.println("Commands:");
        System.out.println(shell);

        boolean running = true;
        while (running) {
            String line = Helper.readLine(scanner, "shell>");
            String[] cmdArgs = line.split(" ", 2);
            Command cmd = shell.getCommand(cmdArgs[0]);

            running = executeCommand(shell, cmd);
        }
    }

    /**
     * Executes the command
     */
    private static boolean executeCommand(Shell shell, Command cmd) {
        switch (cmd.getCmdEnum()) {
            case PLAY:
                Game game = initializeGame();
                if (game == null) {
                    System.out.println("Error! Please try again!");
                } else {
                    game.start();
                    game.printWinnerOrRanking();
                }
                break;
            case LIST:
                System.out.println(shell);
                break;
            case QUIT:
                return false;
            default:
                System.out.println("Invalid command!");
        }
        return true;
    }

    /**
     * Reads the game parameters
     */
    private static Game initializeGame() {
        Game game = null;
        Scanner scanner = new Scanner(System.in);

        String gameName = Helper.readStringOption(scanner, "Enter the game you want to play: ", new String[]{"arith", "clique"});

        int n = 0;
        if (gameName.equals("arith")) {
            n = Helper.readIntOption(scanner, "Enter the number of tokens on the table: ");
        } else if (gameName.equals("clique")) {
            n = Helper.readIntOption(scanner, "Enter the number of vertices: ");
        }
        int duration = Helper.readIntOption(scanner, "Enter the duration of the game (in seconds): ");
        int k = 0;
        if (gameName.equals("arith")) {
            k = Helper.readIntOption(scanner, "Enter the target size of the arithmetic progression: ");
        } else if (gameName.equals("clique")) {
            k = Helper.readIntOption(scanner, "Enter the target size of the clique: ");
        }

        if (gameName.equals("arith")) {
            game = new ArithmeticProgressionGame(n, duration, k);
        } else if (gameName.equals("clique")) {
            game = new CliqueGame(n, duration, k);
        }

        initializePlayers(game, scanner, gameName);

        return game;
    }

    /**
     * Adds players to the game
     */
    private static void initializePlayers(Game game, Scanner scanner, String gameName) {
        int numberOfPlayers = 0;
        while (true) {
            numberOfPlayers = Helper.readIntOption(scanner, "Enter the number of players: ");
            if (numberOfPlayers > 0) {
                break;
            }
            System.out.println("The number of player cannot be zero!");
        }
        for (int i = 0; i < numberOfPlayers; ++i) {
            String playerType = null;
            if (gameName.equals("arith")) {
                playerType = Helper.readStringOption(scanner, "Enter the player type: ", new String[]{"smart", "random", "manual"});
            } else if (gameName.equals("clique")) {
                playerType = Helper.readStringOption(scanner, "Enter the player type: ", new String[]{"random", "manual"});
            }
            System.out.print("Enter player name: ");
            String playerName = scanner.nextLine();

            game.addPlayer(playerType, playerName);
        }
    }

    /**
     * Initializes the shell. The shell structure was copied from lab. 5
     */
    private static Shell getShell() {
        Shell shell = new Shell();

        shell.addCommand(new Command(CommandEnum.PLAY, "play"));
        shell.addCommand(new Command(CommandEnum.LIST, "list"));
        shell.addCommand(new Command(CommandEnum.QUIT, "quit"));

        return shell;
    }
}
