package app;

import java.awt.*;
import java.io.File;
import java.io.IOException;
import java.io.PrintWriter;
import java.io.Serializable;

public class FileSystemDocument extends Document implements Serializable {
    private String path;

    public FileSystemDocument(int id, String name, String path) {
        super(id, name);
        this.path = path;
    }

    @Override
    public void openDocument() {
        Desktop desktop = Desktop.getDesktop();
        try {
            desktop.open(new File(path));
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    @Override
    public void writeToFile(PrintWriter printWriter) {
        if (printWriter == null) {
            return;
        }
        printWriter.println("file-system");
        printWriter.println(path);
        writeCommonInfoToFile(printWriter);
    }

    @Override
    protected String getSpecificDocumentInfo(boolean full) {
        if (full) {
            return "File path: " + path;
        }
        return path;
    }

    @Override
    public String toString() {
        StringBuilder stringBuilder = new StringBuilder();

        stringBuilder.append("#").append(id).append(" ").append(name);
        stringBuilder.append(", path: ").append(path).append('\n');
        stringBuilder.append(getTagsAsString());

        return stringBuilder.toString();
    }
}
