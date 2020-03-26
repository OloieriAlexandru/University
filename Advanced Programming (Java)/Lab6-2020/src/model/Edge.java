package model;

import java.awt.*;
import java.awt.geom.Line2D;
import java.awt.image.BufferedImage;
import java.io.Serializable;

public class Edge implements Serializable {
    private Node node;
    private Color color;

    public Edge(Node node, Color color) {
        this.node = node;
        this.color = color;
    }

    /**
     * Computes the distance between (x,y) and the line (x1,y1) - (this.node.getX(), this.node.getY())
     * If the distance is less than 5 (5 pixels), this edge will be then removed from the grapf
     */
    public double computeDistanceToPoint(double x1, double y1, double x, double y) {
        double x2 = node.getX();
        double y2 = node.getY();

        double A = x - x1;
        double B = y - y1;
        double C = x2 - x1;
        double D = y2 - y1;
        double E = -D;

        double dot = A * E + B * C;
        double len_sq = E * E + C * C;

        return Math.abs(dot) / Math.sqrt(len_sq);
    }

    /**
     * Checks if this edge has to be drawn (any edge from the graph will be drawn only once, but it appears twice, in both adjacency lists)
     */
    public void paint(Graphics2D graphics2D, BufferedImage image, Node from) {
        if (from.getIndex() >= node.getIndex()) {
            return;
        }
        draw(graphics2D, image, from);
    }

    public Node getNode() {
        return node;
    }

    public int getNodeIndex() {
        return node.getIndex();
    }

    /**
     * Draws this edge on a buffered image, which will be later drawn on a canvas
     */
    private void draw(Graphics2D graphics2D, BufferedImage image, Node from) {
        Line2D line2D = new Line2D.Double(from.getX(), from.getY(), node.getX(), node.getY());
        graphics2D.setStroke(new BasicStroke(2));
        graphics2D.setColor(color);
        graphics2D.draw(line2D);
    }
}
