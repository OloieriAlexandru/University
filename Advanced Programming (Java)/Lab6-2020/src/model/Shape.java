package model;

import helper.RandomGenerator;

import java.awt.*;
import java.awt.image.BufferedImage;
import java.io.Serializable;

/**
 * Specifies what shape will a node have
 * Abstract class, cannot be instantiated
 * Implemented derived classes: ShapeCircle, ShapeSquare
 */
public abstract class Shape implements Serializable {
    private double vx;
    private double vy;
    protected double x;
    protected double y;
    protected int size;
    protected Color color;

    public Shape(double x, double y, int size, Color color) {
        this.x = x;
        this.y = y;
        this.size = size;
        this.color = color;
    }

    /**
     * Draws the shape on a buffered image
     */
    public abstract void draw(Graphics2D graphics2D, BufferedImage image);

    /**
     * Checks if the point (cx,cy) is inside this shape
     */
    public abstract boolean isInside(double cx, double cy);

    /**
     * Checks if a shape of size "sz", located at (cx,cy), would intersect this shape
     */
    public abstract boolean intersects(double cx, double cy, int sz);

    public void setX(double x) {
        this.x = x;
    }

    public void setY(double y) {
        this.y = y;
    }

    public double getX() {
        return x;
    }

    public double getY() {
        return y;
    }

    /**
     * The function which is called before the first iteration of the Force Drawing Graph Algorithm
     */
    public void resetPosition() {
        this.x = RandomGenerator.getRandomValue();
        this.y = RandomGenerator.getRandomValue();
        this.vx = 0;
        this.vy = 0;
    }

    /**
     * Computes the final position of this node, after the last iteration of the Force Drawing Graph Algorithm
     */
    public void computePosition() {
        this.x = 340 + 700 * x;
        this.y = 10 + 700 * y;
    }

    public double getVx() {
        return vx;
    }

    public double getVy() {
        return vy;
    }

    public void setVx(double vx) {
        this.vx = vx;
    }

    public void setVy(double vy) {
        this.vy = vy;
    }
}
