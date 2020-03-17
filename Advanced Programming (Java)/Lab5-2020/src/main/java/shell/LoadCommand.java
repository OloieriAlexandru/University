package shell;

import app.Catalog;
import exceptions.ArgumentsMissingException;
import exceptions.DuplicateDocumentException;
import exceptions.InvalidCatalogFileException;

public class LoadCommand extends Command {
    public LoadCommand(String command, String arguments) {
        super(command, arguments);
    }

    @Override
    public Catalog execute(Catalog catalog, Object... arguments) throws ArgumentsMissingException, InvalidCatalogFileException, DuplicateDocumentException {
        if (arguments.length == 0) {
            throw new ArgumentsMissingException();
        } else {
            catalog = Catalog.readFromFile((String) arguments[0]);
            System.out.println("Catalog loaded successfully!");
        }
        return catalog;
    }
}
