package shell;

import app.Catalog;
import app.Document;

public class ViewTagsCommand extends Command {
    public ViewTagsCommand(String command) {
        super(command);
    }

    @Override
    public Catalog execute(Catalog catalog, Object... arguments) {
        Document document = (Document) arguments[0];
        System.out.println(document.getTagsAsString());

        return null;
    }
}
