package controller;

import helper.Helper;
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
import java.io.File;
import java.io.Serializable;
import java.lang.annotation.Annotation;
import java.lang.reflect.InvocationTargetException;
import java.net.MalformedURLException;
import java.net.URL;
import java.nio.file.Paths;
import java.util.*;

/**
 * This class coordinates all the application logic
 */
public class Controller implements Serializable {
    private MainFrame mainFrame;
    private DesignPanel designPanel;
    private ControlPanel controlPanel;
    private ComponentPropertiesPanel componentPropertiesPanel;
    private DynamicClassloader classloader = new DynamicClassloader();

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
                DynamicJComponent newJComponent = new DynamicJComponent(newComponent, x, y, false);
                componentDynamicJComponentMap.put(newComponent, newJComponent);
                designPanel.addComponent(newJComponent);
                designPanel.paintAllComponents();
            } catch (ClassNotFoundException | InstantiationException | IllegalAccessException | NoSuchMethodException | InvocationTargetException evt) {
                if (!tryToLoadDynamicClass(x, y, componentName, componentText)) {
                    System.err.println(evt);
                }
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
        Scanner scanner = new Scanner(System.in);
        mainFrame.open();
        while (true) {
            String line = Helper.readLine(scanner, "jar path: ");
            File path = new File(line);
            if (path.exists()) {
                URL url = null;
                try {
                    url = path.toURI().toURL();
                    classloader.addURL(url);
                } catch (MalformedURLException e) {
                    e.printStackTrace();
                }
            }
        }
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
                designPanel.addMouseListener(designPanelMouseAdapter);
                componentDynamicJComponentMap.clear();
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
        componentDynamicJComponentMap.clear();
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

    /**
     * Tries to load a class from an external JAR using the dynamic class loader
     * Also, checks whether the class has annotations and if it does, applies those properties
     */
    private boolean tryToLoadDynamicClass(int x, int y, String componentName, String componentText) {
        Class clazz = null;
        try {
            clazz = classloader.loadClass(componentName);

            int width = 112, height = 112;
            String widthAnnotationStart = "@annotations.Width\\(";
            String heightAnnotationStart = "@annotations.Height\\(";
            String textAnnotationStart = "@annotations.Text\\(\"";

            for (Annotation annotation : clazz.getAnnotations()) {
                String annotationText = annotation.toString();
                if (annotationText.startsWith("@annotations.Width(")) {
                    width = Integer.parseInt(annotationText.split(widthAnnotationStart)[1].split("\\)")[0]);
                } else if (annotationText.startsWith("@annotations.Height(")) {
                    height = Integer.parseInt(annotationText.split(heightAnnotationStart)[1].split("\\)")[0]);
                } else if (annotationText.startsWith("@annotations.Text(\"")) {
                    if (componentText.length() == 0) {
                        componentText = annotationText.split(textAnnotationStart)[1].split("\"\\)")[0];
                    }
                }
            }
            JComponent newComponent = (JComponent) clazz.getConstructor(String.class).newInstance(componentText);
            newComponent.setBounds(x, y, width, height);
            newComponent.addMouseListener(dynamicComponentMouseAdapter);

            DynamicJComponent newJComponent = new DynamicJComponent(newComponent, x, y, true);
            componentDynamicJComponentMap.put(newComponent, newJComponent);
            designPanel.addComponent(newJComponent);
            designPanel.paintAllComponents();
        } catch (ClassNotFoundException | NoSuchMethodException | IllegalAccessException | InstantiationException | InvocationTargetException e) {
            e.printStackTrace();
            return false;
        }
        return true;
    }
}
