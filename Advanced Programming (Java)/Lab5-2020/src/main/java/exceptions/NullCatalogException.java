package exceptions;

public class NullCatalogException extends Exception {
    public NullCatalogException() {
        super("This command can be executed only when there is an active catalog!");
    }
}
