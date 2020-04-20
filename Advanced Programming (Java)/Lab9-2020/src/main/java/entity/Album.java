package entity;

import javax.persistence.*;
import java.io.Serializable;

@Entity
@Table(name = "albums")
@NamedQueries({
        @NamedQuery(name = "Album.findByName", query = "SELECT a FROM Album a WHERE a.name=:name"),
        @NamedQuery(name = "Album.findByArtist", query = "SELECT a FROM Album a WHERE a.artistId=:artistId")
})
public class Album implements Serializable {
    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    @Column(name = "id")
    private int id;

    @Column(name = "name", nullable = false)
    private String name;

    @Column(name = "artist_id", nullable = false)
    private int artistId;

    @Column(name = "release_year")
    private int releaseYear;

    @Column(name = "genre_id")
    private int genreId;

    public Album() {
    }

    public Album(String name, int artistId, int releaseYear, GenreEnum genre) {
        this.name = name;
        this.artistId = artistId;
        this.releaseYear = releaseYear;
        this.genreId = genre.getValue();
    }

    public Album(String name, int artistId, int releaseYear, int genreId) {
        this.name = name;
        this.artistId = artistId;
        this.releaseYear = releaseYear;
        this.genreId = genreId;
    }

    public GenreEnum getGenre() {
        return GenreEnum.values()[genreId - 1];
    }

    public void setGenre(GenreEnum genre) {
        this.genreId = genre.getValue();
    }

    public int getGenreId() {
        return genreId;
    }

    public void setGenreId(int genreId) {
        this.genreId = genreId;
    }

    public int getArtistId() {
        return artistId;
    }

    public void setArtistId(int artistId) {
        this.artistId = artistId;
    }

    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public int getReleaseYear() {
        return releaseYear;
    }

    public void setReleaseYear(int releaseYear) {
        this.releaseYear = releaseYear;
    }

    @Override
    public String toString() {
        return "Album{" +
                "id=" + id +
                ", name='" + name + '\'' +
                ", artistId=" + artistId +
                ", releaseYear=" + releaseYear +
                ", genre=" + GenreEnum.values()[genreId - 1] +
                '}';
    }
}
