package controller;

import model.ComponentTableModel;
import model.ComponentTableRow;
import model.DynamicJComponent;
import view.*;

import javax.swing.*;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.beans.BeanInfo;
import java.beans.IntrospectionException;
import java.beans.Introspector;
import java.beans.PropertyDescriptor;
import java.io.Serializable;
import java.lang.reflect.InvocationTargetException;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

/**
 * This class coordinates all the application logic
 */
public class Controller implements Serializable {
    private MainFrame mainFrame;
    private DesignPanel designPanel;
    private ControlPanel controlPanel;
    private ComponentPropertiesPanel componentPropertiesPanel;

    private Map<JComponent, DynamicJComponent> componentDynamicJComponentMap = new HashMap<>();

    /**
     * The method that is called when a component on the DesignPanel is clicked
     */
    private MouseAdapter dynamicComponentMouseAdapter = new MouseAdapter() {
        @Override
        public void mousePressed(MouseEvent e) {
            JComponent element = (JComponent) e.getSource();
            dynamicComponentClicked(element);
        }
    };

    /**
     * The method that is called when the DesignPanel is clicked
     */
    private MouseAdapter designPanelMouseAdapter = new MouseAdapter() {
        @Override
        public void mousePressed(MouseEvent e) {
            int x = e.getX();
            int y = e.getY();
            String componentName = controlPanel.getComponentName();
            String componentText = controlPanel.getDefaultComponentText();

            try {
                Class clazz = Class.forName(componentName);
                JComponent newComponent = (JComponent) clazz.getConstructor(String.class).newInstance(componentText);
                newComponent.addMouseListener(dynamicComponentMouseAdapter);
                DynamicJComponent newJComponent = new DynamicJComponent(newComponent, x, y);
                componentDynamicJComponentMap.put(newComponent, newJComponent);
                designPanel.addComponent(newJComponent);
                designPanel.paintAllComponents();
            } catch (ClassNotFoundException | InstantiationException | IllegalAccessException | NoSuchMethodException | InvocationTargetException evt) {
                System.err.println(evt);
            }
        }
    };

    public Controller() {
        controlPanel = new ControlPanel();
        componentPropertiesPanel = new ComponentPropertiesPanel();
        designPanel = new DesignPanel();
        designPanel.addMouseListener(designPanelMouseAdapter);

        ButtonsPanel buttonsPanel = new ButtonsPanel(this);

        mainFrame = new MainFrame(designPanel, buttonsPanel, controlPanel, componentPropertiesPanel);
    }

    /**
     * The function which opens the UI
     */
    public void openApp() {
        mainFrame.open();
    }

    /**
     * A method that can be called to trigger the redraw of the DesignPanel
     */
    public void repaint() {
        designPanel.paintAllComponents();
    }

    /**
     * Loads the DesignPanel from an external file (a serialized file)
     */
    public void load() {
        JFileChooser j = new JFileChooser(Paths.get("").toAbsolutePath().toString());
        int r = j.showOpenDialog(null);
        if (r == JFileChooser.APPROVE_OPTION) {
            DesignPanel desPanel = DesignPanel.deserializeFromXML(j.getSelectedFile().getAbsolutePath());
            if (desPanel != null) {
                designPanel = desPanel;
                mainFrame.setDesignPanel(designPanel);
                designPanel.paintAllComponents();
            } else {
                System.out.println("ERROR");
            }
        }
    }

    /**
     * Saves the DesignPanel to an external file
     */
    public void save() {
        JFileChooser j = new JFileChooser(Paths.get("").toAbsolutePath().toString());
        int r = j.showSaveDialog(null);
        if (r == JFileChooser.APPROVE_OPTION) {
            designPanel.removeMouseListener(designPanelMouseAdapter);
            if (DesignPanel.serializeToXML(j.getSelectedFile().getAbsolutePath(), designPanel)) {
                System.out.println("SUCCESS");
            } else {
                System.out.println("ERROR");
            }
            designPanel.addMouseListener(designPanelMouseAdapter);
        }
    }

    /**
     * Deletes all components from the DesignPanel
     */
    public void reset() {
        designPanel.clearAndPaint();
    }

    /**
     * Extracts all the bean properties using Introspector class and displays them in the table
     */
    private void dynamicComponentClicked(JComponent component) {
        try {
            BeanInfo info = Introspector.getBeanInfo(component.getClass());
            List<ComponentTableRow> rows = new ArrayList<>();
            for (PropertyDescriptor pd : info.getPropertyDescriptors()) {
                String name = pd.getName();
                String type;
                if (pd.getPropertyType() == null) {
                    continue;
                } else {
                    type = pd.getPropertyType().getTypeName();
                }
                if (type.equals("int")) {
                    rows.add(new ComponentTableRow(name, type, (Integer) pd.getReadMethod().invoke(component)));
                } else if (type.equals("java.lang.String")) {
                    rows.add(new ComponentTableRow(name, type, (String) pd.getReadMethod().invoke(component)));
                } else {
                    rows.add(new ComponentTableRow(name, type));
                }
            }
            componentPropertiesPanel.updateTableProperties(new ComponentTableModel(rows, componentDynamicJComponentMap.get(component), this));
        } catch (IntrospectionException | IllegalAccessException | InvocationTargetException e) {
            e.printStackTrace();
        }
    }
}
