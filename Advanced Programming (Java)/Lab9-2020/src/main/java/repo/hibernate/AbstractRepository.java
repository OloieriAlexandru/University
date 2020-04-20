package repo.hibernate;

import util.PersistenceUtil;

import javax.persistence.EntityManager;
import javax.persistence.EntityManagerFactory;

public class AbstractRepository<T> {
    private final Class<T> typeParameterClass;
    protected EntityManagerFactory entityManagerFactory;

    public AbstractRepository(Class<T> type) {
        entityManagerFactory = PersistenceUtil.getInstance().getEntityManagerFactory();
        typeParameterClass = type;
    }

    public T create(T entity) {
        EntityManager manager = entityManagerFactory.createEntityManager();
        manager.getTransaction().begin();

        manager.persist(entity);

        manager.getTransaction().commit();
        manager.close();

        return entity;
    }

    public T findById(int id) {
        return entityManagerFactory.createEntityManager().find(typeParameterClass, id);
    }
}
