package shell;

import app.Catalog;
import exceptions.ArgumentsMissingException;
import exceptions.NullCatalogException;

public class SaveSerializedCommand extends Command {
    public SaveSerializedCommand(String command, String arguments) {
        super(command, arguments);
    }

    @Override
    public Catalog execute(Catalog catalog, Object... arguments) throws ArgumentsMissingException, NullCatalogException {
        if (arguments.length == 0) {
            throw new ArgumentsMissingException();
        } else if (catalog == null) {
            throw new NullCatalogException();
        } else {
            if (catalog.serialize((String) arguments[0])) {
                System.out.println("Serialization complete!");
            }
        }
        return catalog;
    }
}
