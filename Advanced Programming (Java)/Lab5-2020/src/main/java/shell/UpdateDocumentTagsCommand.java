package shell;

import app.Catalog;
import exceptions.*;

public class UpdateDocumentTagsCommand extends Command {
    public UpdateDocumentTagsCommand(String command, String arguments) {
        super(command, arguments);
    }

    @Override
    public Catalog execute(Catalog catalog, Object... arguments) throws ArgumentsMissingException, NullCatalogException, InvalidDocumentNameException {
        if (arguments.length == 0) {
            throw new ArgumentsMissingException();
        } else if (catalog == null) {
            throw new NullCatalogException();
        } else {
            catalog.updateDocumentTags((String) arguments[0]);
        }

        return catalog;
    }
}
