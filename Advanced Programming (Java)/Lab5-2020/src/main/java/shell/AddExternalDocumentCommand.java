package shell;

import app.Catalog;
import app.ExternalDocument;
import exceptions.ArgumentsMissingException;
import exceptions.DuplicateDocumentException;
import exceptions.InvalidNumberOfArgumentsException;
import exceptions.NullCatalogException;

public class AddExternalDocumentCommand extends Command {
    public AddExternalDocumentCommand(String command, String arguments) {
        super(command, arguments);
    }

    @Override
    public Catalog execute(Catalog catalog, Object... arguments) throws NullCatalogException, ArgumentsMissingException, DuplicateDocumentException, InvalidNumberOfArgumentsException {
        if (arguments.length == 0) {
            throw new ArgumentsMissingException();
        } else if (catalog == null) {
            throw new NullCatalogException();
        } else {
            String[] args = ((String) arguments[0]).split(" ", 2);
            if (args.length == 2) {
                catalog.addDocument(new ExternalDocument(catalog.getDocumentId(), args[0], args[1]));
                System.out.println("Document added successfully!");
            } else {
                throw new InvalidNumberOfArgumentsException();
            }
        }
        return catalog;
    }
}
