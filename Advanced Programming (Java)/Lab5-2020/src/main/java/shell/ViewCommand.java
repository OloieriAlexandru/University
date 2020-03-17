package shell;

import app.Catalog;
import exceptions.ArgumentsMissingException;
import exceptions.InvalidDocumentNameException;
import exceptions.NullCatalogException;

public class ViewCommand extends Command {
    public ViewCommand(String command, String arguments) {
        super(command, arguments);
    }

    @Override
    public Catalog execute(Catalog catalog, Object... arguments) throws ArgumentsMissingException, NullCatalogException, InvalidDocumentNameException {
        if (arguments.length == 0) {
            throw new ArgumentsMissingException();
        } else if (catalog == null) {
            throw new NullCatalogException();
        } else {
            catalog.openDocument(((String) arguments[0]).trim());
        }
        return catalog;
    }
}
