package exceptions;

public class DuplicateDocumentException extends Exception {
    public DuplicateDocumentException() {
        super("A document with the same name already exists in this catalog!");
    }
}
