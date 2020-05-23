package view;

import annotations.Height;
import annotations.Text;
import annotations.Width;

import javax.swing.JButton;
import java.awt.Graphics;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.geom.Point2D;
import java.awt.FontMetrics;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;

// https://happycoding.io/examples/java/swing/circle-button
@Width(200)
@Height(200)
@Text(value = "Alt text")
public class CircleButton extends JButton {
    private boolean mouseOver = false;
    private boolean mousePressed = false;
    int width;
    int height;

    public CircleButton(String text) {
        super(text);
        setOpaque(false);
        setFocusPainted(false);
        setBorderPainted(false);

        width = 100;
        height = 100;

        MouseAdapter mouseListener = new MouseAdapter() {

            @Override
            public void mousePressed(MouseEvent me) {
                if (contains(me.getX(), me.getY())) {
                    mousePressed = true;
                    repaint();
                }
            }

            @Override
            public void mouseReleased(MouseEvent me) {
                mousePressed = false;
                repaint();
            }

            @Override
            public void mouseExited(MouseEvent me) {
                mouseOver = false;
                mousePressed = false;
                repaint();
            }

            @Override
            public void mouseMoved(MouseEvent me) {
                mouseOver = contains(me.getX(), me.getY());
                repaint();
            }
        };

        addMouseListener(mouseListener);
        addMouseMotionListener(mouseListener);
    }

    private int getDiameter() {
        int diameter = Math.min(width, height);
        return diameter;
    }

    @Override
    public Dimension getPreferredSize() {
        FontMetrics metrics = getGraphics().getFontMetrics(getFont());
        int minDiameter = 10 + Math.max(metrics.stringWidth(getText()), metrics.getHeight());
        return new Dimension(minDiameter, minDiameter);
    }

    @Override
    public boolean contains(int x, int y) {
        int radius = getDiameter() / 2;
        return Point2D.distance(x, y, getWidth() / 2, getHeight() / 2) < radius;
    }

    @Override
    public void setBounds(int x, int y, int width, int height) {
        super.setBounds(x, y, width, height);
        width = width;
        height = height;
    }

    @Override
    public void paintComponent(Graphics g) {
        int diameter = getDiameter();
        int radius = diameter / 2;

        if (mousePressed) {
            g.setColor(Color.LIGHT_GRAY);
        } else {
            g.setColor(Color.WHITE);
        }
        g.fillOval(getWidth() / 2 - radius, getHeight() / 2 - radius, diameter, diameter);

        if (mouseOver) {
            g.setColor(Color.BLUE);
        } else {
            g.setColor(Color.BLACK);
        }
        g.drawOval(getWidth() / 2 - radius, getHeight() / 2 - radius, diameter, diameter);

        g.setColor(Color.BLACK);
        g.setFont(getFont());
        FontMetrics metrics = g.getFontMetrics(getFont());
        int stringWidth = metrics.stringWidth(getText());
        int stringHeight = metrics.getHeight();
        g.drawString(getText(), getWidth() / 2 - stringWidth / 2, getHeight() / 2 + stringHeight / 4);
    }
}