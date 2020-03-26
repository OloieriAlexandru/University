package view;

import controller.Controller;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;

/**
 * Contains the buttons which generate events
 */
public class ControlPanel extends JPanel {
    private JButton toggleDeleteModeButton = null;
    private Controller controller = null;

    public ControlPanel(Controller controller) {
        this.controller = controller;
        setPreferredSize(new Dimension(1280, 80));

        init();
    }

    private void init() {
        JButton loadButton = new JButton("Load");
        JButton saveButton = new JButton("Save");
        JButton resetButton = new JButton("Reset");
        toggleDeleteModeButton = new JButton("Delete mode: OFF");
        JButton redrawGraphButton = new JButton("Redraw graph");

        add(loadButton);
        add(saveButton);
        add(resetButton);
        add(toggleDeleteModeButton);
        add(redrawGraphButton);

        loadButton.addActionListener(this::onLoadButtonClicked);
        saveButton.addActionListener(this::onSaveButtonClicked);
        resetButton.addActionListener(this::onResetButtonClicked);
        toggleDeleteModeButton.addActionListener(this::onToggleDeleteModeButtonClicked);
        redrawGraphButton.addActionListener(this::onRedrawGraphButtonClicked);
    }

    public void setDeleteModeTextOnButton(boolean deleteMode) {
        if (deleteMode) {
            toggleDeleteModeButton.setText("Delete mode: ON");
        } else {
            toggleDeleteModeButton.setText("Delete mode: OFF");
        }
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

    private void onToggleDeleteModeButtonClicked(ActionEvent actionEvent) {
        controller.toggleDeleteMode();
    }

    private void onRedrawGraphButtonClicked(ActionEvent actionEvent) {
        controller.redrawGraph();
    }
}
