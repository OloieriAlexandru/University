package command;

import entity.Artist;
import repo.ArtistRepo;
import repo.ArtistRepoFactory;
import util.Helper;
import util.PropertyValues;

import java.util.Scanner;

public class CommandAbstractFactoryDemo {
    /**
     * Creates an ArtistRepository based on a parameter given in a configuration file
     * And inserts into database a new artist
     */
    public void execute() {
        Scanner scanner = new Scanner(System.in);
        ArtistRepoFactory repoFactory = new ArtistRepoFactory();
        String type = new PropertyValues().getDBConnectionType();
        ArtistRepo artistRepo = repoFactory.create(type);

        String artistName = Helper.readLine(scanner, "Enter the name of the artist: ");
        String artistCountry = Helper.readLine(scanner, "Enter the country of the artist: ");

        Artist createdArtist = new Artist(artistName, artistCountry);
        createdArtist = artistRepo.create(createdArtist);

        System.out.println("Inserted " + createdArtist + " in the database using " + type + " connection!");
    }
}
