package exceptions;

public class InvalidTagNameException extends Exception {
    public InvalidTagNameException() {
        super("This document doesn't have a tag with the specified name!");
    }
}
