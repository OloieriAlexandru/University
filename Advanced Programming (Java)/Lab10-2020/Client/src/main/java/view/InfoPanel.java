package view;

import javax.swing.*;
import java.awt.*;
import java.awt.image.BufferedImage;

public class InfoPanel extends JPanel {
    private BufferedImage infoPanel;
    private Graphics2D graphics2D;

    public InfoPanel() {
    }

    public void init(int player) {
        resetImageAndGraphics();
        drawInfo(player);
        repaint();
    }

    @Override
    public void paint(Graphics g) {
        super.paintComponent(g);

        Graphics2D g2 = (Graphics2D) g;
        g2.drawImage(infoPanel, 0, 0, this);
    }

    /**
     * Draws information about the player: its id (0 or 1)
     * And the color of his pieces
     */
    public void drawInfo(int player) {
        graphics2D.setColor(Color.BLACK);
        graphics2D.setFont(new Font("Arial", Font.PLAIN, 40));
        graphics2D.drawString("Player: " + player, 60, 60);

        Circle circle = new Circle(240, 300, 320);
        if (player == 0) {
            graphics2D.setColor(Color.ORANGE);
        } else {
            graphics2D.setColor(Color.CYAN);
        }
        graphics2D.fill(circle);
    }

    private void resetImageAndGraphics() {
        infoPanel = new BufferedImage(1280 - Board.BOARD_SIDE_PX, Board.BOARD_SIDE_PX, BufferedImage.TYPE_INT_ARGB);
        graphics2D = infoPanel.createGraphics();

        graphics2D.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
        graphics2D.setStroke(new BasicStroke((2)));
    }
}
