package exception;

public class ArtistCreationFailedException extends Exception {
    public ArtistCreationFailedException(String name, String country) {
        super("Failed to insert artist " + name + " (country: " + country + ") in the database!");
    }
}
