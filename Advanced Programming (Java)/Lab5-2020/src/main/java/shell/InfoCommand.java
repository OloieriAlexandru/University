package shell;

import app.Catalog;
import exceptions.ArgumentsMissingException;
import exceptions.InvalidDocumentNameException;
import exceptions.NullCatalogException;

public class InfoCommand extends Command {
    public InfoCommand(String command, String arguments) {
        super(command, arguments);
    }

    @Override
    public Catalog execute(Catalog catalog, Object... arguments) throws NullCatalogException, ArgumentsMissingException, InvalidDocumentNameException {
        if (arguments.length == 0) {
            throw new ArgumentsMissingException();
        } else if (catalog == null) {
            throw new NullCatalogException();
        } else {
            String documentName = (String) arguments[0];
            catalog.printDocumentInfo(documentName);
        }
        return catalog;
    }
}
