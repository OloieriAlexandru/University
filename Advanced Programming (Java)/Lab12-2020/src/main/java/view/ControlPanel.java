package view;

import javax.swing.*;
import java.awt.*;
import java.io.Serializable;

/**
 * The panel that is located on the top of the application screen, which contains two text fields
 */
public class ControlPanel extends JPanel implements Serializable {
    private JTextField swingComponentTextField;
    private JTextField defaultTextField;

    public ControlPanel() {
        init();
    }

    private void init() {
        setLayout(new GridBagLayout());

        GridBagConstraints bagConstraints = generateGridBagConstraints();
        add(generateComponentNameLabel(), bagConstraints);

        bagConstraints.gridx = 1;
        swingComponentTextField = new JTextField(24);
        add(swingComponentTextField, bagConstraints);

        bagConstraints.gridx = 2;
        add(generateComponentInitialTextLabel(), bagConstraints);

        bagConstraints.gridx = 3;
        defaultTextField = new JTextField(24);
        add(defaultTextField, bagConstraints);
    }

    public String getComponentName() {
        return swingComponentTextField.getText();
    }

    public String getDefaultComponentText() {
        return defaultTextField.getText();
    }

    private GridBagConstraints generateGridBagConstraints() {
        GridBagConstraints bagConstraints = new GridBagConstraints();

        bagConstraints.gridx = 0;
        bagConstraints.gridy = 0;
        bagConstraints.insets = new Insets(6, 8, 6, 8);

        return bagConstraints;
    }

    private JLabel generateComponentNameLabel() {
        return new JLabel("Swing component: ");
    }

    private JLabel generateComponentInitialTextLabel() {
        return new JLabel("Default text: ");
    }

    public void setDefaultTextField(JTextField defaultTextField) {
        this.defaultTextField = defaultTextField;
    }

    public void setSwingComponentTextField(JTextField swingComponentTextField) {
        this.swingComponentTextField = swingComponentTextField;
    }

    public JTextField getDefaultTextField() {
        return defaultTextField;
    }

    public JTextField getSwingComponentTextField() {
        return swingComponentTextField;
    }
}
