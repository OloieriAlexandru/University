package app;

import java.awt.*;
import java.io.IOException;
import java.io.PrintWriter;
import java.io.Serializable;
import java.net.URI;
import java.net.URISyntaxException;

public class ExternalDocument extends Document implements Serializable {
    private String url;

    public ExternalDocument(int id, String name, String url) {
        super(id, name);
        this.url = url;
    }

    @Override
    public void openDocument() {
        Desktop desktop = Desktop.getDesktop();

        try {
            desktop.browse(new URI(url));
        } catch (IOException | URISyntaxException e) {
            e.printStackTrace();
        }
    }

    @Override
    public void writeToFile(PrintWriter printWriter) {
        if (printWriter == null) {
            return;
        }
        printWriter.println("external");
        printWriter.println(url);
        writeCommonInfoToFile(printWriter);
    }

    @Override
    protected String getSpecificDocumentInfo(boolean full) {
        if (full){
            return "Url: " + url;
        }
        return url;
    }

    @Override
    public String toString() {
        StringBuilder stringBuilder = new StringBuilder();

        stringBuilder.append("#").append(id).append(" ").append(name);
        stringBuilder.append(", url: ").append(url).append('\n');
        stringBuilder.append(getTagsAsString());

        return stringBuilder.toString();
    }
}
