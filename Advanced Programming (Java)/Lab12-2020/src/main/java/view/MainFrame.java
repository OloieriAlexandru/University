package view;

import javax.swing.*;
import java.awt.*;

/**
 * The main frame of the application, on which all the other panels are located
 */
public class MainFrame extends JFrame {
    JSplitPane splitPane;

    public MainFrame(DesignPanel designPanel, ButtonsPanel buttonsPanel, ControlPanel controlPanel, ComponentPropertiesPanel componentPropertiesPanel) {
        super("Olo App");
        init(designPanel, buttonsPanel, controlPanel, componentPropertiesPanel);
    }

    private void init(DesignPanel designPanel, ButtonsPanel buttonsPanel, ControlPanel controlPanel, ComponentPropertiesPanel componentPropertiesPanel) {
        setPreferredSize(new Dimension(1500, 800));
        setLayout(new GridBagLayout());

        GridBagConstraints bagConstraints = generateGridBagConstraints();
        add(controlPanel, bagConstraints);

        splitPane = new JSplitPane();
        splitPane.setLeftComponent(componentPropertiesPanel);
        splitPane.setRightComponent(designPanel);
        splitPane.setResizeWeight(0.2);
        splitPane.setDividerLocation(0.2);

        bagConstraints.weighty = 1;
        bagConstraints.gridy = 1;
        add(splitPane, bagConstraints);

        bagConstraints.weighty = 0;
        bagConstraints.gridy = 2;
        add(buttonsPanel, bagConstraints);

        pack();

        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
    }

    public void setDesignPanel(DesignPanel designPanel) {
        splitPane.setRightComponent(designPanel);
    }

    /**
     * Generates the initial GridBagConstraints
     */
    private GridBagConstraints generateGridBagConstraints() {
        GridBagConstraints bagConstraints = new GridBagConstraints();

        bagConstraints.fill = GridBagConstraints.BOTH;
        bagConstraints.gridx = 0;
        bagConstraints.gridy = 0;
        bagConstraints.weightx = 1;
        bagConstraints.weighty = 0;

        return bagConstraints;
    }

    public void open() {
        setVisible(true);
    }
}
