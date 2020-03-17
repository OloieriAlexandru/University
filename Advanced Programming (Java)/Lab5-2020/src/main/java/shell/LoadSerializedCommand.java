package shell;

import app.Catalog;
import exceptions.ArgumentsMissingException;

public class LoadSerializedCommand extends Command {
    public LoadSerializedCommand(String command, String arguments) {
        super(command, arguments);
    }

    @Override
    public Catalog execute(Catalog catalog, Object... arguments) throws ArgumentsMissingException {
        if (arguments.length == 0) {
            throw new ArgumentsMissingException();
        } else {
            Catalog newCatalog = Catalog.deserialize((String) arguments[0]);
            if (newCatalog == null) {
                System.out.println("Deserialization failed!");
            } else {
                System.out.println("Deserialization complete!");
                catalog = newCatalog;
            }
        }
        return catalog;
    }
}
