import app.Catalog;
import common.Helper;
import exceptions.*;
import shell.*;

import java.util.Scanner;

public class Program {
    private static Catalog catalog = null;

    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        Shell shell = getShell();
        System.out.println("Commands:");
        System.out.println(shell);

        boolean running = true;
        while (running) {
            String line = Helper.readLine(scanner, "shell>");
            String[] cmdArgs = line.split(" ", 2);
            Command cmd = shell.getCommand(cmdArgs[0]);
            running = executeCommand(shell, cmd, cmdArgs);
        }
    }

    /**
     * Executes a shell command
     */
    private static boolean executeCommand(Shell shell, Command cmd, String[] cmdArgs) {
        if (cmd == null) {
            System.out.println("Invalid command! Type \"list-cmd\" to see details about the commands!");
        } else if (cmd.getCommand().equals("list-cmd")) {
            System.out.println(shell);
        } else if (cmd.getCommand().equals("quit")) {
            return false;
        } else {
            try {
                if (cmdArgs.length == 1) {
                    catalog = cmd.execute(catalog);
                } else {
                    catalog = cmd.execute(catalog, cmdArgs[1]);
                }
            } catch (NullCatalogException | ArgumentsMissingException | DuplicateDocumentException | InvalidNumberOfArgumentsException | InvalidDocumentNameException | DuplicateTagException | InvalidTagNameException | InvalidCatalogFileException | InvalidReportFormatException e) {
                System.out.println(e.getMessage());
            }
        }
        return true;
    }

    private static Shell getShell() {
        Shell shell = new Shell();

        shell.addCommand(new CatalogCommand("catalog"));
        shell.addCommand(new CreateCatalogCommand("create", "catalogName"));
        shell.addCommand(new AddFileSystemDocumentCommand("add-path", "documentName documentPath"));
        shell.addCommand(new AddExternalDocumentCommand("add-url", "documentName documentUrl"));
        shell.addCommand(new UpdateDocumentTagsCommand("update-tags", "documentName"));
        shell.addCommand(new ViewCommand("view", "documentName"));
        shell.addCommand(new ReportCommand("report", "{ html/ pdf/ word }"));
        shell.addCommand(new InfoCommand("info", "documentName"));
        shell.addCommand(new SaveCommand("save", "fileName"));
        shell.addCommand(new SaveSerializedCommand("save-serialized", "fileName"));
        shell.addCommand(new LoadCommand("load", "fileName"));
        shell.addCommand(new LoadSerializedCommand("load-serialized", "fileName"));
        shell.addCommand(new ListCmdCommand("list-cmd"));
        shell.addCommand(new QuitCommand("quit"));

        return shell;
    }
}
