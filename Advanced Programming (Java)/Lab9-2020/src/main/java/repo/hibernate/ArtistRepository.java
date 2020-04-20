package repo.hibernate;

import entity.Artist;
import repo.ArtistRepo;

import java.util.List;

public class ArtistRepository extends AbstractRepository<Artist> implements ArtistRepo {
    public ArtistRepository() {
        super(Artist.class);
        System.out.println("JPA Repository Created!");
    }

    public List<Artist> findByName(String name) {
        return entityManagerFactory.createEntityManager().createNamedQuery("Artist.findByName")
                .setParameter("name", name)
                .getResultList();
    }
}
