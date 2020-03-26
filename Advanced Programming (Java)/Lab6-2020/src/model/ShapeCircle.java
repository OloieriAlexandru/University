package model;

import view.Circle;

import java.awt.*;
import java.awt.image.BufferedImage;
import java.io.Serializable;

public class ShapeCircle extends Shape implements Serializable {
    public ShapeCircle(double x, double y, int size, Color color) {
        super(x, y, size, color);
    }

    @Override
    public void draw(Graphics2D graphics2D, BufferedImage image) {
        Circle circle = new Circle(x, y, size);
        graphics2D.setColor(color);
        graphics2D.fill(circle);
    }

    /**
     * A point is inside the circle if the distance between that point and the center of the circle is less than the circle radius
     */
    @Override
    public boolean isInside(double cx, double cy) {
        double distance = Math.sqrt((cx - x) * (cx - x) + (cy - y) * (cy - y)) * 2;
        return distance < size;
    }

    @Override
    public boolean intersects(double cx, double cy, int sz) {
        double distance = Math.sqrt((cx - x) * (cx - x) + (cy - y) * (cy - y)) * 2;
        return distance <= (size + sz);
    }
}
