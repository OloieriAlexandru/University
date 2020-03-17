package shell;

import app.Catalog;
import exceptions.ArgumentsMissingException;
import exceptions.InvalidReportFormatException;
import exceptions.NullCatalogException;

public class ReportCommand extends Command {
    public ReportCommand(String command, String arguments) {
        super(command, arguments);
    }

    @Override
    public Catalog execute(Catalog catalog, Object... arguments) throws ArgumentsMissingException, InvalidReportFormatException, NullCatalogException {
        if (arguments.length == 0) {
            throw new ArgumentsMissingException();
        } else if (catalog == null) {
            throw new NullCatalogException();
        } else {
            String type = (String) (arguments[0]);
            String result;
            if (type.equals("html")) {
                result = catalog.reportHTML();
            } else if (type.equals("pdf")) {
                result = catalog.reportPDF();
            } else if (type.equals("word")) {
                result = catalog.reportWORD();
            } else {
                throw new InvalidReportFormatException();
            }
            if (result != null) {
                System.out.println("Report file (\"" + result + "\") created!");
            }
        }
        return catalog;
    }
}
