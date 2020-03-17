package shell;

import app.Catalog;
import app.Document;
import app.DocumentTag;
import exceptions.ArgumentsMissingException;
import exceptions.DuplicateTagException;
import exceptions.InvalidNumberOfArgumentsException;

public class AddDocumentTagCommand extends Command {
    public AddDocumentTagCommand(String command, String arguments) {
        super(command, arguments);
    }

    @Override
    public Catalog execute(Catalog catalog, Object... arguments) throws InvalidNumberOfArgumentsException, ArgumentsMissingException, DuplicateTagException {
        if (arguments.length == 1) {
            throw new ArgumentsMissingException();
        } else {
            String[] args = ((String) arguments[1]).split(" ", 2);
            if (args.length == 2) {
                Document document = (Document) arguments[0];
                DocumentTag tag = document.getTag(args[0]);
                if (tag != null) {
                    throw new DuplicateTagException();
                } else {
                    document.addTag(args[0], args[1]);
                    System.out.println("Tag added successfully!");
                }
            } else {
                throw new InvalidNumberOfArgumentsException();
            }
        }
        return null;
    }
}
