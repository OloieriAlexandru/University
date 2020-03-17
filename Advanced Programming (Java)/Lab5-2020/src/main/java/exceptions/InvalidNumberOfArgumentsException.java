package exceptions;

public class InvalidNumberOfArgumentsException extends Exception {
    public InvalidNumberOfArgumentsException() {
        super("You called the command with invalid number of arguments! Type \"list-cmd\" to see details about the commands!");
    }
}
