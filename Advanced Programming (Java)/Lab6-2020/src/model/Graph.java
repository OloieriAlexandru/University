package model;

import java.awt.*;
import java.awt.image.BufferedImage;

import java.io.*;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class Graph implements Serializable {
    private List<Node> nodes = null;
    private Map<Node, Integer> nodesIndexes = null;

    public Graph() {
        nodes = new ArrayList<>();
        nodesIndexes = new HashMap<>();
    }

    /**
     * Creates a buffered image, which will be then drawn on canvas
     */
    public BufferedImage drawGraph() {
        BufferedImage graphImage = new BufferedImage(1400, 720, BufferedImage.TYPE_INT_ARGB);
        Graphics2D graphics2D = graphImage.createGraphics();
        graphics2D.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);

        for (Node node : nodes) {
            node.paint(graphics2D, graphImage);
        }

        return graphImage;
    }

    /**
     * Initializes the graph nodes for the Force Drawing Algorithm
     */
    public void initDrawingPositions() {
        for (Node node : nodes) {
            node.resetPos();
        }
    }

    /**
     * Computes the final positions of the nodes in the Force Drawing Algorithm
     */
    public void computeNodesPositions() {
        for (Node node : nodes) {
            node.computePos();
        }
    }

    /**
     * An iteration of the Force Drawing Algorithm: computes some forces and moves nodes in different directions
     */
    public void updateNodesPosition() {
        List<Force> forces = new ArrayList<>();

        for (Node node : nodes) {
            forces.add(node.computeTotalForce(nodes));
        }

        for (int i = 0; i < nodes.size(); ++i) {
            nodes.get(i).updatePosition(forces.get(i), 0.01);
        }
    }

    /**
     * Serializes this graph
     */
    public void serialize(String path) {
        try {
            FileOutputStream fileOut = new FileOutputStream(path);
            ObjectOutputStream out = new ObjectOutputStream(fileOut);
            out.writeObject(this);
            out.close();
            fileOut.close();
        } catch (IOException e) {
            System.out.println(e.getMessage());
        }
    }

    /**
     * Creates a new "Graph" object from "path" file
     */
    public static Graph deserialize(String path) {
        Graph newGraph = null;
        try {
            FileInputStream fileIn = new FileInputStream(path);
            ObjectInputStream in = new ObjectInputStream(fileIn);
            newGraph = (Graph) in.readObject();
            in.close();
            fileIn.close();
        } catch (IOException | ClassNotFoundException e) {
            e.printStackTrace();
            return null;
        }
        return newGraph;
    }

    public void addNode(Shape shape) {
        Node newNode = new Node(nodes.size(), shape);
        nodesIndexes.put(newNode, nodes.size());
        nodes.add(newNode);
    }

    public void addEdge(Node node1, Node node2, Color edgeColor) {
        int idx1 = nodesIndexes.get(node1);
        int idx2 = nodesIndexes.get(node2);

        if (!nodes.get(idx1).isAdjacentTo(idx2)) {
            nodes.get(idx1).addAdjacentNode(node2, edgeColor);
            nodes.get(idx2).addAdjacentNode(node1, edgeColor);
        }
    }

    /**
     * Checks if it is possible to draw a node at position (x,y)
     * It's possible to do so only if that node would not intersect with any existing node
     */
    public boolean wouldOverlap(double x, double y, int size) {
        for (Node node : nodes) {
            if (node.intersects(x, y, size)) {
                return true;
            }
        }
        return false;
    }

    /**
     * Returns the node which has the point (x,y) inside it
     */
    public Node getIntersectingNode(double x, double y) {
        for (Node node : nodes) {
            if (node.isInside(x, y)) {
                return node;
            }
        }
        return null;
    }

    /**
     * Checks if the point (x,y) intersects any node/edge
     */
    public boolean checkIntersectionWithNodeEdge(double x, double y) {
        if (checkIntersectionWithNode(x, y)) {
            return true;
        }
        return checkIntersectionWithEdge(x, y);
    }

    /**
     * Checks if the point(x,y) intersects any node
     * If it does, the node is deleted
     */
    private boolean checkIntersectionWithNode(double x, double y) {
        for (Node node : nodes) {
            if (node.isInside(x, y)) {
                removeNode(node);
                return true;
            }
        }
        return false;
    }

    /**
     * Removes a node from the graph
     */
    private void removeNode(Node node) {
        int index = nodesIndexes.get(node);
        node.removeFromAdjacencyLists();
        for (int i = index + 1; i < nodes.size(); ++i) {
            nodesIndexes.put(nodes.get(i), i - 1);
            nodes.get(i).decreaseIndex();
        }
        nodes.remove(index);
    }

    /**
     * Checks if the node (x,y) intersects any edge
     * If it does, that edge is deleted
     */
    private boolean checkIntersectionWithEdge(double x, double y) {
        double min = Double.MAX_VALUE;
        double dist;
        Node minFrom = null;
        Edge minEdge = null;

        for (Node node : nodes) {
            for (Edge edge : node.getAdjacencyList()) {
                if (edge.getNodeIndex() > node.getIndex()) {
                    continue;
                }
                dist = edge.computeDistanceToPoint(node.getX(), node.getY(), x, y);
                if (dist < min) {
                    min = dist;
                    minFrom = node;
                    minEdge = edge;
                }
            }
        }

        if (min > 5.0) {
            return false;
        }
        removeEdge(minFrom, minEdge);
        return true;
    }

    /**
     * Removes an edge from the graph
     */
    private void removeEdge(Node from, Edge edge) {
        edge.getNode().removeAdjacentNode(from);
        from.removeAdjacentNode(edge.getNode());
    }
}
