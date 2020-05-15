package view;

import controller.Controller;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;

/**
 * The panel that is located on the bottom of the application screen, which contains three buttons
 */
public class ButtonsPanel extends JPanel {
    private Controller controller = null;

    public ButtonsPanel(Controller controller) {
        this.controller = controller;
        setPreferredSize(new Dimension(1500, 80));

        init();
    }

    private void init() {
        JButton loadButton = new JButton("Load");
        JButton saveButton = new JButton("Save");
        JButton resetButton = new JButton("Reset");

        add(loadButton);
        add(saveButton);
        add(resetButton);

        loadButton.addActionListener(this::onLoadButtonClicked);
        saveButton.addActionListener(this::onSaveButtonClicked);
        resetButton.addActionListener(this::onResetButtonClicked);
    }

    private void onLoadButtonClicked(ActionEvent actionEvent) {
        controller.load();
    }

    private void onSaveButtonClicked(ActionEvent actionEvent) {
        controller.save();
    }

    private void onResetButtonClicked(ActionEvent actionEvent) {
        controller.reset();
    }
}
