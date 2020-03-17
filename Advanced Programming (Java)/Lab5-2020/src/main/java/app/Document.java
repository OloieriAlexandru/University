package app;

import common.Helper;
import exceptions.*;
import org.apache.poi.xwpf.usermodel.ParagraphAlignment;
import org.apache.poi.xwpf.usermodel.XWPFDocument;
import org.apache.poi.xwpf.usermodel.XWPFParagraph;
import org.apache.poi.xwpf.usermodel.XWPFRun;
import org.apache.tika.exception.TikaException;
import org.apache.tika.metadata.Metadata;
import org.apache.tika.parser.AutoDetectParser;
import org.apache.tika.parser.ParseContext;
import org.apache.tika.parser.Parser;
import org.apache.tika.sax.BodyContentHandler;
import org.xml.sax.SAXException;
import shell.*;

import java.io.*;
import java.util.*;

public abstract class Document implements Serializable {
    private static Shell tagsShell = null;

    protected int id;
    protected String name;
    protected List<DocumentTag> tags = new ArrayList<>();

    public Document(int id, String name) {
        this.id = id;
        this.name = name;
    }

    public void addTag(String name, String value) {
        tags.add(new DocumentTag(name, value));
    }

    public void addTag(DocumentTag tag) {
        tags.add(tag);
    }

    public abstract void openDocument();

    public abstract void writeToFile(PrintWriter printWriter);

    public String getTagsAsString() {
        StringBuilder stringBuilder = new StringBuilder();

        if (tags.size() == 0) {
            stringBuilder.append("No tags");
        } else {
            stringBuilder.append("Tags: ");
            for (int i = 0; i < tags.size(); ++i) {
                stringBuilder.append(tags.get(i));
                if (i + 1 < tags.size()) {
                    stringBuilder.append("; ");
                }
            }
        }

        return stringBuilder.toString();
    }

    public void writeToWORD(XWPFDocument document) {
        writeInfoToWORD(document);
        writeTagsInfoToWORD(document);
    }

    private void writeInfoToWORD(XWPFDocument document) {
        XWPFParagraph paragraphInfo = document.createParagraph();
        paragraphInfo.setAlignment(ParagraphAlignment.BOTH);
        XWPFRun paragraphInfoRun = paragraphInfo.createRun();
        paragraphInfoRun.setBold(true);
        paragraphInfoRun.setFontSize(13);
        paragraphInfoRun.setFontFamily("Arial");
        paragraphInfoRun.setText("#" + id + " Document name: " + name + ", " + getSpecificDocumentInfo(true));
    }

    private void writeTagsInfoToWORD(XWPFDocument document) {
        XWPFParagraph paragraph = document.createParagraph();
        paragraph.setAlignment(ParagraphAlignment.BOTH);
        XWPFRun paragraphRun = paragraph.createRun();
        if (tags.size() > 0) {
            paragraphRun.setText("\tTags:");
            for (int i = 0; i < tags.size(); ++i) {
                tags.get(i).writeToWORD(document, i + 1);
            }
        } else {
            paragraphRun.setText("\tTags:\nnone");
        }
    }

    public void updateTags() {
        System.out.println("File information:\n" + toString());
        Scanner scanner = new Scanner(System.in);
        Shell shell = getTagsShell();
        System.out.println("Commands:");
        System.out.println(shell);

        boolean running = true;
        while (running) {
            String line = Helper.readLine(scanner, "tags-menu>");
            String[] cmdArgs = line.split(" ", 2);
            Command cmd = shell.getCommand(cmdArgs[0]);
            running = executeCommand(cmd, cmdArgs);
        }
    }

    public DocumentTag getTag(String tagName) {
        return tags.stream().filter(tag -> tag.getName().equals(tagName)).findAny().orElse(null);
    }

    public void removeTag(DocumentTag tag) {
        tags.removeIf(t -> t.equals(tag));
    }

    public String getName() {
        return name;
    }

    public static Document readFromFile(BufferedReader bufferedReader) throws IOException, InvalidCatalogFileException {
        Document document = readDocumentFromFile(bufferedReader);
        String line = bufferedReader.readLine();
        Helper.checkNull(line);
        int tagsNumber = Integer.parseInt(line);
        for (int i = 0; i < tagsNumber; ++i) {
            DocumentTag tag = DocumentTag.readFromFile(bufferedReader);
            document.addTag(tag);
        }
        return document;
    }

    public Map getAsMap() {
        Map result = new HashMap();

        ArrayList<Map> tagsList = new ArrayList<>();

        for (DocumentTag tag : tags) {
            Map map = tag.getAsMap();
            tagsList.add(map);
        }

        result.put("document-id", id);
        result.put("document-name", name);
        result.put("document-info", getSpecificDocumentInfo(true));
        result.put("document-tags", tagsList);

        return result;
    }

    protected abstract String getSpecificDocumentInfo(boolean full);

    public void printInfo() {
        String path = getSpecificDocumentInfo(false);

        try (InputStream stream = new FileInputStream(path)) {
            Parser parser = new AutoDetectParser();
            BodyContentHandler handler = new BodyContentHandler();
            Metadata metadata = new Metadata();
            ParseContext context = new ParseContext();
            parser.parse(stream, handler, metadata, context);

            String[] names = metadata.names();
            System.out.println("File metadata:");
            for (String name : names) {
                System.out.println(name + " : " + metadata.get(name));
            }
        } catch (IOException | TikaException | SAXException e) {
            System.out.println(e.getMessage());
        }
    }

    private static Document readDocumentFromFile(BufferedReader bufferedReader) throws IOException, InvalidCatalogFileException {
        String typeString = bufferedReader.readLine();
        Helper.checkNull(typeString);
        String line = bufferedReader.readLine();
        Helper.checkNull(line);
        String idString = bufferedReader.readLine();
        Helper.checkNull(idString);
        String nameString = bufferedReader.readLine();
        Helper.checkNull(nameString);
        Document document;
        if (typeString.equals("external")) {
            document = new ExternalDocument(Integer.parseInt(idString), nameString, line);
        } else if (typeString.equals("file-system")) {
            document = new FileSystemDocument(Integer.parseInt(idString), nameString, line);
        } else {
            throw new InvalidCatalogFileException();
        }
        return document;
    }

    protected void writeCommonInfoToFile(PrintWriter printWriter) {
        printWriter.println(id);
        printWriter.println(name);
        printWriter.println(tags.size());
        for (DocumentTag tag : tags) {
            tag.writeToFile(printWriter);
        }
    }

    private boolean executeCommand(Command cmd, String[] cmdArgs) {
        if (cmd == null) {
            System.out.println("Invalid command! Type \"list-cmd\" to see details about the commands!");
        } else if (cmd.getCommand().equals("list-cmd")) {
            System.out.println(tagsShell);
        } else if (cmd.getCommand().equals("stop")) {
            return false;
        } else {
            try {
                if (cmdArgs.length == 1) {
                    cmd.execute(null, this);
                } else {
                    cmd.execute(null, this, cmdArgs[1]);
                }
            } catch (InvalidDocumentNameException | NullCatalogException | ArgumentsMissingException | DuplicateDocumentException | InvalidNumberOfArgumentsException | DuplicateTagException | InvalidTagNameException | InvalidCatalogFileException | InvalidReportFormatException e) {
                System.out.println(e.getMessage());
            }
        }
        return true;
    }

    private static Shell getTagsShell() {
        if (tagsShell != null) {
            return tagsShell;
        }
        tagsShell = new Shell();

        tagsShell.addCommand(new AddDocumentTagCommand("add", "tagName tagValue"));
        tagsShell.addCommand(new RemoveDocumentTagCommand("remove", "tagName"));
        tagsShell.addCommand(new ViewTagsCommand("view-tags"));
        tagsShell.addCommand(new ListCmdCommand("list-cmd"));
        tagsShell.addCommand(new QuitCommand("stop"));

        return tagsShell;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        Document document = (Document) o;
        return name.equals(document.name);
    }

    @Override
    public int hashCode() {
        return Objects.hash(name);
    }
}
