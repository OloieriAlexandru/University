package shell;

import app.Catalog;

public class QuitCommand extends Command {
    public QuitCommand(String command) {
        super(command);
    }

    @Override
    public Catalog execute(Catalog catalog, Object... arguments) {
        return catalog;
    }
}
