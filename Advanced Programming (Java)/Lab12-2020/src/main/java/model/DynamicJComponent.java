package model;

import javax.swing.*;
import java.beans.BeanInfo;
import java.beans.IntrospectionException;
import java.beans.Introspector;
import java.beans.PropertyDescriptor;
import java.io.Serializable;
import java.lang.reflect.InvocationTargetException;

/**
 * This class contains details about a Java Swing component that is supposed to be drawn on the DesignPanel
 * It has an empty constructor and getters and setter for XML serialization
 */
public class DynamicJComponent implements Serializable {
    private JComponent component;
    private int x;
    private int y;
    private boolean custom;

    public DynamicJComponent() {
    }

    public DynamicJComponent(JComponent component, int x, int y, boolean custom) {
        this.component = component;
        this.x = x;
        this.y = y;
        this.custom = custom;
    }

    public int getY() {
        return y;
    }

    public int getX() {
        return x;
    }

    public JComponent getComponent() {
        return component;
    }

    public void setY(int y) {
        this.y = y;
    }

    public void setX(int x) {
        this.x = x;
    }

    public void setComponent(JComponent component) {
        this.component = component;
    }

    /**
     * Updates one of the JComponent's fields (the one with name equal to "fieldName")
     */
    public void updateStringOrIntField(String fieldName, Object value) {
        try {
            BeanInfo info = Introspector.getBeanInfo(component.getClass());
            for (PropertyDescriptor pd : info.getPropertyDescriptors()) {
                if (pd.getName().equals(fieldName)) {
                    if (pd.getWriteMethod() != null) {
                        pd.getWriteMethod().invoke(component, value);

                        if (fieldName.equals("x")) {
                            x = Integer.parseInt((String) value);
                        } else if (fieldName.equals("y")) {
                            y = Integer.parseInt((String) value);
                        }

                        break;
                    }
                }
            }
        } catch (IntrospectionException | IllegalAccessException | InvocationTargetException e) {
            e.printStackTrace();
        }

        if (fieldName.equals("x")){
            setX(Integer.parseInt((String) value));
        } else if (fieldName.equals("y")){
            setY(Integer.parseInt((String) value));
        }
    }

    public boolean isCustom() {
        return custom;
    }
}
