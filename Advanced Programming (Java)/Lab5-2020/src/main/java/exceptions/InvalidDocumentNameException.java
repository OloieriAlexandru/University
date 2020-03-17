package exceptions;

public class InvalidDocumentNameException extends Exception {
    public InvalidDocumentNameException() {
        super("The active catalog doesn't have a document with the specified name!");
    }
}
