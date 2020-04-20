package command;

import com.github.javafaker.Faker;
import entity.Album;
import entity.Artist;
import repo.hibernate.AlbumRepository;
import repo.hibernate.ArtistRepository;

import java.util.ArrayList;
import java.util.List;

public class CommandRandomGenerate {
    /**
     * Generates random artists and albums
     */
    public void execute() {
        int numberOfArtists = 5;
        int numberOfAlbums = 50;

        Faker faker = new Faker();

        List<Artist> artists = generateArtists(faker, numberOfArtists);
        List<Album> albums = generateAlbums(faker, numberOfAlbums, artists);

        System.out.println("Generated " + artists.size() + " artists");
        for (Artist artist : artists) {
            System.out.println(artist);
        }

        System.out.println("Generated " + albums.size() + " albums");
        for (Album album : albums) {
            System.out.println(album);
        }
    }

    /**
     * Generates 'numberOfArtists' random artists
     */
    private List<entity.Artist> generateArtists(Faker faker, int numberOfArtists) {
        ArtistRepository artistRepository = new ArtistRepository();

        List<entity.Artist> artists = new ArrayList<>();
        for (int i = 1; i <= numberOfArtists; ++i) {
            String name = faker.name().fullName();
            String country = faker.country().name();
            Artist artist = artistRepository.create(new Artist(name, country));
            artists.add(artist);
        }

        return artists;
    }

    /**
     * Generates 'numberOfAlbums' random albums
     */
    private List<Album> generateAlbums(Faker faker, int numberOfAlbums, List<Artist> artists) {
        AlbumRepository albumRepository = new AlbumRepository();

        List<Album> albums = new ArrayList<>();
        for (int i = 1; i <= numberOfAlbums; ++i) {
            int artistId = artists.get(faker.random().nextInt(artists.size())).getId();
            int year = faker.random().nextInt(1950, 2020);
            Album album = albumRepository.create(new Album(faker.book().title(), artistId, year, faker.random().nextInt(1, 11)));
            albums.add(album);
        }

        return albums;
    }
}
