package repo;

public interface AbstractFactory<T> {
    T create(String type);
}
