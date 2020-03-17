package shell;

import app.Catalog;
import exceptions.ArgumentsMissingException;
import exceptions.NullCatalogException;

public class SaveCommand extends Command {
    public SaveCommand(String command, String arguments) {
        super(command, arguments);
    }

    @Override
    public Catalog execute(Catalog catalog, Object... arguments) throws ArgumentsMissingException, NullCatalogException {
        if (arguments.length == 0) {
            throw new ArgumentsMissingException();
        } else if (catalog == null) {
            throw new NullCatalogException();
        } else {
            if (catalog.writeToFile((String) arguments[0])) {
                System.out.println("The catalog was successfully written to file!");
            }
        }
        return catalog;
    }
}
