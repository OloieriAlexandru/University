package app;

import com.lowagie.text.DocumentException;
import common.Helper;
import exceptions.DuplicateDocumentException;
import exceptions.InvalidCatalogFileException;
import exceptions.InvalidDocumentNameException;
import org.apache.poi.xwpf.usermodel.ParagraphAlignment;
import org.apache.poi.xwpf.usermodel.XWPFDocument;
import org.apache.poi.xwpf.usermodel.XWPFParagraph;
import org.apache.poi.xwpf.usermodel.XWPFRun;
import org.apache.velocity.Template;
import org.apache.velocity.VelocityContext;
import org.apache.velocity.app.VelocityEngine;
import org.apache.velocity.runtime.log.NullLogChute;
import org.xhtmlrenderer.pdf.ITextRenderer;

import java.io.*;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;

public class Catalog implements Serializable {
    private String name;
    private List<Document> documents = new ArrayList<>();
    private int documentId;

    public Catalog(String name) {
        this.name = name;
        documentId = 0;
    }

    public void addDocument(Document document) throws DuplicateDocumentException {
        if (documents.contains(document)) {
            throw new DuplicateDocumentException();
        }
        documents.add(document);
    }

    /**
     * Compulsory, second task, third bullet:
     * view: opens a document using the native operating system application (see the Desktop class);
     */
    public void openDocument(String documentName) throws InvalidDocumentNameException {
        for (int i = 0; i < documents.size(); ++i) {
            if (documents.get(i).getName().equals(documentName)) {
                documents.get(i).openDocument();
                return;
            }
        }
        throw new InvalidDocumentNameException();
    }

    /**
     * A function which opens the interactive menu for adding/ removing tags from a document
     */
    public void updateDocumentTags(String documentName) throws InvalidDocumentNameException {
        for (Document document : documents) {
            if (document.getName().equals(documentName)) {
                document.updateTags();
                return;
            }
        }
        throw new InvalidDocumentNameException();
    }

    public int getDocumentId() {
        return documentId++;
    }

    /**
     * Optional, first task:
     * Implement the save and load methods using a plain text representation of the catalog (instead of binary serialization).
     */
    public boolean writeToFile(String fileName) {
        try {
            FileWriter fileWriter = new FileWriter(fileName);
            PrintWriter printWriter = new PrintWriter(fileWriter);

            printWriter.println(name);
            printWriter.println(documents.size());
            for (Document document : documents) {
                document.writeToFile(printWriter);
            }

            printWriter.close();
        } catch (IOException e) {
            e.printStackTrace();
            return false;
        }
        return true;
    }

    /**
     * Optional, first task:
     * Implement the save and load methods using a plain text representation of the catalog (instead of binary serialization).
     */
    public static Catalog readFromFile(String fileName) throws InvalidCatalogFileException, DuplicateDocumentException {
        Catalog catalog = null;
        try {
            BufferedReader bufferedReader = new BufferedReader(new FileReader(fileName));
            String line = bufferedReader.readLine();
            Helper.checkNull(line);
            catalog = new Catalog(line);

            line = bufferedReader.readLine();
            Helper.checkNull(line);
            int documentsCount = Integer.parseInt(line);
            for (int i = 0; i < documentsCount; ++i) {
                Document document = Document.readFromFile(bufferedReader);
                catalog.addDocument(document);
            }
        } catch (IOException e) {
            System.out.println(e.getMessage());
            return null;
        }
        return catalog;
    }

    /**
     * Compulsory, second task, first bullet:
     * save: saves the catalog to an external file, using object serialization;
     */
    public boolean serialize(String fileName) {
        try {
            FileOutputStream file = new FileOutputStream(fileName);
            ObjectOutputStream out = new ObjectOutputStream(file);

            out.writeObject(this);

            out.close();
            file.close();
        } catch (IOException ex) {
            System.out.println("Error!" + ex.getMessage());
            return false;
        }
        return true;
    }

    /**
     * Compulsory, second task, first bullet:
     * save: saves the catalog to an external file, using object serialization;
     */
    public static Catalog deserialize(String fileName) {
        Catalog catalog = null;
        try {
            FileInputStream file = new FileInputStream(fileName);
            ObjectInputStream in = new ObjectInputStream(file);

            catalog = (Catalog) in.readObject();

            in.close();
            file.close();
        } catch (Exception e) {
            System.out.println("Error! " + e.getMessage());
        }
        return catalog;
    }

    /**
     * Bonus, second task, first bullet:
     * A template engine such as FreeMarker or Velocity, in order to create an HTML report.
     *
     * @return the name of the HTML report
     */
    public String reportHTML() {
        StringWriter writer = getHTMLReportWriter();

        return writeHTMLReportToFile(writer);
    }

    /**
     * Loads the HTML template and creates the HTML files
     */
    private StringWriter getHTMLReportWriter() {
        VelocityEngine ve = new VelocityEngine();
        ve.setProperty("runtime.log.logsystem.class", NullLogChute.class.getName());
        ve.init();
        Template t = ve.getTemplate("./templates/report.vm");

        VelocityContext context = getVelocityContext();

        StringWriter writer = new StringWriter();
        t.merge(context, writer);
        return writer;
    }

    /**
     * Bonus, second task, first bullet
     * Creates the HTML report file
     */
    private String writeHTMLReportToFile(StringWriter writer) {
        String fileName = name + ".html";
        try {
            FileWriter fileWriter = new FileWriter(fileName);
            PrintWriter printWriter = new PrintWriter(fileWriter);

            printWriter.println(writer.toString());

            printWriter.close();
        } catch (IOException e) {
            System.out.println(e.getMessage());
            return null;
        }
        return fileName;
    }

    /**
     * Creates a Velocity library related object
     */
    private VelocityContext getVelocityContext() {
        VelocityContext context = new VelocityContext();

        ArrayList<Map> documentsList = new ArrayList<>();

        for (Document document : documents) {
            Map map = document.getAsMap();
            documentsList.add(map);
        }

        context.put("catalog-name", name);
        context.put("documents", documentsList);

        return context;
    }

    /**
     * Bonus, second task, second bullet:
     * A report designer such as Dynamic Reports, in order to create PDF reports.
     *
     * @return the name of the PDF report
     */
    public String reportPDF() {
        StringWriter htmlWriter = getHTMLReportWriter();
        String fileName = name + ".pdf";
        try {
            OutputStream out = new FileOutputStream(fileName);
            ITextRenderer renderer = new ITextRenderer();

            renderer.setDocumentFromString(htmlWriter.toString());
            renderer.layout();
            renderer.createPDF(out);

            out.close();
        } catch (DocumentException | IOException e) {
            e.printStackTrace();
            return null;
        }
        return fileName;
    }

    /**
     * Bonus, second task, third bullet:
     * Apache POI in order to create Microsoft Documents (Word, Excel, etc).
     *
     * @return the name of the WORD report
     */
    public String reportWORD() {
        String fileName = name + ".doc";
        XWPFDocument document = new XWPFDocument();

        addWORDTitle(document);
        addWORDSubtitle(document);

        for (Document doc : documents) {
            doc.writeToWORD(document);
        }

        try {
            FileOutputStream out = new FileOutputStream(fileName);
            document.write(out);
            out.close();
            document.close();
        } catch (IOException e) {
            System.out.println(e.getMessage());
        }

        return fileName;
    }

    private void addWORDTitle(XWPFDocument document) {
        XWPFParagraph title = document.createParagraph();
        title.setAlignment(ParagraphAlignment.CENTER);

        XWPFRun titleRun = title.createRun();
        titleRun.setText("Catalog: " + name);
        titleRun.setColor("101010");
        titleRun.setBold(true);
        titleRun.setFontFamily("Arial");
        titleRun.setFontSize(22);
    }

    private void addWORDSubtitle(XWPFDocument document) {
        XWPFParagraph subTitle = document.createParagraph();

        XWPFRun subTitleRun = subTitle.createRun();
        subTitleRun.setText(documents.size() + " documents");
        subTitleRun.setColor("101010");
        subTitleRun.setFontFamily("Arial");
        subTitleRun.setBold(true);
        subTitleRun.setFontSize(18);
    }

    /**
     * Bonus, first task
     * info: display the metadata of a specific file: (you may want to use Apache Tika or something similar).
     */
    public void printDocumentInfo(String documentName) throws InvalidDocumentNameException {
        for (Document document : documents) {
            if (document.getName().equals(documentName)) {
                if (document instanceof ExternalDocument) {
                    System.out.println("Additional information can only be provided for file system documents!");
                } else {
                    document.printInfo();
                }
                return;
            }
        }
        throw new InvalidDocumentNameException();
    }

    @Override
    public String toString() {
        StringBuilder stringBuilder = new StringBuilder();

        stringBuilder.append("Catalog \"").append(name).append("\"");
        if (documents.size() > 0) {
            stringBuilder.append('\n');
            for (int i = 0; i < documents.size(); ++i) {
                stringBuilder.append(documents.get(i));
                if (i + 1 < documents.size()) {
                    stringBuilder.append('\n');
                }
            }
        } else {
            stringBuilder.append("\nNo documents");
        }

        return stringBuilder.toString();
    }
}
