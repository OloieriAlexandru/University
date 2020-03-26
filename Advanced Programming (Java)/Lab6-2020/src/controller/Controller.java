package controller;

import model.Shape;
import model.*;
import view.Canvas;
import view.ConfigurationPanel;
import view.ControlPanel;
import view.MainFrame;

import javax.swing.*;
import java.awt.*;
import java.nio.file.Paths;

/**
 * The class that coordinates the flow of the application
 */
public class Controller {
    private MainFrame mainFrame;
    private Canvas canvas = null;
    private ConfigurationPanel configurationPanel = null;
    private ControlPanel controlPanel = null;
    private Graph graph = null;

    private Node lastClickedNode = null;
    private boolean deleteMode = false;

    public Controller() {
        canvas = new Canvas(this);
        configurationPanel = new ConfigurationPanel();
        controlPanel = new ControlPanel(this);

        mainFrame = new MainFrame(canvas, configurationPanel, controlPanel);

        graph = new Graph();
    }

    /**
     * The function which opens the UI
     */
    public void openApp() {
        mainFrame.open();
    }

    /**
     * A function which will be called when the users clicks on the Canvas
     */
    public void canvasMouseClicked(int x, int y) {
        boolean repaint = false;
        if (!deleteMode && !graph.wouldOverlap(x, y, configurationPanel.getShapeSize())) {
            repaint = drawNewNode(x, y);
        } else {
            repaint = solveMouseClick(x, y);
        }
        if (repaint) {
            repaintGraph();
        }
    }

    /**
     * Draws a new graph node at (x,y), if possible
     */
    private boolean drawNewNode(int x, int y) {
        Color color = configurationPanel.getShapeColor();
        int size = configurationPanel.getShapeSize();
        lastClickedNode = null;

        Shape shape = null;
        switch (configurationPanel.getShapeType()) {
            case CIRCLE:
                shape = new ShapeCircle(x, y, size, color);
                break;
            case SQUARE:
                shape = new ShapeSquare(x, y, size, color);
                break;
        }
        graph.addNode(shape);

        return true;
    }

    /**
     * This function will be called in delete mode in ON or if it's impossible to draw the shape at (x,y)
     */
    private boolean solveMouseClick(int x, int y) {
        boolean res;
        if (deleteMode) {
            res = solveMouseClickDeleteMode(x, y);
        } else {
            res = solveMouseClickNonDeleteMode(x, y);
        }
        return res;
    }

    /**
     * This function checks if a node/an edge should be removed
     */
    private boolean solveMouseClickDeleteMode(int x, int y) {
        lastClickedNode = null;
        return graph.checkIntersectionWithNodeEdge(x, y);
    }

    /**
     * This function draws an edge between two nodes
     */
    private boolean solveMouseClickNonDeleteMode(int x, int y) {
        boolean res = false;
        Node intersectedNode = graph.getIntersectingNode(x, y);
        if (intersectedNode == null) {
            lastClickedNode = null;
        } else if (lastClickedNode == null || intersectedNode.equals(lastClickedNode)) {
            lastClickedNode = intersectedNode;
        } else {
            graph.addEdge(lastClickedNode, intersectedNode, configurationPanel.getEdgeColor());
            res = true;
            lastClickedNode = null;
        }
        return res;
    }

    /**
     * Loads the image from an external file (a serialized file)
     */
    public void load() {
        JFileChooser j = new JFileChooser(Paths.get("").toAbsolutePath().toString());
        int r = j.showOpenDialog(null);
        if (r == JFileChooser.APPROVE_OPTION) {
            Graph newGraph = Graph.deserialize(j.getSelectedFile().getAbsolutePath());
            if (newGraph != null) {
                graph = newGraph;
                repaintGraph();
            } else {
                System.out.println("Error!");
            }
        }
    }

    /**
     * Saves the image to an external file
     */
    public void save() {
        JFileChooser j = new JFileChooser(Paths.get("").toAbsolutePath().toString());
        int r = j.showSaveDialog(null);
        if (r == JFileChooser.APPROVE_OPTION) {
            if (graph != null) {
                graph.serialize(j.getSelectedFile().getAbsolutePath());
            }
        }
    }

    /**
     * Deletes all the shapes/edges
     */
    public void reset() {
        graph = new Graph();
        lastClickedNode = null;
        repaintGraph();
    }

    public void toggleDeleteMode() {
        lastClickedNode = null;
        deleteMode = !deleteMode;
        controlPanel.setDeleteModeTextOnButton(deleteMode);
    }

    /**
     * Draws the graph in a pretty mode
     */
    public void redrawGraph() {
        if (graph == null) {
            return;
        }
        int steps = 10000;
        graph.initDrawingPositions();
        for (int i = 0; i < steps; ++i) {
            graph.updateNodesPosition();
        }
        graph.computeNodesPositions();
        repaintGraph();
    }

    /**
     * Draws the graph on canvas
     */
    private void repaintGraph() {
        canvas.paintImage(graph.drawGraph());
    }
}
