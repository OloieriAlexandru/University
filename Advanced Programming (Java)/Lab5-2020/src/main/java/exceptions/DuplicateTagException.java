package exceptions;

public class DuplicateTagException extends Exception {
    public DuplicateTagException() {
        super("A tag with the same name is already assigned to this document!");
    }
}
