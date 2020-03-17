package shell;

import app.Catalog;
import app.Document;
import app.DocumentTag;
import exceptions.ArgumentsMissingException;
import exceptions.InvalidTagNameException;

public class RemoveDocumentTagCommand extends Command {
    public RemoveDocumentTagCommand(String command, String arguments) {
        super(command, arguments);
    }

    @Override
    public Catalog execute(Catalog catalog, Object... arguments) throws ArgumentsMissingException, InvalidTagNameException {
        if (arguments.length == 1) {
            throw new ArgumentsMissingException();
        } else {
            Document document = (Document) arguments[0];
            DocumentTag tag = document.getTag((String) arguments[1]);
            if (tag == null) {
                throw new InvalidTagNameException();
            } else {
                document.removeTag(tag);
                System.out.println("Tag removed!");
            }
        }

        return null;
    }
}
