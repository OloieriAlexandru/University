package graph;

import entity.Album;

import java.util.*;

public class Graph {
    private Map<Integer, Integer> artistMap = new HashMap<>();
    private Map<Integer, Integer> genreMap = new HashMap<>();
    private List<Node> nodes = new ArrayList<>();
    private List<Album> albums;
    private int secondStableSetCount = 0;

    /**
     * This constructor receives as a parameter a list of albums and builds a bipartite graph
     * In which the first stable set is the list of artist ids and the second stable set is the list of genres.
     */
    public Graph(List<Album> albums) {
        for (Album album : albums) {
            int artistNode = getArtistNode(album.getArtistId());
            int genreNode = getGenreNode(album.getGenreId());
            nodes.get(artistNode).addNodes(genreNode);
        }
        this.albums = albums;
    }

    /**
     * The largest set of albums such that no two albums have the same artist or belong to the same genre
     * Will be computed using the Hopcroft-Karp algorigm (https://en.wikipedia.org/wiki/Hopcroft%E2%80%93Karp_algorithm),
     * An algorithm that computes the max bipartite matching
     */
    public List<Album> getLargestSetOfAlbumsWithDiffGenreArtist() {
        List<Album> albums = new ArrayList<>();

        int[] leftMatching = new int[nodes.size()];
        Arrays.fill(leftMatching, -1);
        int[] rightMatching = new int[secondStableSetCount];
        Arrays.fill(rightMatching, -1);
        boolean[] visited = new boolean[nodes.size()];

        boolean running = true;
        while (running) {
            running = false;
            Arrays.fill(visited, false);
            for (int i = 0; i < leftMatching.length; ++i) {
                if (!visited[i] && leftMatching[i] == -1) {
                    if (couple(i, leftMatching, rightMatching, visited)) {
                        running = true;
                    }
                }
            }
        }

        for (int i = 0; i < leftMatching.length; ++i) {
            if (leftMatching[i] != -1) {
                albums.add(this.albums.get(i));
            }
        }

        return albums;
    }

    /**
     * Helper function for the Hopcroft-Karp algorithm
     */
    private boolean couple(int node, int[] leftMatching, int[] rightMatching, boolean[] visited) {
        if (visited[node]) {
            return false;
        }
        visited[node] = true;
        List<Integer> adj = nodes.get(node).getAdjacentNodes();
        for (Integer ad : adj) {
            if (rightMatching[ad] != -1) {
                continue;
            }
            rightMatching[ad] = node;
            leftMatching[node] = ad;
            return true;
        }
        for (Integer ad : adj) {
            if (couple(rightMatching[ad], leftMatching, rightMatching, visited)) {
                rightMatching[ad] = node;
                leftMatching[node] = ad;
                return true;
            }
        }
        return false;
    }

    /**
     * Helper function used for building the graph, returns the corresponding graph node for an artistId
     */
    private int getArtistNode(int artistId) {
        if (!artistMap.containsKey(artistId)) {
            artistMap.put(artistId, nodes.size());
            nodes.add(new Node());
        }
        return artistMap.get(artistId);
    }

    /**
     * Helper function used for building the graph, returns the corresponding graph node for a genreId
     */
    private int getGenreNode(int genreId) {
        if (!genreMap.containsKey(genreId)) {
            genreMap.put(genreId, secondStableSetCount++);
        }
        return genreMap.get(genreId);
    }
}
