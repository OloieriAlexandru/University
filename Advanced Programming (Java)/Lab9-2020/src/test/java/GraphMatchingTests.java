import entity.Album;
import graph.Graph;
import org.junit.jupiter.api.Test;

import java.util.ArrayList;
import java.util.List;

import static org.junit.jupiter.api.Assertions.assertEquals;

public class GraphMatchingTests {
    @Test
    void graphMatchingTestRandomGraph() {
        List<Album> albums = new ArrayList<>();
        albums.add(new Album("A1", 1, 2000, 1));
        albums.add(new Album("A2", 1, 2000, 2));
        albums.add(new Album("A3", 2, 2000, 1));

        Graph graph = new Graph(albums);

        assertEquals(2, graph.getLargestSetOfAlbumsWithDiffGenreArtist().size());
    }

    @Test
    void graphMatchingTestAnswerIsOne() {
        List<Album> albums = new ArrayList<>();
        albums.add(new Album("A1", 1, 2000, 1));
        albums.add(new Album("A2", 2, 2000, 1));
        albums.add(new Album("A3", 3, 2000, 1));

        Graph graph = new Graph(albums);

        assertEquals(1, graph.getLargestSetOfAlbumsWithDiffGenreArtist().size());
    }

    @Test
    void graphMatchingTestAnswerIsN() {
        List<Album> albums = new ArrayList<>();
        albums.add(new Album("A1", 1, 2000, 1));
        albums.add(new Album("A2", 2, 2000, 2));
        albums.add(new Album("A3", 3, 2000, 3));

        Graph graph = new Graph(albums);

        assertEquals(3, graph.getLargestSetOfAlbumsWithDiffGenreArtist().size());
    }
}
