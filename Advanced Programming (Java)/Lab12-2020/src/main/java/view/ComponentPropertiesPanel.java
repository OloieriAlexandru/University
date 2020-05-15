package view;

import model.ComponentTableModel;

import javax.swing.*;

/**
 * The table in which the bean properties will be shown
 */
public class ComponentPropertiesPanel extends JScrollPane {
    private JTable propertiesTable;

    public ComponentPropertiesPanel() {
        propertiesTable = new JTable();
        setViewportView(propertiesTable);

        setHorizontalScrollBarPolicy(JScrollPane.HORIZONTAL_SCROLLBAR_ALWAYS);
        setVerticalScrollBarPolicy(JScrollPane.VERTICAL_SCROLLBAR_ALWAYS);
    }

    public void updateTableProperties(ComponentTableModel newModel) {
        propertiesTable = new JTable(newModel);
        propertiesTable.setAutoResizeMode(JTable.AUTO_RESIZE_ALL_COLUMNS);
        setViewportView(propertiesTable);
    }
}
