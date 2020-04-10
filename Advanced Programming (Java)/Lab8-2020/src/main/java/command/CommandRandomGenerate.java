package command;

import com.github.javafaker.Faker;
import common.Helper;
import database.connection.Database;
import database.dao.*;
import exception.AlbumCreationFailedException;
import exception.ArtistCreationFailedException;
import exception.ChartCreationFailedException;
import model.*;

import java.util.ArrayList;
import java.util.List;

public class CommandRandomGenerate {
    public void execute() {
        int numberOfArtists = 5;
        int numberOfAlbums = 50;
        int numberOfCharts = 5;
        int minNumberOfAlbumsInChart = 5;
        int maxNumberOfAlbumsInChart = 10;

        Faker faker = new Faker();

        List<model.Artist> artists = generateArtists(faker, numberOfArtists);
        List<Album> albums = generateAlbums(faker, numberOfAlbums, artists);
        List<Chart> charts = generateCharts(faker, numberOfCharts, minNumberOfAlbumsInChart, maxNumberOfAlbumsInChart, albums);

        System.out.println("Generated " + artists.size() + " artists");
        System.out.println("Generated " + albums.size() + " albums");
        System.out.println("Generated " + charts.size() + " charts");
    }

    /**
     * Generates 'numberOfArtists' random artists
     */
    private List<model.Artist> generateArtists(Faker faker, int numberOfArtists) {
        Database database = Database.getInstance();
        ArtistController artistController = new ArtistController(database);

        List<model.Artist> artists = new ArrayList<>();
        for (int i = 1; i <= numberOfArtists; ++i) {
            String name = faker.name().fullName();
            String country = faker.country().name();
            try {
                model.Artist artist = artistController.create(name, country);
                artists.add(artist);
            } catch (ArtistCreationFailedException e) {
                System.out.println(e.getMessage());
            }
        }

        return artists;
    }

    /**
     * Generates 'numberOfAlbums' random albums
     */
    private List<Album> generateAlbums(Faker faker, int numberOfAlbums, List<Artist> artists) {
        Database database = Database.getInstance();
        AlbumController albumController = new AlbumController(database);

        List<Album> albums = new ArrayList<>();
        for (int i = 1; i <= numberOfAlbums; ++i) {
            int artistId = artists.get(faker.random().nextInt(artists.size())).getId();
            int year = faker.random().nextInt(1950, 2020);

            try {
                Album album = albumController.create(faker.book().title(), artistId, year);
                albums.add(album);
            } catch (AlbumCreationFailedException e) {
                System.out.println(e.getMessage());
            }
        }

        return albums;
    }

    /**
     * Generates 'numberOfCharts' charts, each of them having between 'minNumberOfAlbumsInChart' and 'maxNumberOfAlbumsInChart' albums
     */
    private List<Chart> generateCharts(Faker faker, int numberOfCharts, int minNumberOfAlbumsInChart, int maxNumberOfAlbumsInChart,
                                       List<Album> albums) {
        Database database = Database.getInstance();
        ChartController chartController = new ChartController(database);

        List<Integer> permutation = generatePermutation(albums.size());
        List<Chart> charts = new ArrayList<>();
        for (int i = 1; i <= numberOfCharts; ++i) {
            int numberOfAlbumsInThisChart = faker.random().nextInt(minNumberOfAlbumsInChart, maxNumberOfAlbumsInChart);
            Helper.shufflePermutation(permutation);

            List<Album> thisChartAlbums = new ArrayList<>();
            for (int j = 0; j < numberOfAlbumsInThisChart; ++j) {
                thisChartAlbums.add(albums.get(permutation.get(j)));
            }
            try {
                Chart chart = chartController.create(faker.company().name(), thisChartAlbums);
                charts.add(chart);
            } catch (ChartCreationFailedException e) {
                System.out.println(e.getMessage());
            }
        }

        return charts;
    }

    /**
     * Generates the identity permutation
     */
    private List<Integer> generatePermutation(int numberOfElements) {
        List<Integer> permutation = new ArrayList<>();

        for (int i = 0; i < numberOfElements; ++i) {
            permutation.add(i);
        }

        return permutation;
    }
}
