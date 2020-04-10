package exception;

public class ArtistNotFoundException extends Exception {
    public ArtistNotFoundException(String name) {
        super("There is no artist named " + name + " in the database!");
    }
}
