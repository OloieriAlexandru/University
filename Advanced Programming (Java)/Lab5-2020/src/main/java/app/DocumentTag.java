package app;

import common.Helper;
import exceptions.InvalidCatalogFileException;
import org.apache.poi.xwpf.usermodel.ParagraphAlignment;
import org.apache.poi.xwpf.usermodel.XWPFDocument;
import org.apache.poi.xwpf.usermodel.XWPFParagraph;
import org.apache.poi.xwpf.usermodel.XWPFRun;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.PrintWriter;
import java.io.Serializable;
import java.util.HashMap;
import java.util.Map;
import java.util.Objects;

public class DocumentTag implements Serializable {
    private String name;
    private String value;

    public DocumentTag(String name, String value) {
        this.name = name;
        this.value = value;
    }

    public String getName() {
        return name;
    }

    public static DocumentTag readFromFile(BufferedReader bufferedReader) throws IOException, InvalidCatalogFileException {
        String nameString = bufferedReader.readLine();
        Helper.checkNull(nameString);
        String valueString = bufferedReader.readLine();
        Helper.checkNull(valueString);
        return new DocumentTag(nameString, valueString);
    }

    public void writeToFile(PrintWriter printWriter) {
        printWriter.println(name);
        printWriter.println(value);
    }

    public void writeToWORD(XWPFDocument document, int number) {
        XWPFParagraph paragraph = document.createParagraph();
        paragraph.setAlignment(ParagraphAlignment.LEFT);
        XWPFRun paragraphRun = paragraph.createRun();
        paragraphRun.setFontFamily("Arial");
        paragraphRun.setText(number + ". " + name + " : " + value);
    }

    public Map getAsMap() {
        Map map = new HashMap();

        map.put("tag-name", name);
        map.put("tag-value", value);

        return map;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        DocumentTag that = (DocumentTag) o;
        return Objects.equals(name, that.name) &&
                Objects.equals(value, that.value);
    }

    @Override
    public int hashCode() {
        return Objects.hash(name, value);
    }

    @Override
    public String toString() {
        return "\'" + name + "\' : \'" + value + '\'';
    }
}
