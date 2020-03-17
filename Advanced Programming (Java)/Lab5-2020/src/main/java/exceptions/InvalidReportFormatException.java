package exceptions;

public class InvalidReportFormatException extends Exception {
    public InvalidReportFormatException() {
        super("The specified format is not supported! The supported formats are: { html/ pdf/ word }");
    }
}
