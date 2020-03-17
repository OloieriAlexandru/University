package shell;

import app.Catalog;

public class CatalogCommand extends Command {
    public CatalogCommand(String command) {
        super(command);
    }

    @Override
    public Catalog execute(Catalog catalog, Object... arguments) {
        if (catalog == null) {
            System.out.println("There is no active catalog!");
        } else {
            System.out.println("Active catalog:\n" + catalog.toString());
        }
        return catalog;
    }
}
