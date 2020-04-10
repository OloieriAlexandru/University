package exception;

public class AlbumCreationFailedException extends Exception {
    public AlbumCreationFailedException(String name, int releaseYear) {
        super("Failed to insert album " + name + " (released in " + releaseYear + ") in the database!");
    }
}
