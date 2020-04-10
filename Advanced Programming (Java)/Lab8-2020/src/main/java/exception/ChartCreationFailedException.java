package exception;

public class ChartCreationFailedException extends Exception {
    public ChartCreationFailedException(String name) {
        super("Failed to insert chart " + name + " in the database!");
    }
}
