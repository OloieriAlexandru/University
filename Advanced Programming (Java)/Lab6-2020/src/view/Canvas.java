package view;

import controller.Controller;

import javax.swing.*;
import java.awt.*;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.awt.image.BufferedImage;

public class Canvas extends JPanel {
    private BufferedImage bufferedImage = null;

    public Canvas(Controller controller) {
        addMouseListener(new MouseAdapter() {
            @Override
            public void mouseClicked(MouseEvent e) {
                controller.canvasMouseClicked(e.getX(), e.getY());
            }
        });
    }

    /**
     * Paints the image
     * @param image the image to be painted
     */
    public void paintImage(BufferedImage image) {
        bufferedImage = image;
        repaint();
    }

    @Override
    public void paintComponent(Graphics g) {
        super.paintComponent(g);

        if (bufferedImage != null) {
            Graphics2D graphics2D = (Graphics2D) g;
            graphics2D.drawImage(bufferedImage, 0, 0, this);
        }
    }
}
