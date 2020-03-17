package shell;

import app.Catalog;
import exceptions.*;

public abstract class Command {
    private String command;
    private String arguments;

    public Command(String command) {
        this.command = command;
    }

    public Command(String command, String arguments) {
        this.command = command;
        this.arguments = arguments;
    }

    public String getCommand() {
        return command;
    }

    public abstract Catalog execute(Catalog catalog, Object... arguments) throws NullCatalogException, ArgumentsMissingException, DuplicateDocumentException, InvalidNumberOfArgumentsException, InvalidDocumentNameException, DuplicateTagException, InvalidTagNameException, InvalidCatalogFileException, InvalidReportFormatException;

    @Override
    public String toString() {
        if (arguments == null) {
            return command;
        }
        StringBuilder spaces = new StringBuilder();
        int spacesCount = 18 - command.length();
        for (int i = 0; i < spacesCount; ++i) {
            spaces.append(' ');
        }
        return command + spaces.toString() + arguments;
    }
}
