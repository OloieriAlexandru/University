package model;

/**
 * The force drawing algorithm works with force objects
 * These object specify what values should be added to the x and y coordinate of a node
 */
public class Force {
    private double fx;
    private double fy;

    public Force() {
        fx = 0;
        fy = 0;
    }

    public Force(double x, double y) {
        fx = x;
        fy = y;
    }

    public void add(Force force) {
        fx += force.getFx();
        fy += force.getFy();
    }
    
    public double getFx() {
        return fx;
    }

    public double getFy() {
        return fy;
    }
}
