package shell;

import app.Catalog;

public class ListCmdCommand extends Command {
    public ListCmdCommand(String command) {
        super(command);
    }

    @Override
    public Catalog execute(Catalog catalog, Object... arguments) {
        return catalog;
    }
}
