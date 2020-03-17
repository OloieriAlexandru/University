package shell;

import app.Catalog;
import exceptions.ArgumentsMissingException;

public class CreateCatalogCommand extends Command {
    public CreateCatalogCommand(String command, String arguments) {
        super(command, arguments);
    }

    @Override
    public Catalog execute(Catalog catalog, Object... arguments) throws ArgumentsMissingException {
        if (arguments.length == 0) {
            throw new ArgumentsMissingException();
        } else {
            catalog = new Catalog((String) arguments[0]);
            System.out.println("Catalog created!");
        }
        return catalog;
    }
}
