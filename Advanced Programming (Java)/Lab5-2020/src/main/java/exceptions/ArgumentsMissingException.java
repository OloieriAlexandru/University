package exceptions;

public class ArgumentsMissingException extends Exception {
    public ArgumentsMissingException() {
        super("This command requires arguments! Type \"list-cmd\" to see details about the commands!");
    }
}
