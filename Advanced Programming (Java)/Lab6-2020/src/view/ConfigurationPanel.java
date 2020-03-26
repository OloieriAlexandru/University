package view;

import model.ShapeType;

import javax.swing.*;
import java.awt.*;

/**
 * Contains all the inputs which specify the sizes and colors of the nodes/edges
 */
public class ConfigurationPanel extends JPanel {
    private JComboBox<String> shapeTypeInput = null;
    private JSpinner shapeSizeInput = null;
    private JComboBox<String> shapeColorInput = null;
    private JComboBox<String> edgeColorInput = null;

    public ConfigurationPanel() {
        init();
    }

    /**
     * Creates the Configuration Panel
     */
    private void init() {
        setLayout(new GridBagLayout());

        GridBagConstraints bagConstraints = generateGridBagConstraints();
        add(generateShapeTypeLabel(), bagConstraints);

        bagConstraints.gridx = 1;
        shapeTypeInput = generateShapeTypeInput();
        add(shapeTypeInput, bagConstraints);

        bagConstraints.gridx = 2;
        add(generateShapeSizeLabel(), bagConstraints);

        bagConstraints.gridx = 3;
        shapeSizeInput = generateShapeSizeInput();
        add(shapeSizeInput, bagConstraints);

        bagConstraints.gridx = 4;
        add(generateShapeColorLabel(), bagConstraints);

        bagConstraints.gridx = 5;
        shapeColorInput = generateShapeColorInput();
        add(shapeColorInput, bagConstraints);

        bagConstraints.gridx = 6;
        add(generateEdgeColorLabel(), bagConstraints);

        bagConstraints.gridx = 7;
        edgeColorInput = generateEdgeColorInput();
        add(edgeColorInput, bagConstraints);
    }

    private GridBagConstraints generateGridBagConstraints() {
        GridBagConstraints bagConstraints = new GridBagConstraints();

        bagConstraints.gridx = 0;
        bagConstraints.gridy = 0;
        bagConstraints.insets = new Insets(6, 8, 6, 8);

        return bagConstraints;
    }

    private JLabel generateShapeSizeLabel() {
        return new JLabel("Shape size:");
    }

    private JLabel generateShapeColorLabel() {
        return new JLabel("Shape color:");
    }

    private JLabel generateEdgeColorLabel() {
        return new JLabel("Edge color:");
    }

    private JLabel generateShapeTypeLabel() {
        return new JLabel("Shape type:");
    }

    private JComboBox<String> generateShapeTypeInput() {
        String[] types = {"circle", "square"};
        return new JComboBox<>(types);
    }

    private JSpinner generateShapeSizeInput() {
        return new JSpinner(new SpinnerNumberModel(30, 20, 60, 2));
    }

    private JComboBox<String> generateShapeColorInput() {
        String[] colors = {"blue", "red", "black", "green", "cyan"};
        return new JComboBox<>(colors);
    }

    private JComboBox<String> generateEdgeColorInput() {
        String[] colors = {"black", "yellow", "cyan", "red"};
        return new JComboBox<>(colors);
    }

    public ShapeType getShapeType() {
        return getShapeTypeFromInput();
    }

    public int getShapeSize() {
        if (shapeSizeInput == null) {
            return 30;
        }
        return (int) shapeSizeInput.getValue();
    }

    public Color getShapeColor() {
        return getShapeColorFromInput();
    }

    public Color getEdgeColor() {
        return getEdgeColorFromInput();
    }

    private ShapeType getShapeTypeFromInput() {
        if (shapeTypeInput == null) {
            return ShapeType.CIRCLE;
        }
        switch (shapeTypeInput.getSelectedIndex()) {
            case 0:
                return ShapeType.CIRCLE;
            case 1:
                return ShapeType.SQUARE;
        }
        return ShapeType.CIRCLE;
    }

    private Color getShapeColorFromInput() {
        if (shapeColorInput == null) {
            return Color.cyan;
        }
        switch (shapeColorInput.getSelectedIndex()) {
            case 0:
                return Color.blue;
            case 1:
                return Color.red;
            case 2:
                return Color.black;
            case 3:
                return Color.green;
            case 4:
                return Color.cyan;
        }
        return Color.cyan;
    }

    private Color getEdgeColorFromInput() {
        if (edgeColorInput == null) {
            return Color.red;
        }
        switch (edgeColorInput.getSelectedIndex()) {
            case 0:
                return Color.black;
            case 1:
                return Color.yellow;
            case 2:
                return Color.cyan;
            case 3:
                return Color.red;
        }
        return Color.red;
    }
}
