package util;

import javax.persistence.EntityManagerFactory;
import javax.persistence.Persistence;

/**
 * A singleton class which exposes a method which returns an instance of "EntityManagerFactory" class
 */
public class PersistenceUtil implements AutoCloseable {
    private static EntityManagerFactory entityManagerFactory = null;
    private static PersistenceUtil instance = null;

    static {
        instance = new PersistenceUtil();
        entityManagerFactory = Persistence.createEntityManagerFactory("persistence-unit");
    }

    public synchronized static PersistenceUtil getInstance() {
        return instance;
    }

    public EntityManagerFactory getEntityManagerFactory() {
        return entityManagerFactory;
    }

    @Override
    public void close() throws Exception {
        if (entityManagerFactory != null) {
            entityManagerFactory.close();
        }
    }
}
