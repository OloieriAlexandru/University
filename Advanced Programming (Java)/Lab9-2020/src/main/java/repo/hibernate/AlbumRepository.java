package repo.hibernate;

import entity.Album;
import entity.Artist;

import java.util.List;

public class AlbumRepository extends AbstractRepository<Album> {
    public AlbumRepository() {
        super(Album.class);
    }

    public List<Album> findByName(String name) {
        return entityManagerFactory.createEntityManager().createNamedQuery("Album.findByName")
                .setParameter("name", name)
                .getResultList();
    }

    public List<Album> findAll() {
        return entityManagerFactory.createEntityManager().createQuery("SELECT a FROM Album a").getResultList();
    }

    public List<Album> findByArtist(Artist artist) {
        return findByArtist(artist.getId());
    }

    public List<Album> findByArtist(int artistId) {
        return entityManagerFactory.createEntityManager().createNamedQuery("Album.findByArtist")
                .setParameter("artistId", artistId)
                .getResultList();
    }
}
