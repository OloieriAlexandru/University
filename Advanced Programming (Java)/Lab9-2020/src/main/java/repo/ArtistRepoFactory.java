package repo;

import repo.hibernate.ArtistRepository;
import repo.jdbc.connection.Database;
import repo.jdbc.dao.ArtistController;

public class ArtistRepoFactory implements AbstractFactory<ArtistRepo> {
    @Override
    public ArtistRepo create(String type) {
        if (type.equals("jdbc")) {
            return new ArtistController(Database.getInstance());
        } else if (type.equals("jpa")) {
            return new ArtistRepository();
        }
        return null;
    }
}
