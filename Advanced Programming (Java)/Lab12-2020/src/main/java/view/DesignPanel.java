package view;

import model.DynamicJComponent;

import javax.swing.*;
import java.awt.*;
import java.beans.XMLDecoder;
import java.beans.XMLEncoder;
import java.io.*;
import java.util.ArrayList;
import java.util.List;

/**
 * The design panel, where the JComponents will be drawn
 */
public class DesignPanel extends JPanel implements Serializable {
    private java.util.List<DynamicJComponent> componentsArr;

    /**
     * By setting layout = null, the components will have absolute positioning
     */
    public DesignPanel() {
        init();
        setLayout(null);
    }

    public void init() {
        componentsArr = new ArrayList<>();
    }

    public void addComponent(DynamicJComponent component) {
        componentsArr.add(component);
    }

    public void paintAllComponents() {
        this.paintAllComponents(componentsArr);
    }

    /**
     * Removes all the components and prints an empty panel
     */
    public void clearAndPaint() {
        this.componentsArr.clear();
        this.paintAllComponents(componentsArr);
    }

    private void paintAllComponents(java.util.List<DynamicJComponent> components) {
        this.componentsArr = components;
        repaint();
    }

    /**
     * The method that is called when a new component was added, or when a component's property was updated
     */
    @Override
    protected void paintComponent(Graphics g) {
        super.paintComponent(g);
        if (componentsArr == null) {
            return;
        }
        removeAll();
        revalidate();

        int width = 112;
        int height = 52;
        // https://stackoverflow.com/questions/11894088/java-set-the-exact-position-of-a-jcomponent
        // https://docs.oracle.com/javase/tutorial/uiswing/layout/none.html
        for (DynamicJComponent component : componentsArr) {
            JComponent paintedComponent = component.getComponent();
            int x = component.getX();
            int y = component.getY();
            paintedComponent.setBounds(x - width / 2, y - height / 2, width, height);
            add(paintedComponent);
        }

        repaint();
    }

    // https://howtodoinjava.com/java/serialization/xmlencoder-and-xmldecoder-example/

    /**
     * A static method which serializes a DesignPanel object to a XML file
     */
    public static boolean serializeToXML(String filePath, DesignPanel designPanel) {
        try {
            FileOutputStream fos = new FileOutputStream(filePath);
            XMLEncoder encoder = new XMLEncoder(fos);
            encoder.writeObject(designPanel);
            encoder.close();
        } catch (FileNotFoundException e) {
            e.printStackTrace();
            return false;
        }
        return true;
    }

    /**
     * A static file which tries to deserialize a XML file to a DesignPanel object
     * If it fails, it returns null
     */
    public static DesignPanel deserializeFromXML(String filePath) {
        XMLDecoder decoder = null;
        DesignPanel panel = null;
        try {
            decoder = new XMLDecoder(new BufferedInputStream(new FileInputStream(filePath)));
            panel = (DesignPanel) decoder.readObject();
            decoder.close();
        } catch (FileNotFoundException e) {
            e.printStackTrace();
            return null;
        }
        return panel;
    }

    public List<DynamicJComponent> getComponentsArr() {
        return componentsArr;
    }

    public void setComponentsArr(List<DynamicJComponent> componentsArr) {
        this.componentsArr = componentsArr;
    }
}
