import command.CommandChart;
import command.CommandConcurrentTasks;
import command.CommandRandomGenerate;
import common.Helper;
import database.connection.ConnectionSourceConnPool;
import database.connection.ConnectionSourceSingleton;
import database.connection.Database;
import database.dao.AlbumController;
import database.dao.ArtistController;
import model.Album;
import model.Artist;
import shell.*;

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

            if (cmd == null){
                System.out.println("Invalid command!");
                continue;
            }

            running = executeCommand(shell, cmd);
        }
    }

    /**
     * Executes the command
     */
    private static boolean executeCommand(Shell shell, Command cmd) {
        switch (cmd.getCmdEnum()) {
            case COMPULSORY:
                commandCompulsory();
                break;
            case GENERATE:
                commandGenerate();
                break;
            case RANK_ARTISTS:
                commandRankArtists();
                break;
            case RANK_ARTISTS_HTML:
                commandRankArtistsHTML();
                break;
            case CONCURRENT_SINGLETON:
                commandConcurrentTasksSingleton();
                break;
            case CONCURRENT_CONN_POOL:
                commandConcurrentTasksConnPool();
                break;
            case LIST:
                System.out.println(shell);
                break;
            case QUIT:
                return false;
        }
        return true;
    }

    private static void commandCompulsory() {
        Database database = Database.getInstance();

        ArtistController artistController = new ArtistController(database);
        AlbumController albumController = new AlbumController(database);

        try {
            Artist artist1 = artistController.create("Eminem", "SUA");
            Helper.printDatabaseArtistInsertEvent(artist1);

            Album album1 = albumController.create("Revival", artist1.getId(), 2017);
            Helper.printDatabaseAlbumInsertEvent(album1);
            Album album2 = albumController.create("Recovery", artist1.getId(), 2010);
            Helper.printDatabaseAlbumInsertEvent(album2);
            Album album3 = albumController.create("Relapse", artist1.getId(), 2009);
            Helper.printDatabaseAlbumInsertEvent(album3);
            Album album4 = albumController.create("The Eminem Show", artist1.getId(), 2002);
            Helper.printDatabaseAlbumInsertEvent(album4);

            Artist artist2 = artistController.create("B.U.G Mafia", "Romania");
            Helper.printDatabaseArtistInsertEvent(artist2);

            Album album5 = albumController.create("De cartier", artist2.getId(), 1998);
            Helper.printDatabaseAlbumInsertEvent(album5);
            Album album6 = albumController.create("Un 2 si trei de 0", artist2.getId(), 2000);
            Helper.printDatabaseAlbumInsertEvent(album6);
            Album album7 = albumController.create("B.U.G. Mafia prezinta CASA", artist2.getId(), 2002);
            Helper.printDatabaseAlbumInsertEvent(album7);
        } catch (Exception e) {
            System.out.println(e.getMessage());
        }
    }

    private static void commandGenerate() {
        new CommandRandomGenerate().execute();
    }

    private static void commandRankArtists() {
        new CommandChart().execute();
    }

    private static void commandRankArtistsHTML() {
        new CommandChart().executeHTML();
    }

    private static void commandConcurrentTasksSingleton() {
        CommandConcurrentTasks concurrentTasks = new CommandConcurrentTasks(new ConnectionSourceSingleton());
        concurrentTasks.generateThreads(500);
    }

    private static void commandConcurrentTasksConnPool() {
        CommandConcurrentTasks concurrentTasks = new CommandConcurrentTasks(new ConnectionSourceConnPool());
        concurrentTasks.generateThreads(500);
    }

    /**
     * Initializes the shell. The shell structure was copied from lab. 5
     */
    private static Shell getShell() {
        Shell shell = new Shell();

        shell.addCommand(new Command(CommandEnum.COMPULSORY, "compulsory"));
        shell.addCommand(new Command(CommandEnum.GENERATE, "generate"));
        shell.addCommand(new Command(CommandEnum.RANK_ARTISTS, "rank"));
        shell.addCommand(new Command(CommandEnum.RANK_ARTISTS_HTML, "rank_html"));
        shell.addCommand(new Command(CommandEnum.CONCURRENT_SINGLETON, "c_singleton"));
        shell.addCommand(new Command(CommandEnum.CONCURRENT_CONN_POOL, "c_conn_pool"));
        shell.addCommand(new Command(CommandEnum.LIST, "list"));
        shell.addCommand(new Command(CommandEnum.QUIT, "quit"));

        return shell;
    }
}
