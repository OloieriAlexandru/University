package model;

/**
 * This class defines a table row
 * The third column can be of 3 types: String, int or other (if it's "other" then it cannot be modified")
 */
public class ComponentTableRow {
    private String name;
    private String type;
    private String strValue;
    private int intValue;
    private int cellType;

    /**
     * The constructor called for all the bean properties that are not of type "String" or "int"
     */
    public ComponentTableRow(String name, String type) {
        this.name = name;
        this.type = type;
        cellType = 0;
    }

    /**
     * The constructor called for all the bean properties that are of type "String"
     */
    public ComponentTableRow(String name, String type, String strValue) {
        this.name = name;
        this.type = type;
        this.strValue = strValue;
        cellType = 1;
    }

    /**
     * The constructor called for all the bean properties that are of type "int"
     */
    public ComponentTableRow(String name, String type, int intValue) {
        this.name = name;
        this.type = type;
        this.intValue = intValue;
        cellType = 2;
    }

    /**
     * A cell can be edited only if it is of type String or int
     */
    public boolean editable() {
        return type.equals("java.lang.String") || type.equals("int");
    }

    /**
     * Updates the value of a table cell
     */
    public void update(Object obj) {
        if (cellType == 1) {
            strValue = (String) obj;
        } else if (cellType == 2) {
            intValue = Integer.parseInt((String) obj);
        }
    }

    /**
     * Returns the value of the third column of a table row
     */
    public Object getValue() {
        if (!(type.equals("java.lang.String") || type.equals("int"))) {
            return "-";
        }
        if (type.equals("java.lang.String")) {
            return strValue;
        }
        return intValue;
    }

    public String getName() {
        return name;
    }

    public String getType() {
        return type;
    }
}
