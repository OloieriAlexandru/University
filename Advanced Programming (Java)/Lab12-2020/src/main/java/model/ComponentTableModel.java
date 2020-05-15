package model;

import controller.Controller;

import javax.swing.table.AbstractTableModel;
import java.util.List;

// https://www.codejava.net/java-se/swing/editable-jtable-example

/**
 * This class takes care of the table logic
 */
public class ComponentTableModel extends AbstractTableModel {
    private List<ComponentTableRow> rows;
    private DynamicJComponent dynamicJComponent;
    private Controller controller;

    private final String[] columnNames = new String[]{
            "Prop. name", "Prop. type", "Prop. value"
    };

    public ComponentTableModel(List<ComponentTableRow> rows, DynamicJComponent dynamicJComponent, Controller controller) {
        this.rows = rows;
        this.dynamicJComponent = dynamicJComponent;
        this.controller = controller;
    }

    @Override
    public String getColumnName(int column) {
        return columnNames[column];
    }

    @Override
    public int getRowCount() {
        return rows.size();
    }

    @Override
    public int getColumnCount() {
        return columnNames.length;
    }

    /**
     * This function is called when displaying table information
     */
    @Override
    public Object getValueAt(int rowIndex, int columnIndex) {
        if (rowIndex >= rows.size() || columnIndex >= columnNames.length) {
            return null;
        }
        if (columnIndex == 0) {
            return rows.get(rowIndex).getName();
        } else if (columnIndex == 1) {
            return rows.get(rowIndex).getType();
        }
        return rows.get(rowIndex).getValue();
    }

    /**
     * This function is called when the user tries to modify some table columns
     */
    @Override
    public void setValueAt(Object aValue, int rowIndex, int columnIndex) {
        if (columnIndex < 2) {
            return;
        }
        rows.get(rowIndex).update(aValue);
        dynamicJComponent.updateStringOrIntField(rows.get(rowIndex).getName(), aValue);
        controller.repaint();
    }

    /**
     * This function is called when deciding whether a cell can be modified
     * A cell can be modified only if is of type String or int
     */
    @Override
    public boolean isCellEditable(int rowIndex, int columnIndex) {
        if (columnIndex < 2) {
            return false;
        }
        return rows.get(rowIndex).editable();
    }
}
