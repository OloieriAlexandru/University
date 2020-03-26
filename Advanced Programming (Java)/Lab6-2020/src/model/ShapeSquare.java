package model;

import view.Square;

import java.awt.*;
import java.awt.image.BufferedImage;
import java.io.Serializable;

public class ShapeSquare extends Shape implements Serializable {
    public ShapeSquare(double x, double y, int size, Color color) {
        super(x, y, size, color);
    }

    @Override
    public void draw(Graphics2D graphics2D, BufferedImage image) {
        Square square = new Square(x, y, size);
        graphics2D.setColor(color);
        graphics2D.fill(square);
    }

    /**
     * A point is inside a square if its both x and y coordinates are between the x and y coordinates of two opposite square points
     */
    @Override
    public boolean isInside(double cx, double cy) {
        double x1 = x - size / 2.0;
        double x2 = x + size / 2.0;
        double y1 = y - size / 2.0;
        double y2 = y + size / 2.0;
        return (x1 <= cx && cx <= x2 && y1 <= cy && cy <= y2);
    }

    @Override
    public boolean intersects(double cx, double cy, int sz) {
        double distance = Math.sqrt((cx - x) * (cx - x) + (cy - y) * (cy - y)) * 2;
        return distance <= (size * 1.41 + sz);
    }
}
