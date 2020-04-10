package model;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

public class Chart {
    private int id;
    private String name;
    private List<Album> albums;

    public Chart(int id, String name) {
        this.id = id;
        this.name = name;
        this.albums = new ArrayList<>();
    }

    public Chart(int id, String name, List<Album> albums) {
        this.id = id;
        this.name = name;
        this.albums = albums;
    }

    public void addAlbum(Album... albums) {
        this.albums.addAll(Arrays.asList(albums));
    }

    public String getName() {
        return name;
    }

    public int getId() {
        return id;
    }

    public List<Album> getAlbums() {
        return albums;
    }
}
