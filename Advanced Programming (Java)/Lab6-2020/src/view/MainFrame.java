package view;

import javax.swing.*;
import java.awt.*;

public class MainFrame extends JFrame {
    public MainFrame(Canvas canvas, ConfigurationPanel configurationPanel, ControlPanel controlPanel) {
        super("");
        init(canvas, configurationPanel, controlPanel);
    }

    private void init(Canvas canvas, ConfigurationPanel configurationPanel, ControlPanel controlPanel) {
        setPreferredSize(new Dimension(1400, 880));
        setLayout(new GridBagLayout());

        GridBagConstraints bagConstraints = generateGridBagConstraints();
        add(configurationPanel, bagConstraints);

        bagConstraints.weighty = 1;
        bagConstraints.gridy = 1;
        add(canvas, bagConstraints);

        bagConstraints.weighty = 0;
        bagConstraints.gridy = 2;
        add(controlPanel, bagConstraints);

        pack();

        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
    }

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
