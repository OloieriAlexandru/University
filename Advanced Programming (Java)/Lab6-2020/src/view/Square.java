package view;

import java.awt.geom.Rectangle2D;

/**
 * The square object that will be drawn on canvas
 */
public class Square extends Rectangle2D.Double {
    public Square(double x0, double y0, double l) {
        super(x0 - l / 2, y0 - l / 2, l, l);
    }
}
