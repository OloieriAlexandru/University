package app;

import command.CommandAbstractFactoryDemo;
import command.CommandChartsDemo;
import command.CommandGetLargestSetOfAlbums;
import command.CommandRandomGenerate;
import entity.Album;
import entity.Artist;
import entity.GenreEnum;
import repo.hibernate.AlbumRepository;
import repo.hibernate.ArtistRepository;
import shell.Command;
import shell.CommandEnum;
import shell.Shell;
import util.Helper;
import util.PersistenceUtil;

import java.util.List;
import java.util.Scanner;

public class AlbumManager {
    public static void main(String[] args) {
        PersistenceUtil.getInstance();
        Scanner scanner = new Scanner(System.in);
        Shell shell = getShell();
        System.out.println("Commands:");
        System.out.println(shell);

        boolean running = true;
        while (running) {
            String line = Helper.readLine(scanner, "shell>");
            String[] cmdArgs = line.split(" ", 2);
            Command cmd = shell.getCommand(cmdArgs[0]);

            if (cmd == null) {
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
                compulsoryCommand();
                break;
            case CHARTS_DEMO:
                chartsDemoCommand();
                break;
            case ABSTRACT_FACTORY:
                abstractFactoryDemoCommand();
                break;
            case GENERATE:
                generateCommand();
                break;
            case LARGEST_ALBUMS_SET:
                getLargestSetOfAlbumsCommand();
                break;
            case LIST:
                System.out.println(shell);
                break;
            case QUIT:
                return false;
        }
        return true;
    }

    /**
     * All the logic for testing the compulsory tasks
     */
    private static void compulsoryCommand() {
        ArtistRepository artistRepository = new ArtistRepository();
        AlbumRepository albumRepository = new AlbumRepository();

        Artist artist1 = artistRepository.create(new Artist("Eminem", "SUA"));
        Helper.printDatabaseArtistInsertEvent(artist1);

        Album album1 = albumRepository.create(new Album("Revival", artist1.getId(), 2017, GenreEnum.HIP_HOP));
        Helper.printDatabaseAlbumInsertEvent(album1);
        Album album2 = albumRepository.create(new Album("Recovery", artist1.getId(), 2010, GenreEnum.HIP_HOP));
        Helper.printDatabaseAlbumInsertEvent(album2);
        Album album3 = albumRepository.create(new Album("Relapse", artist1.getId(), 2009, GenreEnum.HIP_HOP));
        Helper.printDatabaseAlbumInsertEvent(album3);
        Album album4 = albumRepository.create(new Album("The Eminem Show", artist1.getId(), 2002, GenreEnum.HIP_HOP));
        Helper.printDatabaseAlbumInsertEvent(album4);

        Artist artist2 = artistRepository.create(new Artist("B.U.G Mafia", "Romania"));
        Helper.printDatabaseArtistInsertEvent(artist2);

        Album album5 = albumRepository.create(new Album("De cartier", artist2.getId(), 1998, GenreEnum.HIP_HOP));
        Helper.printDatabaseAlbumInsertEvent(album5);
        Album album6 = albumRepository.create(new Album("Un 2 si trei de 0", artist2.getId(), 2000, GenreEnum.HIP_HOP));
        Helper.printDatabaseAlbumInsertEvent(album6);
        Album album7 = albumRepository.create(new Album("B.U.G. Mafia prezinta CASA", artist2.getId(), 2002, GenreEnum.HIP_HOP));
        Helper.printDatabaseAlbumInsertEvent(album7);

        System.out.println("Artist.findById:");
        System.out.println(artistRepository.findById(artist1.getId()));

        System.out.println("Album.findById:");
        System.out.println(albumRepository.findById(album1.getId()));

        System.out.println("Album.findByArtist:\nEminem albums:");
        List<Album> eminemAlbums = albumRepository.findByArtist(artist1);
        for (Album album : eminemAlbums) {
            System.out.println(album);
        }
    }

    /**
     * This commands tests the first task from "Optional" section
     */
    private static void chartsDemoCommand() {
        new CommandChartsDemo().execute();
    }

    private static void abstractFactoryDemoCommand() {
        new CommandAbstractFactoryDemo().execute();
    }

    /**
     * This command tests the second task from "Bonus" section
     */
    private static void generateCommand() {
        new CommandRandomGenerate().execute();
    }

    /**
     * This command tests the third task from "Bonus" section
     */
    private static void getLargestSetOfAlbumsCommand() {
        new CommandGetLargestSetOfAlbums().execute();
    }

    /**
     * Initializes the shell. The shell structure was copied from lab. 5
     */
    private static Shell getShell() {
        Shell shell = new Shell();

        shell.addCommand(new Command(CommandEnum.COMPULSORY, "compulsory"));
        shell.addCommand(new Command(CommandEnum.CHARTS_DEMO, "charts-demo"));
        shell.addCommand(new Command(CommandEnum.ABSTRACT_FACTORY, "abstract-factory-demo"));
        shell.addCommand(new Command(CommandEnum.GENERATE, "generate"));
        shell.addCommand(new Command(CommandEnum.LARGEST_ALBUMS_SET, "largest-albums-set"));
        shell.addCommand(new Command(CommandEnum.LIST, "list"));
        shell.addCommand(new Command(CommandEnum.QUIT, "quit"));

        return shell;
    }
}
