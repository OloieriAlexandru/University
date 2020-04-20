package command;

import entity.Album;
import graph.Graph;
import repo.hibernate.AlbumRepository;

import java.util.List;

public class CommandGetLargestSetOfAlbums {
    /**
     * In this function a graph is constructed using all the albums from the database and then
     * the largest set of albums such that no two albums have the same artist or belong to the same genre
     * is printed to stdout
     */
    public void execute() {
        AlbumRepository albumRepository = new AlbumRepository();
        List<Album> albums = albumRepository.findAll();

        List<Album> largestSetOfAlbums = new Graph(albums).getLargestSetOfAlbumsWithDiffGenreArtist();

        System.out.println("Largest set of albums such that no two albums have the same artist or belong to the same genre:");
        for (Album album : largestSetOfAlbums) {
            System.out.println(album);
        }
        System.out.println(largestSetOfAlbums.size() + " albums!");
    }
}
